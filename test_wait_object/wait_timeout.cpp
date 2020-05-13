//
//  wait_timeout.cpp
//  CLIMSDK_ios
//
//  Created by zuoyu on 2020/4/29.
//  Copyright © 2020 yu.zuo. All rights reserved.
//

#include "wait_timeout.h"
#include <chrono>
#include <thread>
#include <iostream>

static void test1();
static void test2();
static void test3();
static void test4();
static void test5();
static void test6();
static void test7();
void test_wait_timeout()
{
    test7();
}

namespace im {
namespace citylife {
namespace sdk {
namespace core{

static int _s_id = 0;

static std::mutex _s_mutex;

wait::wait(int timeout)
    : _timeout(timeout)
    , _quit(false)
{
    _id = ++_s_id;
    if (_s_id > 65535) {
        _id = _s_id = 1;
    }
}

wait::~wait()
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << ":" << _id << std::endl;
    }
}

void wait::start_wait()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _variable.wait_for(lock, std::chrono::milliseconds(_timeout), [this]() ->bool {
            return this->_quit;
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (!_quit.load()) {
        _quit.store(true);
    }
}

void wait::cancel_wait()
{
    _quit.store(true);
    _variable.notify_one();
}

void wait::delete_later()
{
    
}

int wait::get_id()
{
    return _id;
}


wait_manager wait_manager::_g;

wait_manager* wait_manager::instance()
{
    return &_g;
}

void wait_manager::add_wait(const test_shared_ptr<wait> obj_ptr)
{
    _list.insert(std::make_pair(obj_ptr->get_id(), obj_ptr));
    obj_ptr->start_wait();
}
void wait_manager::add_wait2(const zy::my_shared_ptr<wait> obj_ptr)
{
    _list2.insert(std::make_pair(obj_ptr->get_id(), obj_ptr));
    obj_ptr->start_wait();
}
void wait_manager::add_wait3(const zy2::simple_shared_ptr<wait> obj_ptr)
{
    _list3.insert(std::make_pair(obj_ptr->get_id(), obj_ptr));
    obj_ptr->start_wait();
}
void wait_manager::del_wait(const test_shared_ptr<wait> obj_ptr)
{
    obj_ptr->cancel_wait();
    _list.erase(obj_ptr->get_id());
    _list2.erase(obj_ptr->get_id());
    _list3.erase(obj_ptr->get_id());
}
void wait_manager::clear()
{
    for (auto iter = _list.begin(); iter != _list.end(); ++iter) {
        iter->second->cancel_wait();
    }
    _list.clear();
    for (auto iter = _list2.begin(); iter != _list2.end(); ++iter) {
        iter->second->cancel_wait();
    }
    _list2.clear();
    for (auto iter = _list3.begin(); iter != _list3.end(); ++iter) {
        iter->second->cancel_wait();
    }
    _list3.clear();
}


wait_manager2 wait_manager2::_g;

wait_manager2* wait_manager2::instance()
{
    return &_g;
}

void wait_manager2::add_wait(im::citylife::sdk::core::wait* obj_ptr)
{
    _list.insert(std::make_pair(obj_ptr->get_id(), obj_ptr));
    obj_ptr->start_wait();
}
void wait_manager2::del_wait(im::citylife::sdk::core::wait* obj_ptr)
{
    obj_ptr->cancel_wait();
    _list.erase(obj_ptr->get_id());
    delete obj_ptr;
}
void wait_manager2::clear()
{
    for (auto iter = _list.begin(); iter != _list.end(); ++iter) {
        iter->second->cancel_wait();
        delete iter->second;
    }
    _list.clear();
}



timeout_obj timeout_obj::_obj;
timeout_obj *timeout_obj::instance()
{
    return &_obj;
}

timeout_obj::timeout_obj()
    : _quit(false)
{
}
timeout_obj::~timeout_obj()
{
    this->cancel();
}
void timeout_obj::start(int ms)
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << ":before" << std::endl;
    }
    
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _nb = true;
        _variable.wait_for(lock, std::chrono::milliseconds(ms), [this]() -> bool {
            return this->_quit;
        });
        _nb = false;
    }
    
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << ":after" << std::endl;
    }
    
    if (!_quit.load())
    {
        _quit.store(true);
    }
}
void timeout_obj::restart(int ms)
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    this->_cancel_with_wait();
    this->reset();
    this->start(ms);
}
void timeout_obj::cancel()
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    _quit.store(true);
    _variable.notify_one();
}
void timeout_obj::reset()
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    _quit.store(false);
}
void timeout_obj::_cancel_with_wait()
{
    {
        std::unique_lock<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    _quit.store(true);
    _variable.notify_one();
    while (_nb) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


}
}
}
}

//static std::mutex _s_mutex;

void thread_work_task_start(int tid)
{
    using namespace im::citylife::sdk::core;
    {
        std::lock_guard<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << ":" << tid << std::endl;
    }
    
//    wait_manager::instance()->add_wait(test_make_shared<im::citylife::sdk::core::wait>(tid * 1000));
//    wait_manager::instance()->add_wait2(zy::my_make_shared<im::citylife::sdk::core::wait>(tid * 1000));
    wait_manager::instance()->add_wait3(zy2::make_simple_shared<im::citylife::sdk::core::wait>(tid * 1000));
}
void thread_work_task_cancel()
{
    using namespace im::citylife::sdk::core;
    {
        std::lock_guard<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    
    wait_manager::instance()->clear();
}

static void test1()
{
    for (int i = 0; i < 10; ++i) {
        std::thread t(thread_work_task_start, i);
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::thread t(thread_work_task_cancel);
    t.join();
//    std::thread* arr[10];
//    for (int i = 0; i < 10; ++i) {
//        std::thread* t = new std::thread(thread_work_task_cancel, i);
//        arr[i] = t;
//    }
//    for (int i = 0; i < 10; ++i) {
//        std::thread* t = arr[i];
//        t->join();
//    }

}

static void test2()
{
    for (int i = 0; i < 1; ++i) {
        std::thread t(thread_work_task_start, i * 5);
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread t(thread_work_task_cancel);
    t.join();

}

static void test5()
{
    for (int i = 1; i <= 1; ++i) {
        std::thread t(thread_work_task_start, i * 5);
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread t(thread_work_task_cancel);
    t.join();

}


void thread_work_task_start2(int tid)
{
    using namespace im::citylife::sdk::core;
    {
        std::lock_guard<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << ":" << tid << std::endl;
    }
    
    wait_manager2::instance()->add_wait(new im::citylife::sdk::core::wait(tid * 1000));
}
void thread_work_task_cancel2()
{
    using namespace im::citylife::sdk::core;
    {
        std::lock_guard<std::mutex> lock(_s_mutex);
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    
    wait_manager2::instance()->clear();
}

static void test3()
{
    for (int i = 0; i < 1; ++i) {
        std::thread t(thread_work_task_start2, i * 5);
        t.detach();
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::thread t(thread_work_task_cancel2);
    t.join();

}


static void test4()
{
    using namespace im::citylife::sdk::core;
    {
        std::shared_ptr<im::citylife::sdk::core::wait> ptr1 = std::make_shared<im::citylife::sdk::core::wait>(1);
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::shared_ptr<im::citylife::sdk::core::wait> ptr2 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::shared_ptr<im::citylife::sdk::core::wait> ptr3;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
        ptr3 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
    }
    
    
    std::cout << "-------------" << std::endl;
    
    {
        zy::my_shared_ptr<im::citylife::sdk::core::wait> ptr1 = zy::my_make_shared<im::citylife::sdk::core::wait>(1);
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        zy::my_shared_ptr<im::citylife::sdk::core::wait> ptr2 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        zy::my_shared_ptr<im::citylife::sdk::core::wait> ptr3;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
        ptr3 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
    }
    
    std::cout << "-------------" << std::endl;
    
    {
        zy2::simple_shared_ptr<im::citylife::sdk::core::wait> ptr1 = zy2::make_simple_shared<im::citylife::sdk::core::wait>(1);
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        zy2::simple_shared_ptr<im::citylife::sdk::core::wait> ptr2 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        zy2::simple_shared_ptr<im::citylife::sdk::core::wait> ptr3;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
        ptr3 = ptr1;
        std::cout << "ptr1:" << ptr1.use_count() << std::endl;
        std::cout << "ptr3:" << ptr3.use_count() << std::endl;
    }
    
    
    std::cout << "-------------" << std::endl;
}

static void _timeout_obj_cancel(int ms)
{
    using namespace im::citylife::sdk::core;
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    timeout_obj::instance()->cancel();
}
static void _timeout_obj_start(int ms)
{
    using namespace im::citylife::sdk::core;
    timeout_obj::instance()->reset();
    timeout_obj::instance()->start(ms);
}
static void _timeout_obj_restart(int ms)
{
    using namespace im::citylife::sdk::core;
    timeout_obj::instance()->restart(ms);
}
static void test6()
{
    std::thread t1(_timeout_obj_cancel, 0);
    t1.detach();
    std::thread t2(_timeout_obj_start, 5000);
    t2.detach();
    std::thread t3(_timeout_obj_cancel, 2000);
    t3.detach();
}
static void test7()
{
    std::thread t1(_timeout_obj_start, 6000);
    t1.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::thread t2(_timeout_obj_restart, 2000);
    t2.detach();
}
