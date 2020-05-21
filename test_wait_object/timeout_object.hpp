//
//  timeout_object.hpp
//  test_wait_object
//
//  Created by AsteriskZuo on 2020/5/9.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#ifndef timeout_object_hpp
#define timeout_object_hpp

#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <list>
#include <unordered_map>
#include <chrono>
#include <type_traits>
#include <map>
#include <iostream>
#include <typeinfo>
#include <tuple>

class toop;

class timeout_object final
{
public:
    static timeout_object *instance();

public:
    ~timeout_object();

    void start_wait(const int ms);
    void stop_wait();
    void restart_wait(const int ms);
    void reset();

protected:
private:
    timeout_object();
    timeout_object(const timeout_object &o);
    timeout_object &operator=(const timeout_object &o);

    void _stop_and_wait();
    void _stop_and_wait2();

private:
    static timeout_object _s_obj;

private:
    std::mutex _mutex;
    std::condition_variable _variable;
    std::atomic_bool _stop;
    std::atomic_bool _waiting;
};

/**
 * timeout_object_two
 * 适用双线程控制模式：一个线程开始等待，一个线程结束等待
 * 对于任意多开始等待线程，或者任意多结束等待线程，行为不可知
 */
class timeout_object_two final
{
private:
    static timeout_object_two _s_obj;

private:
    std::mutex _mutex_start;
    std::mutex _mutex_wait;
    std::condition_variable _variable_start;
    std::condition_variable _variable_wait;
    bool _started;
    bool _waiting;
    bool _ended;
    bool _is_timeout;
    bool __waiting;
    bool __end_waiting;

public:
    static timeout_object_two *instance();

public:
    ~timeout_object_two();

    void init_wait();
    void uninit_wait();

    void start_wait(const int ms);
    void start_wait();
    void stop_wait();
    void restart_wait(const int ms);

    void start_wait_nb(const int ms);
    void start_wait_nb();
    void stop_wait_nb();
    void restart_wait_nb(const int ms);

    void start_wait_test(const int ms);
    void stop_wait_test();

    void reset();
    void wait_trigger();

protected:
private:
    timeout_object_two();
    timeout_object_two(const timeout_object_two &o);
    timeout_object_two &operator=(const timeout_object_two &o);

    friend class toop;
};

/**
 * toop is timeout object operation class
 */
class toop
{
private:
    static toop _toop_obj;

private:
    std::mutex _mutex;

public:
    static toop *instance();

public:
    void start_wait(const int ms);
    void stop_wait();

public:
private:
    toop(/* args */);
    ~toop();
    toop(const toop &o);
    toop &operator=(const toop &o);
};

/**
 * toop2 is timeout object operation class
 */
class toop2 final
{
    class queue
    {
        friend class toop2;

    public:
        typedef void (*task_t)(int);

    private:
        std::mutex _mutex;
        std::condition_variable _variable;
        std::atomic_bool _quit;

    public:
        void push(const task_t &task);
        task_t front_pop();

    protected:
        std::list<task_t> _ls;
    };
    class run
    {
    public:
        void operator()();
    };

private:
    static toop2 _s_obj;

    std::thread *_t;
    queue *_q;

public:
    static toop2 *instance();

    ~toop2();

    void start_wait(const int ms);
    void stop_wait();

protected:
    toop2();

private:
};

class test_timeout_object
{
private:
    class queue_t
    {
        friend class test_timeout_object;

    public:
        struct task_t
        {
            task_t()
            {
                static int sid = 0;
                id = sid++;
                if (0xff <= sid)
                {
                    id = sid = 0;
                }
            }
            // typedef void (*task_func)(int);
            typedef std::function<void(int)> task_func;
            int id;
            task_func func;
            int ms;
        };

    private:
        std::mutex _mutex;
        std::condition_variable _variable;
        std::atomic_bool _quit;

    public:
        void push(const task_t &task);
        task_t front_pop();
        void quit();

        queue_t();
        ~queue_t();

    protected:
        std::list<task_t> _ls;
    };

private:
    static test_timeout_object _s_obj;

    typedef test_timeout_object::queue_t::task_t task_t;

    test_timeout_object::queue_t *_q;
    std::thread *_t;
    std::atomic_bool _quit;

    std::atomic_bool _started; //true:started false:stoped

public:
    static test_timeout_object *instance();
    static void run();

    test_timeout_object();
    ~test_timeout_object();

    void start();
    void stop();

    void start_wait(const int ms);
    void stop_wait();

private:
    void _start_wait(const int ms);
};

/**
 * 对象超时观察者
 * 主要任务:对现有布局对象生命周期进行检测，是否在规定时间内结束，如果超时则给出警告或者作出相应处理。
 */
class object_timeout_observer
{
public:
    typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
    typedef std::chrono::milliseconds ms_t;

public:
    struct timeout_object_t
    {
        int id;
        explicit timeout_object_t(const std::string name) noexcept;
        explicit timeout_object_t(const std::string name, const int ms) noexcept;
        ~timeout_object_t();
        timeout_object_t(const timeout_object_t &) = delete;
        timeout_object_t &operator=(const timeout_object_t &) = delete;
        timeout_object_t(timeout_object_t &&) = delete;
        timeout_object_t &operator=(timeout_object_t &&) = delete;
    };

private:
    struct timeout_object_info_t
    {
        friend class object_timeout_observer;
        timeout_object_t *obj;
        time_point_t start_point;
        time_point_t end_point;
        std::string obj_name;
        ms_t _threshold;
        bool _enable_log;
    };

private:
    static object_timeout_observer _s_obj;

    std::thread *_t;
    std::atomic_bool _quit;
    std::atomic_bool _is_init;
    std::mutex _mutex;
    // std::list<timeout_object_t *> _ls;
    // std::list<std::unique_ptr<timeout_object_t>> _ls;
    std::unordered_map<int, std::shared_ptr<timeout_object_info_t>> _ls;
    ms_t _threshold;

private:
    object_timeout_observer();
    ~object_timeout_observer();

    static void run();

public:
    static object_timeout_observer *instance();

    void start();
    void stop();

    void add_object(timeout_object_t *obj, const std::string &obj_name);
    void add_object(timeout_object_t *obj, const std::string &obj_name, const int ms);
    void delete_object(const int id);
    void check_timeout();
};

/**
 * 网络重复任务判断
 * 主要应用场景:网络抖动（连接状态重复报告问题)
 */
class network_task_judge final
{
public:
    struct task_t
    {
        explicit task_t() noexcept;
        enum network_status_t
        {
            no_network,
            network_wifi,
            network_mobile
        };
        network_status_t network_status;
        std::string network_info;
        std::chrono::milliseconds task_create_point;
        int id;
    };

    typedef void (*task_notify_t)(const task_t *task);

private:
    struct task_queue_t
    {
        task_queue_t(std::chrono::milliseconds &threshold, const task_notify_t &callback);
        ~task_queue_t();

        void push_back(const std::string &network_status, const std::string &network_info);

        void push_back(network_task_judge::task_t *task);
        network_task_judge::task_t *front_pop();
        void quit();
        void dispose(const network_task_judge::task_t *task);

        void _clear();

        std::mutex _mutex;
        std::condition_variable _variable;
        std::atomic_bool _quit;
        std::list<network_task_judge::task_t *> _ls;
        std::chrono::milliseconds &_threshold;
        task_notify_t _cb;
    };

public:
    static network_task_judge *instance();

    void start(const int ms, const task_notify_t &callback);
    void stop();

    void add_task(const std::string network_status, const std::string network_info);

private:
    explicit network_task_judge() noexcept;
    ~network_task_judge() noexcept;

    static void run();

private:
    static network_task_judge _s_obj;

    std::thread *_t;
    std::atomic_bool _quit;
    std::atomic_bool _is_init;
    std::mutex _mutex;
    std::chrono::milliseconds _threshold;

    network_task_judge::task_queue_t *_q;
};

namespace std
{
#if _LIBCPP_STD_VER > 17
#else
    template <class T>
    struct remove_cvref
    {
        typedef remove_cv_t<remove_reference_t<T>> type;
    };
#endif
#if _LIBCPP_STD_VER > 17
#else
    template <class _Fp, class... _Args, size_t... _Indices>
    static void __execute_v2(std::tuple<_Fp, _Args...> &__t, std::index_sequence<_Indices...>)
    {
        std::move(std::get<0>(__t))(std::move(std::get<_Indices>(__t))...);
    }
    template <class _Fp, class... _Args, size_t... _Indices>
    static void __invoke_v2(std::tuple<_Fp, _Args...> &__t, std::index_sequence<_Indices...>)
    {
        auto ret = std::index_sequence<(_Indices + 1)...>(); // +1
        __execute_v2(__t, ret);
    }
    template <class _Fp>
    static void *_invoke_v2(_Fp &&__v)
    {
        auto ret = std::make_index_sequence<std::tuple_size<_Fp>::value - 1>(); // -1
        __invoke_v2(__v, ret);
        return nullptr;
    }
    template <class _Fp, class... _Args>
    static void invoke_v2(_Fp &&f, _Args &&... args)
    {
        _invoke_v2(std::make_tuple(std::move(f), std::move(args)...));
    }
#endif
    template <class _Tp>
    typename decay<_Tp>::type
    decay_copy(_Tp &&__t)
    {
        return forward<_Tp>(__t);
    }
} // namespace std

extern std::mutex s_mutex;

static const char *test_current_time();
static long long test_get_time_difference();

class simple_thread_pool final
{
public:
    typedef void *task_param_t;
    typedef void (*task_func_t)(task_param_t);

public:
    /**
     * 没有采用继承std::thread的方式，因为它不建议继承使用
     * 采用第二种方案，使用成员的方式进行封装使用
     * 需要提供所有thread的公共方法
     */
    class thread_t final
    {
    private:
        int id;
        std::thread _t;

    private:
        thread_t(const thread_t &) = delete;
        thread_t &operator=(const thread_t &) = delete;

    public:
        thread_t() noexcept {}
        ~thread_t();
        template <class _Fp, class... _Args,
                  class = typename std::enable_if<
                      !std::is_same<typename std::remove_cvref<_Fp>::type, std::thread>::value>::type>
        explicit thread_t(_Fp &&f, _Args &&... args) : _t(f, args...) {}
        thread_t(thread_t &&t) _NOEXCEPT : _t(std::move(t._t)) {}
        thread_t &operator=(thread_t &&t) _NOEXCEPT
        {
            _t.operator=(std::move(t._t));
            return *this;
        }
        void swap(thread_t &t) noexcept { std::swap(_t, t._t); }
        bool joinable() const noexcept { return _t.joinable(); }
        void join() { _t.join(); }
        void detach() { _t.detach(); }
        std::thread::id get_id() const noexcept { return _t.get_id(); }
        std::thread::native_handle_type native_handle() noexcept { return _t.native_handle(); }

        static unsigned hardware_concurrency() noexcept { return std::thread::hardware_concurrency(); }
    };

    struct task_base_t
    {
        int id;
        int pri;

        explicit task_base_t(const int priority) noexcept : pri(priority)
        {
            static std::atomic_int _s_id = {0};
            if (0xffff <= (id = _s_id.fetch_add(1)))
                id = _s_id.exchange(0);
        }
        virtual ~task_base_t() {}
        virtual const std::type_info &get_really_type() = 0;
    };

    template <class _Fp, class... _Args>
    struct task_t : public task_base_t
    {
        std::tuple<typename std::decay<_Fp>::type, typename std::decay<_Args>::type...> params;
        // std::tuple<_Fp, _Args...> params;

        // explicit task_t() noexcept {}
        explicit task_t(const int priority, _Fp &&f, _Args &&... args) noexcept
            : task_base_t(priority), params(std::move(f), std::move(args)...)
        {
        }
        const std::type_info &get_really_type() override
        {
            return typeid(*this);
        }
    };
    // template <>
    // struct task_t<task_func_t, task_param_t> : public task_base_t
    // {
    //     task_func_t func;
    //     task_param_t param;
    //
    //     explicit task_t(const task_func_t &f, const task_param_t &p, const int priority = 0) noexcept
    //         : task_base_t(priority), func(f), param(p)
    //     {
    //     }
    //     const std::type_info &get_really_type() override
    //     {
    //         return typeid(*this);
    //     }
    // };

    struct task_queue_base_t
    {
        std::mutex _mutex;
        std::condition_variable _variable;
        std::atomic_bool _quit;
        simple_thread_pool *_tp;

        explicit task_queue_base_t(simple_thread_pool *tp) noexcept : _tp(tp), _quit(false) {}
        virtual ~task_queue_base_t() {}
        virtual void push_back(task_base_t *task) = 0;
        virtual task_base_t *front_pop() = 0;
        virtual void clear() = 0;
        virtual std::list<task_base_t *> get_task_list() = 0;
        virtual void quit() = 0;
        //        template <class _Fp, class... _Args>
        //        task_base_t *create_task(int priority, _Fp &&f, _Args &&... args) { return nullptr; }
        //        task_base_t *create_task(const task_func_t &func, const task_param_t &param, int priority = 0) { return nullptr; }
        virtual void destory_task(task_base_t **task) = 0;
    };

    template <class _Fp, class... _Args>
    struct task_queue_t;

    struct thread_run_t
    {
        task_queue_base_t *_q;
        thread_run_t(task_queue_base_t *q) : _q(q) {}
        virtual ~thread_run_t() {}

        template <class _Fp, class... _Args>
        void run();
    };

    struct thread_queue_t
    {
        std::atomic_bool _is_started;
        std::list<std::pair<thread_t *, thread_run_t *>> _ls;
        task_queue_base_t *_q;

        explicit thread_queue_t(task_queue_base_t *q) noexcept;
        ~thread_queue_t();

        template <class _Fp, class... _Args>
        void start(const int max);
        void stop();
    };

    template <class _Fp, class... _Args>
    struct task_queue_t : public task_queue_base_t
    {
        std::map<std::pair<int, int>, task_t<_Fp, _Args...> *> _tasks;

        explicit task_queue_t(simple_thread_pool *tp) noexcept : task_queue_base_t(tp) {}
        ~task_queue_t() { clear(); }

        void push_back(task_base_t *task) override
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                task_t<_Fp, _Args...> *t = dynamic_cast<task_t<_Fp, _Args...> *>(task);
                _tasks.insert(std::make_pair(std::make_pair(t->pri, t->id), t));
            }
            _variable.notify_one();
        }
        task_base_t *front_pop() override
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _variable.wait(lock, [this]() -> bool {
                    return _quit.load() || 0 != _tasks.size();
                });
            }
            task_base_t *ret = nullptr;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_tasks.size() && !_quit.load())
                {
                    ret = _tasks.begin()->second;
                    _tasks.erase(std::make_pair(ret->pri, ret->id));
                }
            }
            return ret;
        }
        void clear() override
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
            {
                task_t<_Fp, _Args...> *t = iter->second;
                delete t;
            }
            _tasks.clear();
        }
        std::list<task_base_t *> get_task_list() override
        {
            std::list<task_base_t *> ret;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                for (auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
                {
                    ret.push_back(iter->second);
                }
                _tasks.clear();
            }
            return ret;
        }
        void quit() override
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _quit.store(true);
            }
            _variable.notify_all();
        }

        task_base_t *create_task(int priority, _Fp &&f, _Args &&... args)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            task_t<_Fp, _Args...> *task = new task_t<_Fp, _Args...>(priority, std::move(f), std::move(args)...);
            assert(task);
            _tasks.insert(std::make_pair(std::make_pair(task->pri, task->id), task));
            return task;
        }
        void destory_task(task_base_t **task) override
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (task && *task)
            {
                auto iter = _tasks.find(std::make_pair((*task)->pri, (*task)->id));
                if (iter != _tasks.end())
                {
                    _tasks.erase(iter);
                    delete *task;
                }
            }
            *task = nullptr;
        }
    };

    // template <>
    // struct task_queue_t<task_func_t, task_param_t> : public task_queue_base_t
    // {
    //     std::map<std::pair<int, int>, task_t<task_func_t, task_param_t> *> _tasks;
    //
    //     explicit task_queue_t(simple_thread_pool *tp) noexcept : task_queue_base_t(tp) {}
    //     ~task_queue_t() { clear(); }
    //
    //     void push_back(task_base_t *task) override
    //     {
    //         {
    //             std::lock_guard<std::mutex> lock(_mutex);
    //             task_t<task_func_t, task_param_t> *t = dynamic_cast<task_t<task_func_t, task_param_t> *>(task);
    //             _tasks.insert(std::make_pair(std::make_pair(t->pri, t->id), t));
    //             {
    //                 std::lock_guard<std::mutex> lock(s_mutex);
    //                 std::cout << __FUNCTION__ << ":" << __LINE__
    //                           << ":t=" << test_current_time()
    //                           << ":els=" << test_get_time_difference()
    //                           << ":tid=" << std::this_thread::get_id()
    //                           << ":pri=" << t->pri
    //                           << ":oid=" << t->id
    //                           << ":sz=" << _tasks.size()
    //                           << std::endl;
    //             }
    //         }
    //         _variable.notify_one();
    //     }
    //     task_base_t *front_pop() override
    //     {
    //         {
    //             std::unique_lock<std::mutex> lock(_mutex);
    //             _variable.wait(lock, [this]() -> bool {
    //                 return _quit.load() || 0 != _tasks.size();
    //             });
    //         }
    //         task_t<task_func_t, task_param_t> *ret = nullptr;
    //         {
    //             std::lock_guard<std::mutex> lock(_mutex);
    //             if (_tasks.size() && !_quit.load())
    //             {
    //                 ret = _tasks.begin()->second;
    //                 _tasks.erase(std::make_pair(ret->pri, ret->id));
    //             }
    //         }
    //         return ret;
    //     }
    //     void clear() override
    //     {
    //         std::lock_guard<std::mutex> lock(_mutex);
    //         std::cout << __FUNCTION__ << ":" << __LINE__
    //                   << ":t=" << test_current_time()
    //                   << ":els=" << test_get_time_difference()
    //                   << ":tid=" << std::this_thread::get_id()
    //                   << std::endl;
    //         for (auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    //         {
    //             task_t<task_func_t, task_param_t> *t = iter->second;
    //             delete t;
    //         }
    //     }
    //     std::list<task_base_t *> get_task_list() override
    //     {
    //         std::list<task_base_t *> ret;
    //         {
    //             std::lock_guard<std::mutex> lock(_mutex);
    //             for (auto iter = _tasks.begin(); iter != _tasks.end(); ++iter)
    //             {
    //                 ret.push_back(iter->second);
    //             }
    //             _tasks.clear();
    //         }
    //         return ret;
    //     }
    //     void quit() override
    //     {
    //         {
    //             std::lock_guard<std::mutex> lock(_mutex);
    //             _quit.store(true);
    //         }
    //         _variable.notify_all();
    //     }
    //
    //     task_base_t *create_task(const task_func_t &func, const task_param_t &param, int priority = 0)
    //     {
    //         std::lock_guard<std::mutex> lock(_mutex);
    //         task_t<task_func_t, task_param_t> *task = new task_t<task_func_t, task_param_t>(func, param, priority);
    //         assert(task);
    //         _tasks.insert(std::make_pair(std::make_pair(task->pri, task->id), task));
    //         return task;
    //     }
    //     void destory_task(task_base_t **task) override
    //     {
    //         std::lock_guard<std::mutex> lock(_mutex);
    //         if (task && *task)
    //         {
    //             task_t<task_func_t, task_param_t> *t = dynamic_cast<task_t<task_func_t, task_param_t> *>(*task);
    //             auto iter = _tasks.find(std::make_pair(t->pri, t->id));
    //             if (iter != _tasks.end())
    //             {
    //                 _tasks.erase(iter);
    //                 delete *task;
    //             }
    //         }
    //         *task = nullptr;
    //     }
    // };

    enum task_strategy_t
    {
        task_strategy_join_all_task,
        task_strategy_cancel_all_task,
    };

    enum task_queue_strategy_t
    {
        task_queue_strategy_fix_param,
        task_queue_strategy_variable_param
    };

    /**
     * 通知任务运行状态
     */
    class task_notify_interface
    {
    public:
        virtual void task_run_before() = 0;
        virtual void task_run_after() = 0;
    };

private:
    task_queue_base_t *_task_queue;
    thread_queue_t *_thread_queue;
    task_notify_interface *_task_notify;
    task_strategy_t _task_strategy;
    task_queue_strategy_t _task_queue_strategy;
    int _thread_count;

public:
public:
    simple_thread_pool(/* args */);
    ~simple_thread_pool();

    static simple_thread_pool *instance();
    static void uninstance();

    /**
     * 初始化
     * 
     * @param thread_count 并发线程数，建议不要超过处理器的线程数
     */
    void init(const int thread_count);

    /**
     * 初始化 变参版本
     * 注意：模板在编译期决定，所以一旦这里确定了参数类型以及个数，后续postv方法一定要保证一致
     * 
     * @param thread_count 并发线程数
     */
    template <class _Fp, class... _Args>
    void initv(const int thread_count);

    /**
     * 反初始化
     * 初始化之后，使用完毕必须使用该方法才能正常退出，否则线程无法正常退出
     */
    void uninit();

    /**
     * 异步任务执行
     * priority [-1 - 1] -1 低优先级 0 普通 1 高优先级
     * 调用init之后，请使用该方法
     * 
     * @param func 函数体 需要使用者持有
     * @param param 参数对象 需要使用者持有
     * @param priority 任务优先级
     */
    void post(task_func_t func, task_param_t param, int priority = 0);

    /**
     * 异步任务执行 变参版本
     * priority [-1 - 1] -1 低优先级 0 普通 1 高优先级
     * 调用initv之后，请使用该方法
     * 注意: 该版本不支持 void(*)(void*)的方法
     * 
     * @param f 函数体 调用者持有，使用者不需要关心
     * @param args 变参 调用者持有，使用者不需要关心 
     * @param priority 任务优先级
     */
    template <class _Fp, class... _Args>
    void postv(int priority, _Fp &&f, _Args &&... args);

    /**
     * 设置任务通知，默认没有通知
     * 设置通知之后，可以收到任务执行前后等通知
     * 
     * @param notify 通知对象
     */
    void set_notify(task_notify_interface *notify);

    /**
     * 获取并发线程数
     * @return 返回并发线程数
     */
    int get_thread_count() { return _thread_count; }

    /**
     * 获取变参策略
     * @return 返回变参策略
     */
    task_queue_strategy_t get_parameter_strategy() { return _task_queue_strategy; }

    /**
     * 获取任务策略
     * @return 返回任务策略
     */
    task_strategy_t get_task_strategy() { return _task_strategy; }

    /**
     * 获取通知监听对象
     * @return 返回监听对象
     */
    task_notify_interface *get_notify_object() { return _task_notify; }

private:
private:
    static simple_thread_pool *_s_obj;
    std::atomic_bool _is_init;
};

void test_timeout_object_function();

#endif /* timeout_object_hpp */
