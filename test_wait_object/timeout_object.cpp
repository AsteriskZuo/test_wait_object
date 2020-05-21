//
//  timeout_object.cpp
//  test_wait_object
//
//  Created by AsteriskZuo on 2020/5/9.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "timeout_object.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>
#include <stdexcept>

std::mutex s_mutex;

static const char *test_current_time();
static long long test_get_time_difference();
static void test1();
static void test3();
static void test5();
static void test7();
static void test8();
static void test9();
static void test10();
static void test11();
static void test12();
static void test13();
static void test14();
static void test15();
static void test16();
static void test17();
static void test18();
static void test19();
static void test22();
static void test23();
static void test24();
static void test25();
static void test26();
static void test27();
static void test28();
static void test29();
static void test30();
static void test31();
static void test32();
static void test33();
static void test34();
static void test35();
static void test36();
static void test37();
static void test38();
void test_timeout_object_function()
{
    test29();
}

timeout_object timeout_object::_s_obj;
timeout_object *timeout_object::instance()
{
    return &_s_obj;
}
timeout_object::timeout_object()
    : _stop(false)
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
}
timeout_object::~timeout_object()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
}
void timeout_object::start_wait(const int ms)
{

    if (true)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":before:" << __LINE__ << std::endl;
        }
        _waiting.store(true);
        _variable.wait_for(lock, std::chrono::milliseconds(ms), [this]() -> bool {
            {
                std::lock_guard<std::mutex> lock(s_mutex);
                std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":" << __LINE__ << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // test
            }
            return this->_stop.load();
        });
        _waiting.store(false);
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":after:" << __LINE__ << std::endl;
        }
    }
}
void timeout_object::stop_wait()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
    _stop.store(true);
    _variable.notify_one();
}
void timeout_object::restart_wait(const int ms)
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
    _stop_and_wait();
    reset();
    start_wait(ms);
}
void timeout_object::reset()
{
    _stop.store(false);
}
void timeout_object::_stop_and_wait()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":before:" << __LINE__ << std::endl;
    }
    _stop.store(true);
    _variable.notify_one();
    while (_waiting.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":after:" << __LINE__ << std::endl;
    }
}
void timeout_object::_stop_and_wait2()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":before:" << __LINE__ << std::endl;
    }
    _stop.store(true);
    _variable.notify_one();
    if (true)
    {
        std::this_thread::yield();
        std::unique_lock<std::mutex> lock(_mutex); //利用互斥对象进行同步等待，但是多线程可能有问题
    }
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time() << "][" << test_get_time_difference() << "]" << __func__ << ":after:" << __LINE__ << std::endl;
    }
}

timeout_object_two timeout_object_two::_s_obj;

timeout_object_two *timeout_object_two::instance()
{
    return &_s_obj;
}

timeout_object_two::timeout_object_two()
    : _started(false), _waiting(false), _ended(false), _is_timeout(false), __waiting(false), __end_waiting(false)
{
}
timeout_object_two::~timeout_object_two()
{
}

void timeout_object_two::start_wait(const int ms)
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "][ms:" << ms << "]" << __func__ << ":before----------:" << __LINE__ << std::endl;
    }

    {
        std::unique_lock<std::mutex> lock(_mutex_start);                                                //1
        __waiting = true;                                                                               //2
        _is_timeout = !_variable_start.wait_for(lock, std::chrono::milliseconds(ms), [this]() -> bool { //3
            return !this->_started;                                                                     //4
        });                                                                                             //5
        __waiting = false;                                                                              //6
    }
    if (!_is_timeout)
    {
        std::cout << __func__ << ":" << __LINE__ << ":_is_timeout:" << _is_timeout << std::endl;
        _waiting = false;
        _variable_wait.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "][ms:" << ms << "]" << __func__ << ":after----------:" << __LINE__ << std::endl;
    }
}
void timeout_object_two::start_wait()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":before----------:" << __LINE__ << std::endl;
    }

    {
        std::unique_lock<std::mutex> lock(_mutex_start); //1
        __waiting = true;                                //2
        _variable_start.wait(lock, [this]() -> bool {    //3
            return !this->_started;                      //4
        });                                              //5
        __waiting = false;                               //6
    }
    std::cout << __func__ << ":" << __LINE__ << ":_is_timeout:" << _is_timeout << std::endl;
    _waiting = false;
    _variable_wait.notify_one();

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":after----------:" << __LINE__ << std::endl;
    }
}
void timeout_object_two::stop_wait()
{
    /**
     * 尝试取消另一个线程的等待，并且等待这个线程的结束等待通知
     * 说明1：_mutex_start获取到锁的情况：1.等待前、2.等待中、3.等待后
     * 说明2：等待前(//1之前)：_mutex_start没有被占用，并且__waiting为false
     *       等待中(//3-5)：_mutex_start没有被占用，并且__waiting为true
     *       等待后(//6之后)：_mutex_start没有被占用，并且__waiting为false
     *       其它过程中(//1-3 和//5-6)：_mutex_start被占用
     */

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":before:" << __LINE__ << std::endl;
    }

    std::unique_lock<std::mutex> lock(_mutex_wait);
    if (_mutex_start.try_lock())
    {
        _mutex_start.unlock();
        if (__waiting)
        {
            //保证进入这里是安全的
            //如果进入这里，保证start_wait正在等待
            _started = false;
            _variable_start.notify_one();
            _variable_wait.wait(lock, [this]() -> bool {
                return !this->_waiting;
            });
        }
    }

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":after:" << __LINE__ << std::endl;
    }
}
static void test______________1()
{
    timeout_object_two::instance()->stop_wait_test();
}
void timeout_object_two::start_wait_test(const int ms)
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "][ms:" << ms << "]" << __func__ << ":before----------:" << __LINE__ << std::endl;
    }

    {
        std::unique_lock<std::mutex> lock(_mutex_start); //1
        //        std::thread t(test______________1);
        //        t.join();
        __waiting = true;                                                                               //2
        _is_timeout = !_variable_start.wait_for(lock, std::chrono::milliseconds(ms), [this]() -> bool { //3
            //             std::this_thread::sleep_for(std::chrono::milliseconds(50));
            //            std::this_thread::yield();
            return !this->_started; //4
        });                         //5
        __waiting = false;          //6
    }
    std::cout << __func__ << ":" << __LINE__ << ":_is_timeout:" << _is_timeout << std::endl;
    if (!_is_timeout)
    {
        //        if (_mutex_wait.try_lock()) // 可能无法结束等待对象_variable_wait的等待
        _mutex_wait.lock();
        {
            std::cout << __func__ << ":" << __LINE__ << ":__end_waiting:" << __end_waiting << std::endl;
            if (__end_waiting)
            {
                _mutex_wait.unlock();
                _waiting = false;
                _variable_wait.notify_one();
            }
            else
            {
                _mutex_wait.unlock();
            }
        }
        //        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //        std::this_thread::yield();
    }

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "][ms:" << ms << "]" << __func__ << ":after----------:" << __LINE__ << std::endl;
    }
}
void timeout_object_two::stop_wait_test()
{
    /**
     * 尝试取消另一个线程的等待，并且等待这个线程的结束等待通知
     * 说明1：_mutex_start获取到锁的情况：1.等待前、2.等待中、3.等待后
     * 说明2：等待前(//1之前)：_mutex_start没有被占用，并且__waiting为false
     *       等待中(//3-5)：_mutex_start没有被占用，并且__waiting为true
     *       等待后(//6之后)：_mutex_start没有被占用，并且__waiting为false
     *       其它过程中(//1-3 和//5-6)：_mutex_start被占用
     */

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":before:" << __LINE__ << std::endl;
    }

    std::unique_lock<std::mutex> lock(_mutex_wait);
    if (_mutex_start.try_lock())
    {
        if (__waiting)
        {
            _mutex_start.unlock();
            //保证进入这里是安全的
            //如果进入这里，保证start_wait正在等待
            _started = false;
            _variable_start.notify_one();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); //假设耗时较多
            __end_waiting = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); //假设耗时较多
            _variable_wait.wait(lock, [this]() -> bool {
                return !this->_waiting;
            });
            __end_waiting = false;
        }
        else
        {
            _mutex_start.unlock();
        }
    }

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":after:" << __LINE__ << std::endl;
    }
}
void timeout_object_two::start_wait_nb(const int ms)
{
    std::unique_lock<std::mutex> lock(_mutex_start);
    _variable_start.wait_for(lock, std::chrono::milliseconds(ms), [this]() -> bool {
        return !this->_started;
    });
}
void timeout_object_two::start_wait_nb()
{
    std::unique_lock<std::mutex> lock(_mutex_start);
    _variable_start.wait(lock, [this]() -> bool {
        return !this->_started;
    });
}
void timeout_object_two::stop_wait_nb()
{
    std::unique_lock<std::mutex> lock(_mutex_wait);
    _started = false;
    _variable_start.notify_one();
}
void timeout_object_two::restart_wait(const int ms)
{
    // TODO: 同一个线程无法实现
}
void timeout_object_two::restart_wait_nb(const int ms)
{
    // TODO: 无法实现
}
void timeout_object_two::reset()
{
    if (!_ended)
    {
        _started = true;
        _waiting = true;
    }
}
void timeout_object_two::init_wait()
{
    _ended = false;
    _started = true;
    _waiting = true;
}
void timeout_object_two::uninit_wait()
{
    _ended = true;
    _started = false;
    _waiting = false;
}
void timeout_object_two::wait_trigger()
{
    /**
     * 使用情况1：只有两个线程，一个是等待线程，一个是结束等待线程 (ok)
     * 使用情况2：有任意多个等待线程并发执行，只有一个结束等待线程 (?)
     * 使用情况3：有任意多个等待线程，有任意多个结束等待线程 (?)
     */
    static std::mutex _s_mutex;
    static bool f = false;
    if (false == f)
    {
        f = !f;
    }
    else if (true == f)
    {
        f = !f;
    }
}

toop toop::_toop_obj;

toop *toop::instance()
{
    return &_toop_obj;
}

toop::toop(/* args */)
{
}

toop::~toop()
{
}

void toop::start_wait(const int ms)
{
    timeout_object_two *s_obj = timeout_object_two::instance();
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (s_obj->_started)
        {
            s_obj->stop_wait();
        }
    }
    {
        std::lock_guard<std::mutex> lock(_mutex);
        s_obj->reset();
        s_obj->start_wait(ms);
    }
}
void toop::stop_wait()
{
    std::lock_guard<std::mutex> lock(_mutex);
    timeout_object_two::instance()->stop_wait();
}

toop2 toop2::_s_obj;
toop2 *toop2::instance()
{
    return &_s_obj;
}
toop2::toop2()
    : _t(new std::thread(toop2::run())), _q(new toop2::queue())
{
}
toop2::~toop2()
{
    if (_t->joinable())
    {
        _t->join();
    }
    delete _t;
    _t = nullptr;
    delete _q;
    _q = nullptr;
}
void toop2::start_wait(const int ms)
{
}
void toop2::stop_wait()
{
}

void toop2::run::operator()()
{
}

void toop2::queue::push(const toop2::queue::task_t &task)
{
    _ls.push_back(std::move(task));
}
toop2::queue::task_t toop2::queue::front_pop()
{
    task_t t;
    std::swap(t, _ls.front());
    _ls.pop_front();
    return t;
}

test_timeout_object::queue_t::queue_t()
    : _quit(false)
{
}
test_timeout_object::queue_t::~queue_t()
{
}
void test_timeout_object::queue_t::push(const test_timeout_object::queue_t::task_t &task)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _ls.push_back(std::move(task));
    }
    _variable.notify_one();
}
test_timeout_object::queue_t::task_t test_timeout_object::queue_t::front_pop()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _variable.wait(lock, [this]() -> bool {
            return this->_quit || 0 != this->_ls.size();
        });
    }
    task_t t;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (0 < _ls.size() && !this->_quit)
        {
            std::swap(t, _ls.front());
            _ls.pop_front();
        }
    }
    return t;
}
void test_timeout_object::queue_t::quit()
{
    _quit.store(true);
    _variable.notify_one();
}

test_timeout_object test_timeout_object::_s_obj;
test_timeout_object *test_timeout_object::instance()
{
    return &_s_obj;
}
test_timeout_object::test_timeout_object()
    : _started(false)
{
}
test_timeout_object::~test_timeout_object()
{
}
void test_timeout_object::start()
{
    if (!_started.exchange(true))
    {
        _q = new queue_t();
        _t = new std::thread(&test_timeout_object::run);
        _quit = false;
    }
}
void test_timeout_object::stop()
{
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":before:" << __LINE__ << std::endl;
    }
    if (_started.exchange(false))
    {
        if (_t)
        {
            _quit.store(true);
            _q->quit();
            if (_t->joinable())
            {
                _t->join();
            }
            delete _t;
        }
        if (_q)
        {
            delete _q;
        }
    }
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[cur:" << test_current_time()
                  << "][els:" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":after:" << __LINE__ << std::endl;
    }
}
void test_timeout_object::run()
{
    timeout_object_two::instance()->init_wait();
    test_timeout_object *_obj = test_timeout_object::instance();
    for (;;)
    {
        if (_obj->_quit.load())
        {
            if (0 < _obj->_q->_ls.size())
            {
                _obj->_q->_ls.clear(); //?
            }
            break;
        }
        task_t &&t = _obj->_q->front_pop();
        if (t.func)
        {
            t.func(t.ms);
        }
    }
    timeout_object_two::instance()->uninit_wait();
}
void test_timeout_object::start_wait(const int ms)
{
    task_t t;
    t.ms = ms;
    t.func = std::bind(&test_timeout_object::_start_wait, this, std::placeholders::_1);
    _q->push(t);
}
void test_timeout_object::stop_wait()
{
    timeout_object_two::instance()->stop_wait_test();
}
void test_timeout_object::_start_wait(const int ms)
{
    timeout_object_two::instance()->reset();
    timeout_object_two::instance()->start_wait_test(ms);
}

static std::atomic_int _timeout_object_t_id = {0};

object_timeout_observer::timeout_object_t::timeout_object_t(const std::string name) noexcept
    : timeout_object_t(std::move(name), 0)
{
    /* 1.error */
    //    timeout_object_t(std::move(name), 0);

    /* 2.repetition */
    //    if (0xffff <= (id = _timeout_object_t_id.fetch_add(1)))
    //        id = _timeout_object_t_id.exchange(0);
    //    object_timeout_observer::instance()->add_object(std::addressof(*this), name, 0);
}
object_timeout_observer::timeout_object_t::timeout_object_t(const std::string name, const int ms) noexcept
{
    if (0xffff <= (id = _timeout_object_t_id.fetch_add(1)))
        id = _timeout_object_t_id.exchange(0);
    object_timeout_observer::instance()->add_object(std::addressof(*this), name, ms);
}
object_timeout_observer::timeout_object_t::~timeout_object_t()
{
    object_timeout_observer::instance()->delete_object(this->id);
}

object_timeout_observer object_timeout_observer::_s_obj;

object_timeout_observer *object_timeout_observer::instance()
{
    return &_s_obj;
}

object_timeout_observer::object_timeout_observer()
    : _quit(false), _is_init(false), _threshold(std::chrono::milliseconds(20))
{
}

object_timeout_observer::~object_timeout_observer()
{
}

void object_timeout_observer::start()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_is_init.exchange(true))
    {
        _quit.store(false);
        if (!_t)
        {
            _t = new std::thread(&object_timeout_observer::run);
        }
    }
}
void object_timeout_observer::stop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_is_init.exchange(false))
    {
        _quit.store(true);
        if (_t)
        {
            if (_t->joinable())
            {
                _t->join();
            }
            delete _t;
            _t = nullptr;
        }
    }
}

void object_timeout_observer::add_object(timeout_object_t *obj, const std::string &obj_name)
{
    add_object(obj, obj_name, 0);
}
void object_timeout_observer::add_object(timeout_object_t *obj, const std::string &obj_name, const int ms)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::shared_ptr<timeout_object_info_t> obj_info_ptr = std::make_shared<timeout_object_info_t>();
    obj_info_ptr->obj = obj;
    obj_info_ptr->start_point = std::chrono::system_clock::now();
    obj_info_ptr->end_point = std::chrono::time_point<std::chrono::system_clock>();
    obj_info_ptr->obj_name = std::move(obj_name);
    obj_info_ptr->_threshold = std::chrono::milliseconds(ms);
    obj_info_ptr->_enable_log = true;
    _ls.insert(std::make_pair(obj->id, obj_info_ptr));
}
void object_timeout_observer::delete_object(const int id)
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto iter = _ls.find(id);
    if (iter != _ls.end())
    {
        const std::shared_ptr<timeout_object_info_t> &obj_info_ptr = iter->second;
        auto end_point = std::chrono::system_clock::now();
        auto ret = end_point - obj_info_ptr->start_point;
        if (ret >= (obj_info_ptr->_threshold.count() ? obj_info_ptr->_threshold : _threshold))
        {
            //超时
            std::cout << __FUNCTION__ << ":" << __LINE__
                      << ":name=" << obj_info_ptr->obj_name.c_str()
                      << ":threshold=" << std::chrono::duration_cast<std::chrono::milliseconds>(obj_info_ptr->_threshold.count() ? obj_info_ptr->_threshold : _threshold).count()
                      << ":ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(ret).count() << std::endl;
        }
        else
        {
            //在规定时间
            std::cout << __FUNCTION__ << ":" << __LINE__
                      << ":name=" << obj_info_ptr->obj_name.c_str() << std::endl;
        }
        _ls.erase(iter);
    }
}
void object_timeout_observer::check_timeout()
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto end_point = std::chrono::system_clock::now();
    for (auto iter = _ls.begin(); iter != _ls.end(); ++iter)
    {
        const std::shared_ptr<timeout_object_info_t> &obj_info_ptr = iter->second;
        auto ret = end_point - obj_info_ptr->start_point;
        if (obj_info_ptr->_enable_log && ret >= (obj_info_ptr->_threshold.count() ? obj_info_ptr->_threshold : _threshold))
        {
            //超时
            std::cout << __FUNCTION__ << ":" << __LINE__
                      << ":name=" << obj_info_ptr->obj_name.c_str()
                      << ":threshold=" << std::chrono::duration_cast<std::chrono::milliseconds>(obj_info_ptr->_threshold.count() ? obj_info_ptr->_threshold : _threshold).count()
                      << ":ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(ret).count() << std::endl;
            obj_info_ptr->_enable_log = false; //检测超时只打印一次，但是对于删除无效
        }
        else
        {
            //在规定时间
            //            std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
        }
    }
}

void object_timeout_observer::run()
{
    object_timeout_observer *obj = object_timeout_observer::instance();
    while (true)
    {
        if (obj->_quit)
            break;
        //        std::cout << __FUNCTION__ << ":" << __LINE__ << ":size=" << obj->_ls.size() << std::endl;
        if (0 != obj->_ls.size())
            obj->check_timeout();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

#define TIMEOUT_CHECK_OBJECT object_timeout_observer::timeout_object_t d46319f193e6461b89178020841fd685(__FUNCTION__);
#define TIMEOUT_CHECK_OBJECT_PARAM(x) object_timeout_observer::timeout_object_t e8e6ca0ea2da478393e95f26cbd2b14d(__FUNCTION__, x);

network_task_judge network_task_judge::_s_obj;

network_task_judge *network_task_judge::instance()
{
    return &_s_obj;
}

network_task_judge::network_task_judge() noexcept
    : _quit(false), _is_init(false)
{
}
network_task_judge::~network_task_judge() noexcept
{
}

void network_task_judge::run()
{
    network_task_judge *obj = network_task_judge::instance();
    for (;;)
    {
        if (obj->_quit.load())
        {
            break;
        }
        task_t *task = obj->_q->front_pop();
        if (task)
        {
            obj->_q->dispose(task);
        }
    }
}

void network_task_judge::start(const int ms, const task_notify_t &callback)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference() << std::endl;
    if (!_is_init.exchange(true))
    {
        _quit.store(false);
        _threshold = std::chrono::milliseconds(ms);
        if (!_q)
        {
            _q = new network_task_judge::task_queue_t(_threshold, callback);
        }
        if (!_t)
        {
            _t = new std::thread(&network_task_judge::run);
        }
    }
}
void network_task_judge::stop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference() << ":before:" << std::endl;
    if (_is_init.exchange(false))
    {
        _quit.store(true);
        if (_q)
        {
            _q->quit();
        }
        if (_t)
        {
            if (_t->joinable())
            {
                _t->join();
            }
            delete _t;
            _t = nullptr;
        }
        if (_q)
        {
            delete _q; //!!!
            _q = nullptr;
        }
    }
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference() << ":after:" << std::endl;
}

void network_task_judge::add_task(const std::string network_status, const std::string network_info)
{
    std::lock_guard<std::mutex> lock(_mutex);
    //    std::cout << __FUNCTION__ << ":" << __LINE__
    //              << ":t=" << test_current_time()
    //              << ":els=" << test_get_time_difference() << std::endl;
    if (_is_init.load())
    {
        _q->push_back(network_status, network_info);
    }
}

network_task_judge::task_queue_t::task_queue_t(std::chrono::milliseconds &threshold, const task_notify_t &callback)
    : _quit(false), _threshold(threshold), _cb(callback)
{
}
network_task_judge::task_queue_t::~task_queue_t()
{
}
void network_task_judge::task_queue_t::push_back(const std::string &network_status, const std::string &network_info)
{
    network_task_judge::task_t *task = new network_task_judge::task_t();
    assert(task);
    if (network_status.empty())
    {
        task->network_status = network_task_judge::task_t::no_network;
    }
    else
    {
        if (network_status.find("wifi"))
            task->network_status = network_task_judge::task_t::network_wifi;
        else
            task->network_status = network_task_judge::task_t::network_mobile;
    }
    task->network_info = network_info;
    push_back(task);
}
void network_task_judge::task_queue_t::push_back(network_task_judge::task_t *task)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":id=" << task->id
                  << ":status=" << task->network_status
                  << ":tmsp=" << task->task_create_point.count() << std::endl;
        _ls.push_back(task);
    }
    _variable.notify_one();
}
network_task_judge::task_t *network_task_judge::task_queue_t::front_pop()
{
    /**
     * case 1: 没有任务，一直超时等待，等待间隔_threshold
     * case 2: 一个周期只有一个任务: 1.等待刚开始（概率很低） 2.等待一定时间（小于一个周期）
     * case 3: 一个周期出现多个任务: 1.
     */
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _variable.wait_for(lock, _threshold, [this]() -> bool {
            return _quit.load() || 0 != _ls.size();
        });
    }
    task_t *task = nullptr;
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_ls.size() && !_quit.load())
        {
            task = _ls.back(); //maybe is nullptr
            assert(task);
            std::cout << __FUNCTION__ << ":" << __LINE__
                      << ":t=" << test_current_time()
                      << ":els=" << test_get_time_difference()
                      << ":id=" << task->id
                      << ":status=" << task->network_status
                      << ":tmsp=" << task->task_create_point.count() << std::endl;
            auto &&timeout_point = task->task_create_point + _threshold;
            auto &&now_point = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto &&diff = timeout_point - now_point;
            if (0 < diff.count())
            {
                std::cout << __FUNCTION__ << ":" << __LINE__
                          << ":t=" << test_current_time()
                          << ":els=" << test_get_time_difference()
                          << ":diff=" << diff.count() << std::endl;
                //没有超时
                task = nullptr;
                while (1 < _ls.size())
                {
                    task_t *tmp = _ls.front();
                    delete tmp;
                    _ls.pop_front();
                }
                _variable.wait_for(lock, diff, [this]() -> bool { //避免浪费资源
                    return _quit.load();
                });
            }
            else
            {
                std::cout << __FUNCTION__ << ":" << __LINE__
                          << ":t=" << test_current_time()
                          << ":els=" << test_get_time_difference()
                          << ":id=" << task->id
                          << ":status=" << task->network_status
                          << ":tmsp=" << task->task_create_point.count() << std::endl;
                //超时
                _ls.pop_back();
                _clear();
            }
        }
    }
    return task;
}
void network_task_judge::task_queue_t::quit()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference() << std::endl;
        _quit.store(true);
        _clear();
    }
    _variable.notify_one();
}
void network_task_judge::task_queue_t::_clear()
{
    for (auto iter = _ls.begin(); iter != _ls.end(); ++iter)
    {
        task_t *task = *iter;
        if (task)
        {
            delete task;
        }
    }
    _ls.clear();
}
void network_task_judge::task_queue_t::dispose(const network_task_judge::task_t *task)
{
    /**
     * 用锁说明: 
     * 1.dispose和push_back不是一个线程: 操作不冲突
     * 2.dispose和front_pop是一个线程: 同一个线程不会同时执行
     * 3.dispose和quit不是一个线程: 操作冲突
     */
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_quit.load())
    {
        if (_cb)
        {
            _cb(task);
        }
        if (task)
        {
            delete task;
        }
    }
}

network_task_judge::task_t::task_t() noexcept
{
    static std::atomic_int _s_id = {0};
    if (0xffff <= (id = _s_id.fetch_add(1)))
        id = _s_id.exchange(0);
    task_create_point = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

simple_thread_pool *simple_thread_pool::_s_obj = nullptr;

simple_thread_pool *simple_thread_pool::instance()
{
    if (!_s_obj)
    {
        _s_obj = new simple_thread_pool();
    }
    assert(_s_obj);
    return _s_obj;
}

void simple_thread_pool::uninstance()
{
    if (_s_obj)
    {
        delete _s_obj;
    }
    _s_obj = nullptr;
}

simple_thread_pool::simple_thread_pool(/* args */)
    : _thread_count(5), _is_init(false), _task_notify(nullptr), _task_strategy(task_strategy_join_all_task), _task_queue_strategy(task_queue_strategy_fix_param)
{
}

simple_thread_pool::~simple_thread_pool()
{
    uninit();
}

void simple_thread_pool::init(const int thread_count)
{
    if (!_is_init.exchange(true))
    {
        throw std::logic_error("init is deprecated, please initv method.");
    }
}
template <class _Fp, class... _Args>
void simple_thread_pool::initv(const int thread_count)
{
    if (!_is_init.exchange(true))
    {
        _thread_count = thread_count;
        _task_queue_strategy = task_queue_strategy_variable_param;
        _task_queue = new task_queue_t<_Fp, _Args...>(this);
        assert(_task_queue);
        _thread_queue = new thread_queue_t(_task_queue);
        assert(_thread_queue);
        _thread_queue->start<_Fp, _Args...>(_thread_count);
    }
}
void simple_thread_pool::uninit()
{
    if (_is_init.exchange(false))
    {
        if (_task_queue)
        {
            _task_queue->quit();
        }
        if (_thread_queue)
        {
            _thread_queue->stop();
            delete _thread_queue;
        }
        _thread_queue = nullptr;
        if (_task_queue)
        {
            delete _task_queue;
        }
        _task_queue = nullptr;
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << std::endl;
    }
}
void simple_thread_pool::post(task_func_t func, task_param_t param, int priority /*= 0*/)
{
    throw std::logic_error("post is deprecated, please postv method.");
}
template <class _Fp, class... _Args>
void simple_thread_pool::postv(int priority, _Fp &&f, _Args &&... args)
{
    if (task_queue_strategy_variable_param != _task_queue_strategy)
    {
        std::stringstream ss;
        ss << __FUNCTION__ << ":" << __LINE__ << ":error="
           << "not_support_variable_parameter";
        throw std::runtime_error(ss.str().c_str());
    }

    // static_assert(!std::is_same<_Fp, void(*)(void*)>::value, "please use post method.");
    // static_assert(!std::is_same<typename std::decay<_Fp>::type, void (*)(void *)>::value, "please use post method.");

    task_queue_t<_Fp, _Args...> *q = dynamic_cast<task_queue_t<_Fp, _Args...> *>(_task_queue);
    if (!q)
    {
        throw std::logic_error("No initialization, or template parameters do not match.");
    }
    task_base_t *task = q->create_task(priority, std::move(f), std::move(args)...);
    // task_base_t *task = q->create_task_ref(priority, std::decay_copy(std::forward(f)), std::decay_copy(std::forward(args))...);
    _task_queue->push_back(task);
}
void simple_thread_pool::set_notify(task_notify_interface *notify)
{
    _task_notify = notify;
}

simple_thread_pool::thread_queue_t::thread_queue_t(task_queue_base_t *q) noexcept
    : _q(q)
{
}
simple_thread_pool::thread_queue_t::~thread_queue_t()
{
    stop();
}
template <class _Fp, class... _Args>
void simple_thread_pool::thread_queue_t::start(const int max)
{
    if (!_is_started.exchange(true))
    {
        for (size_t i = 0; i < max; i++)
        {
            thread_run_t *r = new thread_run_t(_q);
            thread_t *t = new thread_t(&thread_run_t::run<_Fp, _Args...>, r);
            _ls.push_back(std::make_pair(t, r));
        }
    }
}
void simple_thread_pool::thread_queue_t::stop()
{
    if (_is_started.exchange(false))
    {
        for (auto iter = _ls.begin(); iter != _ls.end(); ++iter)
        {
            if (iter->first->joinable())
            {
                iter->first->join();
            }
            delete iter->second;
            delete iter->first;
        }
        _ls.clear();
    }
}

simple_thread_pool::thread_t::~thread_t()
{
}

template <class _Fp, class... _Args>
void simple_thread_pool::thread_run_t::run()
{
    for (;;)
    {
        if (_q->_quit.load())
        {
            switch (_q->_tp->_task_strategy)
            {
            case task_strategy_join_all_task:
            {
                auto &&ts = _q->get_task_list();
                for (auto iter = ts.begin(); iter != ts.end(); ++iter)
                {
                    task_base_t *task = *iter;
                    task_t<_Fp, _Args...> *t = nullptr;
                    if (static_cast<void>(t = dynamic_cast<task_t<_Fp, _Args...> *>(task)), nullptr == t)
                    {
                        std::stringstream ss;
                        ss << __FUNCTION__ << ":" << __LINE__ << ":err="
                           << "not_parse_param";
                        throw std::logic_error(ss.str().c_str());
                    }
                    std::_invoke_v2(std::move(t->params));
                    _q->destory_task(&task);
                }
            }
            break;
            case task_strategy_cancel_all_task:
            {
                _q->clear();
            }
            break;
            default:
            {
                _q->clear();
            }
            break;
            }
            break;
        }

        task_base_t *task = _q->front_pop();
        if (task)
        {
            task_t<_Fp, _Args...> *t = nullptr;
            if (static_cast<void>(t = dynamic_cast<task_t<_Fp, _Args...> *>(task)), nullptr == t)
            {
                std::stringstream ss;
                ss << __FUNCTION__ << ":" << __LINE__ << ":err="
                   << "not_parse_param";
                throw std::logic_error(ss.str().c_str());
            }
            if (_q->_tp->_task_notify)
                _q->_tp->_task_notify->task_run_before();
            std::_invoke_v2(std::move(t->params));
            if (_q->_tp->_task_notify)
                _q->_tp->_task_notify->task_run_after();
            _q->destory_task(&task);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
////  test partition                                                       ////
///////////////////////////////////////////////////////////////////////////////

/**
 * 返回当前时间字符串
 */
static const char *test_current_time()
{
    if (false)
    {
        static char now_buffer[9] = {0};
        std::time_t now_time = std::time(NULL);
        std::tm *now_tm = localtime(&now_time);
        std::memset(now_buffer, 0, 9);
        std::strftime(now_buffer, 8, "%H:%M:%S", now_tm);
        return now_buffer;
    }

    if (true)
    {
        //hh:mm:ss.zzz
        static char now_buffer[1024] = {0};
        std::memset(now_buffer, 0, 1024);
        std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
        std::chrono::system_clock::duration dur = tp.time_since_epoch();
        auto ds = std::chrono::duration_cast<std::chrono::duration<long, std::ratio<3600 * 24>>>(dur);
        auto hs = std::chrono::duration_cast<std::chrono::duration<long, std::ratio<3600>>>(dur - ds);
        auto hs_tmp = std::chrono::duration_cast<std::chrono::duration<long, std::ratio<3600>>>(dur);
        auto ms = std::chrono::duration_cast<std::chrono::duration<long, std::ratio<60>>>(dur - hs_tmp);
        auto ms_tmp = std::chrono::duration_cast<std::chrono::duration<long, std::ratio<60>>>(dur);
        auto ss = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1>>>(dur - ms_tmp);
        auto ss_tmp = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1>>>(dur);
        auto mills = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1, 1000>>>(dur - ss_tmp);
        std::sprintf(now_buffer, "%ld:%ld:%lld.%lld", hs.count(), ms.count(), ss.count(), mills.count());
        return now_buffer;
    }

    //    std::chrono::time_point<std::chrono::system_clock> now_time_point = std::chrono::system_clock::now();
    //    std::time_t now_time = std::chrono::system_clock::to_time_t(now_time_point);
    //    std::tm* now_tm = localtime(&now_time);
    //    char now_buffer[80] = { 0 };
    //    std::strftime(now_buffer, 80, "%H:%M:%S", now_tm);
    //    return std::string(now_buffer).c_str();
    //    std::cout << std::put_time(now_tm, "%H:%M:%S");
    //    std::stringstream ss;
    //    ss << std::put_time(now_tm, "%H:%M:%S");
    //    ss.flush();
    //    return ss.str().c_str();
}

static std::chrono::milliseconds s_last;

/**
 * @return now ms
 */
static std::chrono::milliseconds test_get_current_time()
{
    std::chrono::time_point<std::chrono::system_clock> now_time_point = std::chrono::system_clock::now();
    std::chrono::system_clock::duration now_since_epoch = now_time_point.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now_since_epoch);
}
/**
 * @param from ms
 * @param to ms
 * @return difference value
 */
static long long test_get_time_difference_from_to(const std::chrono::milliseconds from, const std::chrono::milliseconds to)
{
    assert(from >= to);
    return (from - to).count();
}
/**
 * @param last ms
 * @return difference value from 1970.01.01 00:00:00
 */
static long long test_get_time_difference_from_now(const long long last)
{
    auto now_ms = test_get_current_time();
    auto last_ms = std::chrono::milliseconds(last);
    return test_get_time_difference_from_to(now_ms, last_ms);
}
/**
 * @return difference value from 1970.01.01 00:00:00
 */
static long long test_get_time_difference()
{
    auto now_ms = test_get_current_time();
    auto last_ms = s_last;
    s_last = now_ms;
    return test_get_time_difference_from_to(now_ms, last_ms);
}

static void test_start(const int ms)
{
    timeout_object::instance()->start_wait(ms);
}
static void test_stop()
{
    timeout_object::instance()->stop_wait();
}
static void test_restart(const int ms)
{
    timeout_object::instance()->restart_wait(ms);
}

static void test1()
{
    test_current_time();
    s_last = test_get_current_time();
    std::thread t1(test_start, 6000);
    t1.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //    test_current_time();
    std::thread t2(test_restart, 4000);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    //    test_current_time();
    std::thread t3(test_stop);
    t3.detach();
    test_current_time();
}

static void test_start2(const int ms)
{
    timeout_object_two::instance()->start_wait(ms);
}
static void test_stop2()
{
    timeout_object_two::instance()->stop_wait();
}
static void test_reset2()
{
    timeout_object_two::instance()->reset();
}

static void test2()
{
    s_last = test_get_current_time();
    std::thread t1(test_start2, 6000);
    t1.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::thread t2(test_stop2);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::thread t3(test_reset2);
    t3.detach();
}

static void test_toop_start(const int ms)
{
    toop::instance()->start_wait(ms);
}
static void test_toop_stop()
{
    toop::instance()->stop_wait();
}
static void test_toop_start_group()
{
    std::thread ts[100];
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 10);
    for (size_t i = 0; i < 100; i++)
    {
        int ms = 1000 * (dist(rd)); //1000-10000 ms
        std::thread ti(test_toop_start, ms);
        ts[i] = std::move(ti);
    }
    for (size_t i = 0; i < 100; i++)
    {
        ts[i].join();
    }
}
static void test3()
{
    s_last = test_get_current_time();
    std::thread t1(test_toop_start_group);
    t1.join();
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // std::thread t2(test_toop_stop);
    // t2.join();
}

static void test_thread()
{
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::cout << ss.str().c_str() << std::endl;
}
static void test4()
{
    std::thread t(test_thread);
    t.join();
}

static void test5()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 10);
    for (size_t i = 0; i < 10; i++)
    {
        std::cout << dist(rd) << std::endl;
    }
}

static void task_fun(const int ms)
{
    timeout_object_two::instance()->start_wait(ms);
}
static void test_queue()
{
    typedef void (*task3_t)(int);
    std::list<task3_t> ls;
    ls.push_back(task_fun);
}
static void test_queue2()
{
    typedef std::function<void(int)> task2_t;
    std::list<task2_t> ls;
    ls.push_back(task2_t(std::bind(task_fun, 2)));
    for (auto iter = ls.begin(); iter != ls.end(); ++iter)
    {
        task2_t t = *iter;
        t(2);
    }
}
static void test6()
{
    std::thread t(test_queue);
    t.detach();
}

static bool test_condition_lock_p = false;
static std::mutex test_condition_lock_m;
static std::condition_variable test_condition_lock_v;
static void test_condition_lock_sub()
{
    std::mutex &_m(test_condition_lock_m);
    if (_m.try_lock())
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
        _m.unlock();
    }
    else
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
}
static void test_condition_lock()
{
    std::mutex &_m(test_condition_lock_m);
    std::condition_variable &_v(test_condition_lock_v);
    bool &p = test_condition_lock_p;

    if (_m.try_lock())
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
        _m.unlock();
    }
    else
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
    }

    {
        std::unique_lock<std::mutex> _l(_m);
        std::thread t(test_condition_lock_sub);
        t.join();
        if (_m.try_lock())
        {
            std::cout << "[" << test_current_time()
                      << "][" << test_get_time_difference()
                      << "][t:" << std::this_thread::get_id()
                      << "]" << __func__ << ":" << __LINE__ << std::endl;
            _m.unlock();
        }
        else
        {
            std::cout << "[" << test_current_time()
                      << "][" << test_get_time_difference()
                      << "][t:" << std::this_thread::get_id()
                      << "]" << __func__ << ":" << __LINE__ << std::endl;
        }
        _v.wait_for(_l, std::chrono::milliseconds(4000), [p, &_m]() -> bool {
            if (_m.try_lock())
            {
                std::cout << "[" << test_current_time()
                          << "][" << test_get_time_difference()
                          << "][t:" << std::this_thread::get_id()
                          << "]" << __func__ << ":" << __LINE__ << std::endl;
                _m.unlock();
            }
            else
            {
                std::cout << "[" << test_current_time()
                          << "][" << test_get_time_difference()
                          << "][t:" << std::this_thread::get_id()
                          << "]" << __func__ << ":" << __LINE__ << std::endl;
            }
            std::cout << "p:" << p << std::endl;
            return p;
        });
        if (_m.try_lock())
        {
            std::cout << "[" << test_current_time()
                      << "][" << test_get_time_difference()
                      << "][t:" << std::this_thread::get_id()
                      << "]" << __func__ << ":" << __LINE__ << std::endl;
            _m.unlock();
        }
        else
        {
            std::cout << "[" << test_current_time()
                      << "][" << test_get_time_difference()
                      << "][t:" << std::this_thread::get_id()
                      << "]" << __func__ << ":" << __LINE__ << std::endl;
        }
    }
    if (_m.try_lock())
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
        _m.unlock();
    }
    else
    {
        std::cout << "[" << test_current_time()
                  << "][" << test_get_time_difference()
                  << "][t:" << std::this_thread::get_id()
                  << "]" << __func__ << ":" << __LINE__ << std::endl;
    }
}
/**
 * 测试互斥锁在条件变量下的状态
 */
static void test7()
{
    s_last = test_get_current_time();
    std::thread t(test_condition_lock);
    t.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    test_condition_lock_p = true;
    test_condition_lock_v.notify_one();
}

static void test_timeout_object_start()
{
    for (int i = 0; i < 2; ++i)
    {
        std::cout << "start:" << i << std::endl;
        test_timeout_object::instance()->start_wait(4000);
    }
}
static void test_timeout_object_stop()
{
    for (int i = 0; i < 2; ++i)
    {
        std::cout << "stop:" << i << std::endl;
        test_timeout_object::instance()->stop_wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    }
}
/**
 * 测试test_timeout_object对象
 * 在临界值下的是否正常
 */
static void test8()
{
    s_last = test_get_current_time();
    test_timeout_object::instance()->start();
    std::thread t1(test_timeout_object_start);
    t1.detach();
    std::thread t2(test_timeout_object_stop);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    test_timeout_object::instance()->stop();
}

static void test_timeout_object_start2()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 4);
    for (int i = 0; i < 10; ++i)
    {
        int r = dist(rd);
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            std::cout << "start:" << i << ":" << r << std::endl;
        }
        test_timeout_object::instance()->start_wait(1000 * r);
    }
}
static void test_timeout_object_stop2()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 4);
    for (int i = 0; i < 10; ++i)
    {
        int r = dist(rd);
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            std::cout << "stop:" << i << ":" << r << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 * r));
        test_timeout_object::instance()->stop_wait();
    }
}
/**
 * 测试test_timeout_object对象
 * 随机等待时间，模拟正式情况测试
 */
static void test9()
{
    s_last = test_get_current_time();
    test_timeout_object::instance()->start();
    std::thread t1(test_timeout_object_start2);
    t1.detach();
    std::thread t2(test_timeout_object_stop2);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    test_timeout_object::instance()->stop();
}

static void test10()
{
    bool ret = false;
    ret = true || true;   //ret = true
    ret = true || false;  //ret = true
    ret = false || false; //ret = false

    ret = true && true;   //ret = true
    ret = true && false;  //ret = false
    ret = false && false; //ret = false

    int a = ret;
}

static void test_timeout_object_start3()
{
    static const int dur[] = {1, 2, 4, 8, 16, 32, 64};
    static const int dur_sz = sizeof(dur) / sizeof(int);
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 3);
    int r = dur[dist(rd) % dur_sz] * 1000;
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "start"
                  << ":" << r << std::endl;
    }
    test_timeout_object::instance()->start_wait(r);
}
static void test_timeout_object_stop3()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 4);
    int r = dist(rd) * 1000;
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "stop"
                  << ":" << r << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
    test_timeout_object::instance()->stop_wait();
}
static void test_timeout_object_stop_and_start3()
{
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 4);
    int r = dist(rd) * 1000;
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "stop"
                  << ":" << r << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(r));
    test_timeout_object::instance()->stop_wait();
    test_timeout_object::instance()->start_wait(r / 2);
}
/**
 * 测试test_timeout_object对象
 * 1.开始等待
 * 2.结束等待
 */
static void test11()
{
    s_last = test_get_current_time();
    test_timeout_object::instance()->start();
    std::thread t1(test_timeout_object_start3);
    t1.detach();
    std::thread t2(test_timeout_object_stop3);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    test_timeout_object::instance()->stop();
}
/**
 * 测试test_timeout_object对象
 * 1.开始等待
 * 2.结束等待，并且再次开始（相当于立马重新开始等待）
 */
static void test12()
{
    s_last = test_get_current_time();
    test_timeout_object::instance()->start();
    std::thread t1(test_timeout_object_start3);
    t1.detach();
    std::thread t2(test_timeout_object_stop_and_start3);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    test_timeout_object::instance()->stop();
}

struct object_timeout_observer_obj
{
    object_timeout_observer_obj()
    {
    }
    ~object_timeout_observer_obj()
    {
    }
    static void work1()
    {
        TIMEOUT_CHECK_OBJECT;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    static void work2()
    {
        TIMEOUT_CHECK_OBJECT_PARAM(500);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    static void work3()
    {
        TIMEOUT_CHECK_OBJECT;
    }
    static void work4()
    {
        TIMEOUT_CHECK_OBJECT_PARAM(500);
    }
};
static void object_timeout_observer_1()
{
    object_timeout_observer::instance()->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    object_timeout_observer_obj::work1();
    object_timeout_observer_obj::work2();
    object_timeout_observer_obj::work3();
    object_timeout_observer_obj::work4();
}
static void test13()
{
    std::thread t(object_timeout_observer_1);
    t.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    object_timeout_observer::instance()->stop();
}

static void test14()
{
    std::cout << test_current_time() << std::endl;
}

class test_network_status
{
private:
    /* data */
public:
    test_network_status(/* args */);
    ~test_network_status();

    static void callback(const network_task_judge::task_t *task);
};

test_network_status::test_network_status(/* args */)
{
}

test_network_status::~test_network_status()
{
}

void test_network_status::callback(const network_task_judge::task_t *task)
{
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":id=" << task->id
              << ":status=" << task->network_status
              << ":tmsp=" << task->task_create_point.count() << std::endl;
}
static int network_task_judge_factor = 1; //1000, 100, 10 //2
static void network_task_judge_add_task()
{
    // std::this_thread::sleep_for(std::chrono::milliseconds(2 * network_task_judge_factor)); //1
    network_task_judge::instance()->add_task("wifi", "");
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * network_task_judge_factor));
    network_task_judge::instance()->add_task("4G", "yidong");
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * network_task_judge_factor));
    network_task_judge::instance()->add_task("wifi", "");
}
static void test15()
{
    /**
     * 测试用例: 
     * 1.//1 语句生效 OK
     * 2.//1 语句不生效 OK
     * 3.//2 1000 OK
     * 4.//2 100 OK
     * 5.//2 10 OK
     * 6.//2 1 不支持
     * 7.//7 OK
     */
    network_task_judge_factor = 1000;
    network_task_judge::instance()->start(4 * network_task_judge_factor, &test_network_status::callback);
    std::thread t(network_task_judge_add_task);
    t.detach();
    // std::this_thread::sleep_for(std::chrono::milliseconds(15 * network_task_judge_factor));//7
    std::this_thread::sleep_for(std::chrono::milliseconds(6 * network_task_judge_factor)); //7
    network_task_judge::instance()->stop();
}
int network_task_judge_count = 1000; //1000 10000 100000 //3
static void network_task_judge_add_task4()
{
    // std::this_thread::sleep_for(std::chrono::milliseconds(2 * network_task_judge_factor));//4
    for (size_t i = 0; i < network_task_judge_count; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2 * network_task_judge_factor));
        if (i % 2)
        {
            network_task_judge::instance()->add_task("wifi", "");
        }
        else
        {
            network_task_judge::instance()->add_task("4G", "yidong");
        }
    }
}
static void test18()
{
    /**
     * 测试用例:
     * 1.//4 语句生效 OK
     * 2.//4 语句不生效 OK
     * 3.//2 1000 OK network_task_judge_factor
     * 4.//2 100 OK
     * 5.//2 10 OK
     * 6.//2 1 不支持
     * 7.//3 1000 OK network_task_judge_count
     * 8.//3 10000 OK
     * 9.//3 100000 OK
     * 10.//4 OK
     * 11.//5 OK
     */
    network_task_judge_factor = 10;
    network_task_judge_count = 1000;
    network_task_judge::instance()->start(4 * network_task_judge_factor, &test_network_status::callback);
    std::thread t(network_task_judge_add_task4);
    t.detach();
    // std::this_thread::sleep_for(std::chrono::milliseconds(network_task_judge_count * 3 * network_task_judge_factor + 4 * network_task_judge_factor * 2));//4
    std::this_thread::sleep_for(std::chrono::milliseconds(15000)); //5
    network_task_judge::instance()->stop();
}

static void test_function_pointer_func()
{
}

static void test_function_pointer()
{
    typedef void(func)();
    typedef void (*func2)();

    func *f1 = test_function_pointer_func;
    f1();
    f1 = nullptr;

    func2 f2 = test_function_pointer_func;
    f2();
    f2 = nullptr;
}
/**
 * 测试函数指针
 */
static void test19()
{
    test_function_pointer();
}

class test_type_traits
{
public:
    int v;
};
template <class _Tp>
struct my_remove_reference
{
    static int __id;
    typedef _Tp type;
};
template <class _Tp>
int my_remove_reference<_Tp>::__id = 0;
template <class _Tp>
struct my_remove_reference<_Tp &>
{
    typedef _Tp type;
};
template <class _Tp>
struct my_remove_reference<_Tp &&>
{
    typedef _Tp type;
};
static void test_type_traits_function()
{
    int a = 3;
    int &ar = a;
    int b = 2;
    int &&arr = 3;
    test_type_traits &&tt = test_type_traits();
    std::remove_reference<test_type_traits &&>::type tt2;
    my_remove_reference<test_type_traits &>::type tt3;
    tt2.v = 1;
}
/**
 * 测试type traits
 */
static void test20()
{
    test_type_traits_function();
}

#include <iostream>

template <int N>
class Sum
{
public:
    enum
    {
        sum = Sum<N - 1>::sum + N
    };
};

template <>
class Sum<0>
{
public:
    enum
    {
        sum = 0
    };
};
/**
 * 递归模板编程
 */
static void test21()
{
    std::cout << "sum of 1 to 100 is: " << Sum<100>::sum << std::endl;
}

class data_object
{
private:
public:
    data_object();
    ~data_object();
};

data_object::data_object(/* args */)
{
    std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

data_object::~data_object()
{
    std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

class data_object_wrapper
{
private:
    data_object _obj;

public:
    data_object_wrapper();
    ~data_object_wrapper();
};

data_object_wrapper::data_object_wrapper(/* args */)
    : _obj()
{
    std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

data_object_wrapper::~data_object_wrapper()
{
    _obj.~data_object(); //影响执行顺序
    std::cout << __FUNCTION__ << ":" << __LINE__ << std::endl;
}

/**
 * 测试类的数据成员的生命周期问题
 */
static void test22()
{
    {
        data_object_wrapper obj;
    }
}

static void test_thread1()
{
    //    simple_thread_pool::thread_t *t = new simple_thread_pool::thread_t();
    //    t = new simple_thread_pool::thread_t(&simple_thread_pool::thread_t::run, t);
    //    t->detach();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //    simple_thread_pool::thread_t *t2 = new simple_thread_pool::thread_t();
    //    simple_thread_pool::thread_t *t22 = t2;
    //    simple_thread_pool::thread_t **tt = &t2;
    //    simple_thread_pool::thread_t *t3 = new simple_thread_pool::thread_t(&simple_thread_pool::thread_t::run, t2);
    //    t3->detach();
    //    // delete t22;
    //    delete *tt;
    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //    delete t3;
    std::cout << __func__ << std::endl;
}
/**
 * 这样使用线程对吗？
 */
static void test23()
{
    test_thread1();
}

static void test_function2()
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
}
static void test_function3(const int ms)
{
    std::cout << __func__ << ":" << __LINE__ << ":ms=" << ms << std::endl;
}
class test_invoke
{
private:
    /* data */
public:
    test_invoke(/* args */);
    ~test_invoke();
    void print()
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    void print2(int v)
    {
        std::cout << __func__ << ":" << __LINE__ << ":ms=" << v << std::endl;
    }
};

test_invoke::test_invoke(/* args */)
{
}

test_invoke::~test_invoke()
{
}

template <class _Fp, class _A0, class... _Args,
          class = std::__enable_if_bullet1<_Fp, _A0>>
inline __attribute__((__visibility__("hidden"))) __attribute__((internal_linkage)) auto
__my_invoke(_Fp &&__f, _A0 &&__a0, _Args &&... __args) noexcept(noexcept((std::__1::forward<_A0>(__a0).*__f)(std::__1::forward<_Args>(__args)...))) -> decltype((std::__1::forward<_A0>(__a0).*__f)(std::__1::forward<_Args>(__args)...))
{
    return (std::__1::forward<_A0>(__a0).*__f)(std::__1::forward<_Args>(__args)...);
}
template <class _Fp, class _A0, class... _Args,
          class = std::__enable_if_bullet1<_Fp, _A0>>
void test_class_memeber_function(_Fp &&__f, _A0 &&__a0, _Args &&... __args)
{
    //    std::cout << __func__ << std::endl;
    (std::__1::forward<_A0>(__a0).*__f)(std::__1::forward<_Args>(__args)...);
}
template <class _Fp, class _A0, class... _Args,
          class = std::__enable_if_bullet1<_Fp, _A0>>
void test_class_memeber_ptr_function(_Fp &&__f, _A0 *&&__a0, _Args &&... __args) noexcept(noexcept((__a0->*__f)(std::__1::forward<_Args>(__args)...)))
{
    //    std::cout << __func__ << std::endl;
    return (__a0->*__f)(std::__1::forward<_Args>(__args)...);
}
template <class _Fp, class _A0, class... _Args,
          class = std::__enable_if_bullet1<_Fp, _A0>>
inline __attribute__((__visibility__("hidden"))) __attribute__((internal_linkage)) auto
__my_invoke_ptr(_Fp &&__f, _A0 *__a0, _Args &&... __args) noexcept(noexcept((__a0->*__f)(std::__1::forward<_Args>(__args)...))) -> decltype((__a0->*__f)(std::__1::forward<_Args>(__args)...))
{
    return (__a0->*__f)(std::__1::forward<_Args>(__args)...);
}
/**
 * 源码测试
 */
static void test24()
{
    test_invoke obj;
    __my_invoke(&test_invoke::print, obj);
    __my_invoke(&test_invoke::print2, obj, 3);

    test_class_memeber_function(&test_invoke::print, obj);
    test_class_memeber_ptr_function(&test_invoke::print, &obj);
    test_class_memeber_ptr_function(&test_invoke::print2, &obj, 4);

    test_invoke *pobj = &obj;
    __my_invoke_ptr(&test_invoke::print, pobj);
    __my_invoke_ptr(&test_invoke::print2, pobj, 5);
}

class test_is_array_class
{
};

/**
 * 测试是否是数组
 */
static void test25()
{
    std::cout << std::boolalpha;
    std::cout << std::is_array<test_is_array_class>::value << '\n';
    std::cout << std::is_array<test_is_array_class[]>::value << '\n';
    std::cout << std::is_array<test_is_array_class *>::value << '\n'; //false
    std::cout << std::is_array<test_is_array_class[3]>::value << '\n';
    std::cout << std::is_array<float>::value << '\n';
    std::cout << std::is_array<int>::value << '\n';
    std::cout << std::is_array<int[]>::value << '\n';
    std::cout << std::is_array<int[3]>::value << '\n';
}

struct test_is_function_struct
{
    int fun() const &;
    static int fun2();
};

template <typename>
struct PM_traits
{
};

template <class T, class U>
struct PM_traits<U T::*>
{
    using member_type = U;
};

int test_is_function_function();

static void test26()
{
    std::cout << std::boolalpha;
    std::cout << std::is_function<test_is_function_struct>::value << '\n';
    std::cout << std::is_function<int(int)>::value << '\n';
    std::cout << std::is_function<decltype(test_is_function_function)>::value << '\n';
    std::cout << std::is_function<int>::value << '\n';

    using T = PM_traits<decltype(&test_is_function_struct::fun)>::member_type; // T 为 int() const&
    std::cout << std::is_function<T>::value << '\n';
    using T2 = decltype(&test_is_function_struct::fun);
    std::cout << std::is_function<T2>::value << '\n';
    using T3 = decltype(&test_is_function_struct::fun2);
    std::cout << std::is_function<T3>::value << '\n';
}

class test_type_parse_traits
{
private:
    /* data */
public:
    test_type_parse_traits(/* args */);
    ~test_type_parse_traits();
};

test_type_parse_traits::test_type_parse_traits(/* args */)
{
}

test_type_parse_traits::~test_type_parse_traits()
{
}

template <typename T>
struct TypeParseTraits
{
};

#define REGISTER_PARSE_TYPE(X)   \
    template <>                  \
    struct TypeParseTraits<X>    \
    {                            \
        static const char *name; \
        typedef X type;          \
    };                           \
    const char *TypeParseTraits<X>::name = #X

REGISTER_PARSE_TYPE(int);
REGISTER_PARSE_TYPE(double);
REGISTER_PARSE_TYPE(test_type_parse_traits);

/**
 * 获取模板参数类型
 */
static void test27()
{
    //ref: https://stackoverflow.com/questions/1055452/c-get-name-of-type-in-template
    //ref: https://en.cppreference.com/w/cpp/types/type_info/name
    //ref: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#typeid
    //ref: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-type

    std::cout << __func__ << ":" << __LINE__ << ":name=" << TypeParseTraits<int>::name << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << typeid(TypeParseTraits<int>::type).name() << std::endl;

    std::cout << __func__ << ":" << __LINE__ << ":name=" << TypeParseTraits<test_type_parse_traits>::name << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << typeid(TypeParseTraits<test_type_parse_traits>::type).name() << std::endl;
}

#include <cxxabi.h>

template <typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if (status == 0)
    {
        tname = demangled_name;
        std::free(demangled_name);
    }
    return tname;
}

/**
 * gcc or clang typeid().name() is not human readable
 * 在mac上获取类型名称信息
 */
static void test28()
{
    std::cout << __func__ << ":" << __LINE__ << ":name=" << type_name<int>().c_str() << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << type_name<test_type_parse_traits>().c_str() << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << type_name<std::decay<test_type_parse_traits>::type>().c_str() << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << type_name<std::decay<test_type_parse_traits *>::type>().c_str() << std::endl;
    std::cout << __func__ << ":" << __LINE__ << ":name=" << type_name<std::decay<test_type_parse_traits &&>::type>().c_str() << std::endl;
}

class work_object
{
private:
    static std::random_device rd;
    static std::uniform_int_distribution<int> dist;

public:
    work_object(/* args */);
    ~work_object();
    void operator()()
    {
        std::cout << __func__ << ":" << __LINE__ << ":tid=" << std::this_thread::get_id() << std::endl;
    }
    void work(void *)
    {
        std::cout << __func__ << ":" << __LINE__ << ":tid=" << std::this_thread::get_id() << std::endl;
    }
    static void work2(void *)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << std::endl;
    }
    static void work3(int i, void *)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << "i=" << i
                  << std::endl;
    }
    static void work4(int i, int j, void *)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << "i=" << i
                  << std::endl;
    }
};

std::random_device work_object::rd;
std::uniform_int_distribution<int> work_object::dist(1, 1000);

work_object::work_object(/* args */)
{
}

work_object::~work_object()
{
}

typedef void (*work_object_memeber_function_t)(int, void *);
typedef void (*work_object_memeber_function3_t)(int, int, void *);
typedef void (*work_object_memeber_function2_t)(void *);
static void test_add_thread_pool_task()
{
    int count = 100;
    work_object *wobjs[count];
    for (size_t i = 0; i < count; i++)
    {
        if (simple_thread_pool::task_queue_strategy_t::task_queue_strategy_fix_param == simple_thread_pool::instance()->get_parameter_strategy())
        {
            simple_thread_pool::instance()->post(&work_object::work2, nullptr);
        }
        else if (simple_thread_pool::task_queue_strategy_t::task_queue_strategy_variable_param == simple_thread_pool::instance()->get_parameter_strategy())
        {
            simple_thread_pool::instance()->postv<work_object_memeber_function_t, int, void *>(0, &work_object::work3, i, nullptr);
            // simple_thread_pool::instance()->postv<work_object_memeber_function3_t, int, int, void *>(0, &work_object::work4, i, 2*i, nullptr);//参数不一致 warning
        }
    }
    for (size_t i = 0; i < count; i++)
    {
    }
}
static void test_add_thread_pool_task2()
{
    int count = 1;
    work_object *wobjs[count];
    for (size_t i = 0; i < count; i++)
    {
        if (simple_thread_pool::task_queue_strategy_t::task_queue_strategy_fix_param == simple_thread_pool::instance()->get_parameter_strategy())
        {
            simple_thread_pool::instance()->post(&work_object::work2, nullptr);
        }
        else if (simple_thread_pool::task_queue_strategy_t::task_queue_strategy_variable_param == simple_thread_pool::instance()->get_parameter_strategy())
        {
            // simple_thread_pool::instance()->postv<work_object_memeber_function2_t, void *>(0, &work_object::work2, nullptr);//compile error
            // simple_thread_pool::instance()->postv<simple_thread_pool::task_func_t, void *>(0, &work_object::work2, nullptr);//compile error
            // simple_thread_pool::instance()->postv<work_object_memeber_function2_t, void *>(0, &work_object::work2, nullptr);
            simple_thread_pool::instance()->postv<work_object_memeber_function_t, int, void *>(0, &work_object::work3, i, nullptr);
        }
    }
    for (size_t i = 0; i < count; i++)
    {
    }
}
static void test_thread_pool_1()
{
    simple_thread_pool::instance()->init(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    test_add_thread_pool_task();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    simple_thread_pool::instance()->uninit();
    simple_thread_pool::uninstance();
}
static void test_thread_pool_2()
{
    simple_thread_pool::instance()->initv<work_object_memeber_function_t, int, void *>(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    test_add_thread_pool_task();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    simple_thread_pool::instance()->uninit();
    simple_thread_pool::uninstance();
}
static void test_thread_pool_3()
{
    /**
     * 测试静态模板编译错误判断
     */
    simple_thread_pool::instance()->initv<work_object_memeber_function2_t, void *>(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    test_add_thread_pool_task2();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    simple_thread_pool::instance()->uninit();
    simple_thread_pool::uninstance();
}
/**
 * 简单测试线程池
 */
static void test29()
{
    s_last = test_get_current_time();
    test_thread_pool_2();
}

class test_condition_mutex
{
private:
    std::mutex _mutex;
    std::condition_variable _variable;

public:
    std::atomic_bool _quit;
    test_condition_mutex(/* args */);
    ~test_condition_mutex();
    void work1()
    {
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << std::endl;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _variable.wait(lock, [this]() -> bool {
                return _quit;
            });
        }
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << std::endl;
    }
};

test_condition_mutex::test_condition_mutex(/* args */)
{
}

test_condition_mutex::~test_condition_mutex()
{
}

static void test30()
{
    test_condition_mutex obj;
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    std::thread t1(&test_condition_mutex::work1, &obj);
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    t1.join();
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
}
static void test31()
{
    test_condition_mutex obj;
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    simple_thread_pool::thread_t t1(&test_condition_mutex::work1, &obj);
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    t1.join();
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
}

static void test_join_1(test_condition_mutex *obj)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    delete obj;
}
static void test_thread_2()
{
    test_condition_mutex *obj = new test_condition_mutex();
    simple_thread_pool::thread_t t(test_join_1, obj);
    t.detach();
}
/**
 * 线程对象会先运行体释放，会导致问题吗？
 * 如果使用该线程对象就会导致未知行为
 * 如果仅仅在结构体里面，应该没有问题
 */
static void test32()
{
    test_thread_2();
}

static void test_thread_function()
{
}
std::thread *tb = nullptr;
static void test_thread_A()
{
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    tb = new std::thread(test_thread_function);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
}
static void test_thread_C()
{
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
    tb->join();
    delete tb;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << __FUNCTION__ << ":" << __LINE__
              << ":t=" << test_current_time()
              << ":els=" << test_get_time_difference()
              << ":tid=" << std::this_thread::get_id()
              << std::endl;
}
/**
 * 在线程A创建线程B
 * 在线程C销毁线程join B
 */
static void test33()
{
    if (true)
    {
        std::thread t1(test_thread_A);
        t1.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::thread t2(test_thread_C);
        t2.detach();
    }
    if (false)
    {
        std::thread t1(test_thread_A);
        t1.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        test_thread_C();
    }
}

static void test_map()
{
    std::map<std::pair<int, int>, std::string> list;
    list.insert(std::make_pair(std::make_pair(-1, 1), "-11"));
    list.insert(std::make_pair(std::make_pair(0, 1), "01"));
    list.insert(std::make_pair(std::make_pair(1, 1), "11"));
    list.insert(std::make_pair(std::make_pair(1, 2), "12"));
    list.insert(std::make_pair(std::make_pair(2, 2), "22"));
    for (auto iter = list.begin(); iter != list.end(); ++iter)
    {
        std::cout << __FUNCTION__ << ":" << __LINE__
                  << ":t=" << test_current_time()
                  << ":els=" << test_get_time_difference()
                  << ":tid=" << std::this_thread::get_id()
                  << ":key1=" << iter->first.first
                  << ":key2=" << iter->first.second
                  << ":v=" << iter->second
                  << std::endl;
    }
}
/**
 * 测试map排序
 */
static void test34()
{
    test_map();
}

class BaseClass
{
public:
    virtual void printf() {}
};
class DerivedClass : public BaseClass
{
public:
    void printf() override {}
};

class A
{
};
class B : public A
{
};
class C
{
};
class D
{
public:
    operator C()
    {
        return c;
    }
    C c;
};
/**
 * 测试类型是否相同
 */
static void test35()
{
    std::cout << std::boolalpha;
    std::cout << std::is_same<simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t>, simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t>>::value << std::endl;
    std::cout << std::is_same<simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t>, simple_thread_pool::task_t<work_object_memeber_function_t, int, void *>>::value << std::endl;

    std::cout << "1:hash_code:" << typeid(simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t>).hash_code() << std::endl;
    std::cout << "2:hash_code:" << typeid(simple_thread_pool::task_t<work_object_memeber_function_t, int, void *>).hash_code() << std::endl;

    //    simple_thread_pool::task_base_t *t1 = new simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t>(&work_object::work2, nullptr);
    simple_thread_pool::task_base_t *t2 = new simple_thread_pool::task_t<work_object_memeber_function_t, int, void *>(0, &work_object::work3, 2, nullptr);

    //    std::cout << "t1:hash_code:" << typeid(t1).hash_code() << std::endl;
    std::cout << "t2:hash_code:" << typeid(t2).hash_code() << std::endl;

    //    std::cout << "t1:name:" << typeid(t1).name() << std::endl;
    std::cout << "t2:name:" << typeid(t2).name() << std::endl;

    std::cout << std::is_convertible<simple_thread_pool::task_base_t *, simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *>::value << std::endl;
    std::cout << std::is_convertible<simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *, simple_thread_pool::task_base_t *>::value << std::endl;

    std::cout << "-----" << std::endl;

    std::cout << std::is_convertible<simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *, simple_thread_pool::task_base_t *>::value << std::endl;
    std::cout << std::is_convertible<simple_thread_pool::task_base_t *, simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *>::value << std::endl;

    std::cout << "-----" << std::endl;

    std::cout << std::is_convertible<BaseClass, DerivedClass>::value << std::endl;
    std::cout << std::is_convertible<BaseClass *, DerivedClass *>::value << std::endl;
    std::cout << std::is_convertible<DerivedClass *, BaseClass *>::value << std::endl;

    D d;
    d.operator C();
    C c = D();

    std::cout << "-----" << std::endl;

    //    simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *param_2_ret = dynamic_cast<simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *>(t1);
    simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *param_v_ret = dynamic_cast<simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *>(t2);

    //    std::cout << "param_2_ret:" << (param_2_ret == nullptr) << std::endl;
    std::cout << "param_v_ret:" << (param_v_ret == nullptr) << std::endl;

    std::cout << "-----" << std::endl;

    simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *param_2_ret2 = dynamic_cast<simple_thread_pool::task_t<simple_thread_pool::task_func_t, simple_thread_pool::task_param_t> *>(t2);
    //    simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *param_v_ret2 = dynamic_cast<simple_thread_pool::task_t<work_object_memeber_function_t, int, void *> *>(t1);

    std::cout << "param_2_ret:" << (param_2_ret2 == nullptr) << std::endl;
    //    std::cout << "param_v_ret:" << (param_v_ret2 == nullptr) << std::endl;
}

static void test_template_callback(int v1, double v2, std::string v3)
{
}
template <class _Fp, class... _Args>
static void test_template_function(const std::tuple<_Fp, _Args...> &t)
{
    typedef typename std::__make_tuple_indices<std::tuple_size<std::tuple<_Fp, _Args...>>::value, 1>::type _Index;
    std::tuple<_Fp, _Args...> params = t;
    //    std::function_call<_Fp, _Args..., _Index...>(params, _Index());
}
/**
 * 测试类型
 */
static void test36()
{
    test_template_function(std::make_tuple(&test_template_callback, 1, 3.14, "haha"));
}

void test_function4(int a, double b, int *c)
{
    std::cout << a << ":" << b << ":" << c << std::endl;
}
template <typename Function, typename Tuple, size_t... I>
static auto test_call_function(Function f, Tuple t, std::index_sequence<I...>)
{
    return f(std::get<I>(t)...);
}
template <typename Function, typename Tuple>
static auto test_call_function(Function f, Tuple t)
{
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return test_call_function(f, t, std::make_index_sequence<size>{});
}
template <class _Fp, class... _Args, size_t... _Indices>
static void test_call_function2(_Fp &f, std::index_sequence<_Indices...>, _Args &... g)
{
    f(std::get<_Indices>(g)...);
}
template <class _Fp, class _Args, size_t... _Indices>
static void test_call_function3(_Fp &&f, _Args &&t, std::index_sequence<_Indices...>)
{
    f(std::get<_Indices>(t)...);
}
static void test37()
{
    // ref: https://cpluspluspedia.com/en/tutorial/8315/std-integer-sequence
    // ref: https://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
    int a = 8;
    std::tuple<int, double, int *> t = std::make_tuple(1, 3.14, &a);
    //or std::array<int, 3> t;
    //or std::pair<int, double> t;
    test_call_function(&test_function4, t);

    // test_call_function2(&test_function4, std::make_index_sequence<3>{}, 1, 3.14, (void *)&a);
    auto ss = std::make_index_sequence<std::tuple_size<std::tuple<int, double, int *>>::value>();
    auto ss2 = std::make_index_sequence<std::tuple_size<std::tuple<int, double, int *>>::value>{};
    // test_call_function3(&test_function4, t, ss2);
    test_call_function(&test_function4, t, ss2);
    test_call_function3(&test_function4, t, ss2);
}

template <typename T, T... ints>
static void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    // ref: https://en.cppreference.com/w/cpp/utility/integer_sequence
    // c++ 17 above
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    //    ((std::cout << ints << ' '),...);
    //    std::cout << ints[0] << std::endl;
    // for (int i = 0; i < int_seq.size(); ++i)
    // {
    //        std::cout << int_seq[i] << ' ';
    // }
    // std::cout << '\n';
    // std::integer_sequence<size_t, 1, 2, 4, 8> seq;
    //    std::cout << seq[0] << std::endl;
    //    for (auto i: seq) {
    //        std::cout << i << std::endl;
    //    }
    int int_array[] = {ints...};
    for (auto i : int_array)
    {
        std::cout << i << std::endl;
    }
}

template <size_t... ints>
static void print_sequence2(std::__tuple_indices<ints...> int_seq)
{
    // ref: https://en.cppreference.com/w/cpp/utility/integer_sequence
    // c++ 17 above
    std::cout << "The sequence of size x: ";
    int int_array[] = {(ints)...};
    for (auto i : int_array)
    {
        std::cout << i << std::endl;
    }
    // std::cout << "The sequence of size x2: ";
    // int int_array2[] = {(ints + 1)...};
    // for (auto i : int_array2)
    // {
    //     std::cout << i << std::endl;
    // }
}

template <class _TSp, class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute(std::tuple<_TSp, _Fp, _Args...> &__t, std::__tuple_indices<_Indices...>)
{
    std::__invoke(std::move(std::get<1>(__t)), std::move(std::get<_Indices>(__t))...);
}

template <class _Fp>
static void *my_thread_proxy(void *__vp)
{
    // _Fp = std::tuple< unique_ptr<__thread_struct>, Functor, Args...>
    std::unique_ptr<_Fp> __p(static_cast<_Fp *>(__vp));
    std::__thread_local_data().set_pointer(std::get<0>(*__p).release());
    typedef typename std::__make_tuple_indices<std::tuple_size<_Fp>::value, 2>::type _Index;
    my_thread_execute(*__p, _Index());
    return nullptr;
}

template <class _Fp, class... _Args>
static void my_call_function(_Fp &&__f, _Args &&... __args)
{
    /**
     * from thread modal
     */
    typedef std::unique_ptr<std::__thread_struct> _TSPtr;
    _TSPtr __tsp(new std::__thread_struct);
    typedef std::tuple<_TSPtr, typename std::decay<_Fp>::type, typename std::decay<_Args>::type...> _Gp;
    std::unique_ptr<_Gp> __p(
        new _Gp(std::move(__tsp),
                std::decay_copy(std::forward<_Fp>(__f)),
                std::decay_copy(std::forward<_Args>(__args))...));
    my_thread_proxy<_Gp>(__p.get());
    // int __ec = __libcpp_thread_create(&__t_, &__thread_proxy<_Gp>, __p.get());
    // if (__ec == 0)
    __p.release();
    // else
    //     __throw_system_error(__ec, "thread constructor failed");
}

template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute2(std::tuple<_Fp, _Args...> &__t, std::__tuple_indices<_Indices...>)
{
    std::__invoke(std::move(std::get<0>(__t)), std::move(std::get<_Indices>(__t))...);
}
template <class _Fp>
static void *my_thread_proxy2(void *__vp)
{
    // _Fp = std::tuple< unique_ptr<__thread_struct>, Functor, Args...>
    std::unique_ptr<_Fp> __p(static_cast<_Fp *>(__vp));
    typedef typename std::__make_tuple_indices<std::tuple_size<_Fp>::value, 1>::type _Index;
    my_thread_execute2(*__p, _Index());
    return nullptr;
}
template <class _Fp, class... _Args>
static void my_call_function2(_Fp &&__f, _Args &&... __args)
{
    typedef std::tuple<typename std::decay<_Fp>::type, typename std::decay<_Args>::type...> _Gp;
    std::unique_ptr<_Gp> __p(new _Gp(std::decay_copy(std::forward<_Fp>(__f)), std::decay_copy(std::forward<_Args>(__args))...));
    my_thread_proxy2<_Gp>(__p.get());
    __p.release();
}

template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute3(std::tuple<_Fp, _Args...> &__t, std::__tuple_indices<_Indices...>)
{
    std::move(std::get<0>(__t))(std::move(std::get<_Indices>(__t))...);
}
template <class _Fp>
static void *my_thread_proxy3(_Fp &&__v)
{
    typedef typename std::__make_tuple_indices<std::tuple_size<_Fp>::value, 1>::type _Index;
    auto ret = std::make_index_sequence<std::tuple_size<_Fp>::value>();
    //    print_sequence(ret);
    //    print_sequence2(_Index());
    my_thread_execute3(__v, _Index());
    //    print_sequence(std::index_sequence_for<float, std::iostream, char, std::string, void *, long long int>{});
    return nullptr;
}
template <class _Fp, class... _Args>
static void my_call_function3(_Fp &&__f, _Args &&... __args)
{
    my_thread_proxy3(std::make_tuple(std::move(__f), std::move(__args)...));
}

template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute4(std::tuple<_Fp, _Args...> &__t, std::__tuple_indices<_Indices...>)
{
    std::move(std::get<0>(__t))(std::move(std::get<_Indices>(__t))...);
}
template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute44(std::tuple<_Fp, _Args...> &__t, std::__tuple_indices<_Indices...>)
{
    auto ret = std::__tuple_indices<(_Indices + 1)...>();
    print_sequence2(ret);
    my_thread_execute4(__t, ret);
}
template <class _Fp>
static void *my_thread_proxy4(_Fp &&__v)
{
    typedef typename std::__make_tuple_indices<std::tuple_size<_Fp>::value - 1, 0>::type _Index;
    print_sequence2(_Index());
    my_thread_execute44(__v, _Index());
    return nullptr;
}
template <class _Fp, class... _Args>
static void my_call_function4(_Fp &&__f, _Args &&... __args)
{
    my_thread_proxy4(std::make_tuple(std::move(__f), std::move(__args)...));
}

template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute5(std::tuple<_Fp, _Args...> &__t, std::index_sequence<_Indices...>)
{
    std::move(std::get<0>(__t))(std::move(std::get<_Indices>(__t))...);
}
template <class _Fp, class... _Args, size_t... _Indices>
static void my_thread_execute55(std::tuple<_Fp, _Args...> &__t, std::index_sequence<_Indices...>)
{
    auto ret = std::index_sequence<(_Indices + 1)...>(); // +1
    print_sequence(ret);
    my_thread_execute5(__t, ret);
}
template <class _Fp>
static void *my_thread_proxy5(_Fp &&__v)
{
    auto ret = std::make_index_sequence<std::tuple_size<_Fp>::value - 1>(); // -1
    print_sequence(ret);
    my_thread_execute55(__v, ret);
    return nullptr;
}
template <class _Fp, class... _Args>
static void my_call_function5(_Fp &&__f, _Args &&... __args)
{
    my_thread_proxy5(std::make_tuple(std::move(__f), std::move(__args)...));
}

/**
 * 实现变参函数调用 支持C++14
 * 如果是C++11，需要自己写index_sequence相关实现
 * invoke/index_sequence都是C++14新特性
 */
static void test38()
{
    /**
     * 顺藤摸瓜方法
     */
    int a = 8;
    my_call_function(&test_function4, 1, 3.14, &a);
    my_call_function2(&test_function4, 1, 3.14, &a);
    my_call_function3(&test_function4, 1, 3.14, &a);
    my_call_function4(&test_function4, 1, 3.14, &a);
    my_call_function5(&test_function4, 1, 3.14, &a);
}
