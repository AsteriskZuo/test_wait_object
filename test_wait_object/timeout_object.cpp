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

static std::mutex s_mutex;

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
void test_timeout_object_function()
{
    test13();
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

object_timeout_observer::timeout_object_t::timeout_object_t() noexcept
{
    timeout_object_t(0);
}
object_timeout_observer::timeout_object_t::timeout_object_t(const int ms) noexcept
{
    if (0xff <= (id = _timeout_object_t_id.fetch_sub(1)))
        id = _timeout_object_t_id.exchange(0);
    object_timeout_observer::instance()->add_object(std::addressof(*this), __FUNCTION__, ms);
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
    : _quit(false), _threshold(std::chrono::milliseconds(20))
{
}

object_timeout_observer::~object_timeout_observer()
{
}

void object_timeout_observer::start()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_quit.exchange(true))
    {
        if (!_t)
        {
            _t = new std::thread(&object_timeout_observer::run);
        }
    }
}
void object_timeout_observer::stop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_quit.exchange(false))
    {
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

void object_timeout_observer::add_object(timeout_object_t *obj, const std::string obj_name)
{
    add_object(obj, obj_name, 0);
}
void object_timeout_observer::add_object(timeout_object_t *obj, const std::string obj_name, const int ms)
{
    std::shared_ptr<timeout_object_info_t> obj_info_ptr = std::make_shared<timeout_object_info_t>();
    obj_info_ptr->obj = obj;
    obj_info_ptr->start_point = std::chrono::system_clock::now();
    obj_info_ptr->end_point = std::chrono::time_point<std::chrono::system_clock>();
    obj_info_ptr->obj_name = std::move(obj_name);
    obj_info_ptr->_threshold = std::chrono::milliseconds(ms);
    _ls.insert(std::make_pair(obj->id, obj_info_ptr));
}
void object_timeout_observer::delete_object(const int id)
{
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
                      << ":ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(ret).count() << std::endl;
        }
        else
        {
            //在规定时间
        }
        _ls.erase(iter);
    }
}
void object_timeout_observer::check_timeout()
{
    auto end_point = std::chrono::system_clock::now();
    for (auto iter = _ls.begin(); iter != _ls.end();)
    {
        const std::shared_ptr<timeout_object_info_t> &obj_info_ptr = iter->second;
        auto ret = end_point - obj_info_ptr->start_point;
        if (ret >= (obj_info_ptr->_threshold.count() ? obj_info_ptr->_threshold : _threshold))
        {
            //超时
            std::cout << __FUNCTION__ << ":" << __LINE__
                      << ":name=" << obj_info_ptr->obj_name.c_str()
                      << ":ms=" << std::chrono::duration_cast<std::chrono::milliseconds>(ret).count() << std::endl;
            iter = _ls.erase(iter);
        }
        else
        {
            //在规定时间
            ++iter;
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
        if (0 != obj->_ls.size())
            obj->check_timeout();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

#define TIMEOUT_CHECK_OBJECT object_timeout_observer::timeout_object_t();
#define TIMEOUT_CHECK_OBJECT_PARAM(x) object_timeout_observer::timeout_object_t(x);

///////////////////////////////////////////////////////////////////////////////
////  test partition                                                       ////
///////////////////////////////////////////////////////////////////////////////

/**
 * 返回当前时间字符串
 */
static const char *test_current_time()
{
    if (true)
    {
        static char now_buffer[9] = {0};
        std::time_t now_time = std::time(NULL);
        std::tm *now_tm = localtime(&now_time);
        std::memset(now_buffer, 0, 9);
        std::strftime(now_buffer, 8, "%H:%M:%S", now_tm);
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
    object_timeout_observer_obj(){

    }
    ~object_timeout_observer_obj(){

    }
    static void work1(){
        TIMEOUT_CHECK_OBJECT;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    static void work2(){
        TIMEOUT_CHECK_OBJECT_PARAM(500);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};
static void object_timeout_observer_1()
{
    object_timeout_observer::instance()->start();
    object_timeout_observer_obj::work1();
    object_timeout_observer_obj::work2();
}
static void test13()
{
    std::thread t(object_timeout_observer_1);
    t.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    object_timeout_observer::instance()->stop();
}
