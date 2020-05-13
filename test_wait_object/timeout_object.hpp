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

class object_timeout_observer
{
public:
    typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
    typedef std::chrono::milliseconds ms_t;

public:
    struct timeout_object_t
    {
        int id;
        explicit timeout_object_t() noexcept;
        explicit timeout_object_t(const int ms) noexcept;
        ~timeout_object_t();
        timeout_object_t(const timeout_object_t &) = delete;
        timeout_object_t &operator=(const timeout_object_t &) = delete;
        timeout_object_t(timeout_object_t &&) = delete;
        timeout_object_t &operator=(timeout_object_t &&) = delete;
    };
    struct timeout_object_info_t
    {
        timeout_object_t *obj;
        time_point_t start_point;
        time_point_t end_point;
        std::string obj_name;
        ms_t _threshold;
    };

private:
    static object_timeout_observer _s_obj;

    std::thread *_t;
    std::atomic_bool _quit;
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

    void add_object(timeout_object_t* obj, const std::string obj_name);
    void add_object(timeout_object_t* obj, const std::string obj_name, const int ms);
    void delete_object(const int id);
    void check_timeout();
};

void test_timeout_object_function();

#endif /* timeout_object_hpp */
