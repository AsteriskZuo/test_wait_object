//
//  wait_timeout.h
//  CLIMSDK_ios
//
//  Created by zuoyu on 2020/4/29.
//  Copyright © 2020 yu.zuo. All rights reserved.
//

#ifndef wait_timeout_hpp
#define wait_timeout_hpp

#include <stdio.h>
#include <mutex>
#include <atomic>
#include <list>
#include <memory>
#include <unordered_map>
#include <thread>

#include "my_memory.h"
#include "simple_memory.h"

#define USE_SIMPLE

#ifdef USE_STD
template<class _Tp>
using test_shared_ptr = std::shared_ptr<_Tp>;
#define test_make_shared std::make_shared
#elif defined(USE_SIMPLE)
template<class _Tp>
using test_shared_ptr = zy2::simple_shared_ptr<_Tp>;
#define test_make_shared std::make_simple_shared
#else
template<class _Tp>
using test_shared_ptr = zy::my_shared_ptr<_Tp>;
#define test_make_shared zy::my_make_shared
#endif //USE_STD

namespace im {
namespace citylife {
namespace sdk {
namespace core{

class wait {
public:
    /**
     * @brief 开始构造等待对象
     * @param timeout 超时时间（单位：毫秒）
     */
    explicit wait(int timeout);
    virtual ~wait();
    
    /**
     * @brief 获取当前对象id
     */
    int get_id();
    
    /**
     * @brief 开始等待
     */
    void start_wait();
    
    /**
     * @brief 取消等待
     */
    void cancel_wait();
    
    /**
     * @brief 后续删除
     */
    void delete_later();
    
private:
    std::mutex _mutex;
    std::condition_variable _variable;
    std::atomic_bool _quit;
    int _timeout;
    int _id;
};

class wait_manager {
    using wait = im::citylife::sdk::core::wait;
public:
    static wait_manager* instance();
    
    void add_wait(const test_shared_ptr<wait> obj_ptr);
    void add_wait2(const zy::my_shared_ptr<wait> obj_ptr);
    void add_wait3(const zy2::simple_shared_ptr<wait> obj_ptr);
    void del_wait(const test_shared_ptr<wait> obj_ptr);
    void clear();
    
protected:
    wait_manager(){}
    
private:
    std::unordered_map<int, test_shared_ptr<wait>> _list;
    std::unordered_map<int, zy::my_shared_ptr<wait>> _list2;
    std::unordered_map<int, zy2::simple_shared_ptr<wait>> _list3;
    static wait_manager _g;
};
class wait_manager2 {
    using wait = im::citylife::sdk::core::wait;
public:
    static wait_manager2* instance();
    
    void add_wait(wait* obj_ptr);
    void del_wait(wait* obj_ptr);
    void clear();
    
protected:
    wait_manager2(){}
    
private:
    std::unordered_map<int, wait*> _list;
    static wait_manager2 _g;
};

class auto_delete_manager {
public:
    static wait_manager2* instance();
    void clear_obj();
    
private:
    std::thread* _t;
};



class timeout_obj
{
public:
    static timeout_obj *instance();

    explicit timeout_obj();
    virtual ~timeout_obj();
    void start(int ms);
    void restart(int ms);
    void cancel();
    void reset();

private:
    void _cancel_with_wait();
    
private:
    static timeout_obj _obj;

    std::mutex _mutex;
    std::condition_variable _variable;
    std::atomic_bool _quit;
    bool _nb;
};

}
}
}
}

void test_wait_timeout();

#endif /* wait_timeout_hpp */
