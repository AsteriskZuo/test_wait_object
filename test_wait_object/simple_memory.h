
/**
 * simple shared pointer
 */

#ifndef SIMPLE_MEMORY_H
#define SIMPLE_MEMORY_H

#include <memory>
#include <atomic>
#include <tuple>
#include <type_traits>
#include <iostream>

namespace zy2
{

class _use_count
{
private:
    std::atomic<long> __use_count;

private:
    _use_count(const _use_count &);
    _use_count &operator=(const _use_count &);

public:
    ~_use_count() {}
    explicit _use_count() : __use_count(0) {}
    void add_shared() { std::atomic_fetch_add<long>(&__use_count, 1); }
    void release_shared()
    {
        std::cout << __func__ << ":" << __LINE__ << ":" << __use_count.load() << std::endl;
        if (0 == std::atomic_fetch_sub<long>(&__use_count, 1))
        {
            on_zero_shared();
        }
        std::cout << __func__ << ":" << __LINE__ << ":" << __use_count.load() << std::endl;
    }
    long get_use_count() { return __use_count.load() + 1; }
    virtual void on_zero_shared() = 0;
};

template <class _Tp>
class simple_shared_ptr;

template <class _Tp, class _Alloc, class _Dp>
class _use_obj : public _use_count
{
    template <class _Up>
    friend class simple_shared_ptr;

private:
    _Tp *__data;

public:
    // template <class... _Args>
    // explicit _use_obj(_Alloc a, _Args &&... __args)
    //     : _use_count()
    // {
    //     _Tp *p = ::new _Tp(std::forward<_Args>(__args)...);
    //     assert(p);
    //     // __data = std::make_tuple<_Tp *, _Alloc, _Dp>(p, std::move(a), std::nullptr_t());
    //     __data = std::make_tuple<_Tp *, _Alloc, _Dp>(p, a, std::default_delete<_Tp>());
    // }
    template <class... _Args>
    explicit _use_obj(_Args &&... __args)
        : _use_count(), __data(nullptr)
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        _Tp *p = ::new _Tp(std::forward<_Args>(__args)...);
        assert(p);
        __data = p;
    }
    virtual ~_use_obj()
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    void on_zero_shared() override
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        assert(__data);
        ::delete __data;
        __data = nullptr;

        std::allocator<_use_obj<_Tp, _Alloc, _Dp>> a;
        a.destroy(this);
        a.deallocate(this, 1);
    }
    _Tp *get() noexcept
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        if (0 < get_use_count())
        {
            return __data;
        }
        return nullptr;
    }
};

template <class _Tp>
class simple_shared_ptr
{
private:
    _use_count *__use_obj;
    _Tp *__ptr;

public:
    simple_shared_ptr();
    simple_shared_ptr(const simple_shared_ptr &o);
    simple_shared_ptr(simple_shared_ptr &&o);
    simple_shared_ptr &operator=(const simple_shared_ptr &o);
    simple_shared_ptr &operator=(simple_shared_ptr &&o);
    ~simple_shared_ptr();

    _Tp *get() const noexcept { return __ptr; }
    typename std::add_lvalue_reference<_Tp>::type operator*() const noexcept { return *__ptr; }
    _Tp *operator->() const noexcept { return __ptr; }
    long use_count() const noexcept { return __use_obj ? __use_obj->get_use_count() : 0; }
    bool unique() const noexcept { return use_count() == 1; }
    explicit operator bool() const noexcept { return get() != 0; }

public:
    static simple_shared_ptr create_shared_ptr(_use_count *use_obj, _Tp *ptr);

private:
    void swap(simple_shared_ptr &o);
    void reset();
    long get_use_count();
};

template <class _Tp, class... _Args>
inline typename std::enable_if<
    !std::is_array<_Tp>::value,
    simple_shared_ptr<_Tp>>::type
make_simple_shared(_Args &&... __args)
{
    static_assert(std::is_constructible<_Tp, _Args...>::value, "Can't construct object in make_simple_shared");
    typedef _use_obj<_Tp, std::allocator<_Tp>, std::default_delete<_Tp>> _use_obj_t;
    std::allocator<_use_obj_t> a;
    _use_obj_t *use_obj = a.allocate(1);
    a.construct(use_obj, std::forward<_Args>(__args)...);
    _Tp *p = use_obj->get();
    return simple_shared_ptr<_Tp>::create_shared_ptr(use_obj, p);
}

template <class _Tp>
simple_shared_ptr<_Tp>::simple_shared_ptr()
    : __use_obj(nullptr), __ptr(nullptr)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
}
template <class _Tp>
simple_shared_ptr<_Tp>::simple_shared_ptr(const simple_shared_ptr &o)
    : __use_obj(o.__use_obj), __ptr(o.__ptr)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    if (__use_obj)
    {
        __use_obj->add_shared();
    }
}

template <class _Tp>
simple_shared_ptr<_Tp>::simple_shared_ptr(simple_shared_ptr &&o)
    : __use_obj(o.__use_obj), __ptr(o.__ptr)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    o.__use_obj = nullptr;
    o.__ptr = nullptr;
}

template <class _Tp>
simple_shared_ptr<_Tp> &simple_shared_ptr<_Tp>::operator=(const simple_shared_ptr<_Tp> &o)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    simple_shared_ptr(o).swap(*this);
    return *this;
}

template <class _Tp>
simple_shared_ptr<_Tp> &simple_shared_ptr<_Tp>::operator=(simple_shared_ptr<_Tp> &&o)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    simple_shared_ptr(std::move(o)).swap(*this);
    return *this;
}
template <class _Tp>
simple_shared_ptr<_Tp>::~simple_shared_ptr()
{
    if (__use_obj)
    {
        __use_obj->release_shared();
    }
    __use_obj = nullptr;
    std::cout << __func__ << ":" << __LINE__ << std::endl;
}
template <class _Tp>
void simple_shared_ptr<_Tp>::swap(simple_shared_ptr<_Tp> &o)
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    std::swap(__use_obj, o.__use_obj);
}
template <class _Tp>
void simple_shared_ptr<_Tp>::reset()
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    simple_shared_ptr().swap(*this);
}
template <class _Tp>
long simple_shared_ptr<_Tp>::get_use_count()
{
    std::cout << __func__ << ":" << __LINE__ << std::endl;
    return __use_obj->get_use_count();
}
template <class _Tp>
simple_shared_ptr<_Tp> simple_shared_ptr<_Tp>::create_shared_ptr(_use_count *use_obj, _Tp *ptr)
{
    simple_shared_ptr<_Tp> ret;
    ret.__use_obj = use_obj;
    ret.__ptr = ptr;
    return ret;
}

} // namespace zy2

void test_simple_memory();

#endif //SIMPLE_MEMORY_H
