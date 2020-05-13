// #include <type_traits>
#include <memory>
#include <iostream>

namespace zy
{
    using namespace std;

    template <class _Tp>
    class my_enable_shared_from_this;

    template <class _Tp>
    class my_weak_ptr;

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp>
    class my_shared_ptr
    {
    public:
        typedef _Tp element_type;

#if _LIBCPP_STD_VER > 14
        typedef my_weak_ptr<_Tp> weak_type;
#endif
    private:
        element_type *__ptr_;
        __shared_weak_count *__cntrl_;

        struct __nat
        {
            int __for_bool_;
        };

    public:
        constexpr my_shared_ptr() noexcept;

        constexpr my_shared_ptr(nullptr_t) noexcept;
        template <class _Yp>
        explicit my_shared_ptr(_Yp *__p,
                               typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
        template <class _Yp, class _Dp>
        my_shared_ptr(_Yp *__p, _Dp __d,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
        template <class _Yp, class _Dp, class _Alloc>
        my_shared_ptr(_Yp *__p, _Dp __d, _Alloc __a,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
        template <class _Dp>
        my_shared_ptr(nullptr_t __p, _Dp __d);
        template <class _Dp, class _Alloc>
        my_shared_ptr(nullptr_t __p, _Dp __d, _Alloc __a);
        template <class _Yp>
        my_shared_ptr(const my_shared_ptr<_Yp> &__r, element_type *__p) noexcept;

        my_shared_ptr(const my_shared_ptr &__r) noexcept;
        template <class _Yp>

        my_shared_ptr(const my_shared_ptr<_Yp> &__r,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat()) noexcept;
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

        my_shared_ptr(my_shared_ptr &&__r) noexcept;
        template <class _Yp>
        my_shared_ptr(my_shared_ptr<_Yp> &&__r,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat()) noexcept;
#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Yp>
        explicit my_shared_ptr(const my_weak_ptr<_Yp> &__r,
                               typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Yp>
        my_shared_ptr(auto_ptr<_Yp> &&__r,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
#else
        template <class _Yp>
        my_shared_ptr(auto_ptr<_Yp> __r,
                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
#endif
#endif
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Yp, class _Dp>
        my_shared_ptr(unique_ptr<_Yp, _Dp> &&,
                      typename enable_if<
                          !is_lvalue_reference<_Dp>::value &&
                              !is_array<_Yp>::value &&
                              is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type *>::value,
                          my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
        template <class _Yp, class _Dp>
        my_shared_ptr(unique_ptr<_Yp, _Dp> &&,
                      typename enable_if<
                          is_lvalue_reference<_Dp>::value &&
                              !is_array<_Yp>::value &&
                              is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type *>::value,
                          my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
#else  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
        template <class _Yp, class _Dp>
        my_shared_ptr(unique_ptr<_Yp, _Dp>,
                      typename enable_if<
                          !is_lvalue_reference<_Dp>::value &&
                              !is_array<_Yp>::value &&
                              is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type *>::value,
                          my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
        template <class _Yp, class _Dp>
        my_shared_ptr(unique_ptr<_Yp, _Dp>,
                      typename enable_if<
                          is_lvalue_reference<_Dp>::value &&
                              !is_array<_Yp>::value &&
                              is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type *>::value,
                          my_shared_ptr<_Tp>::__nat>::type = my_shared_ptr<_Tp>::__nat());
#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

        ~my_shared_ptr();

        my_shared_ptr &operator=(const my_shared_ptr &__r) noexcept;
        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            my_shared_ptr &>::type

        operator=(const my_shared_ptr<_Yp> &__r) noexcept;
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

        my_shared_ptr &operator=(my_shared_ptr &&__r) noexcept;
        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            my_shared_ptr<_Tp> &>::type

        operator=(my_shared_ptr<_Yp> &&__r);
#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
        template <class _Yp>

        typename enable_if<
            !is_array<_Yp>::value &&
                is_convertible<_Yp *, element_type *>::value,
            my_shared_ptr>::type &
        operator=(auto_ptr<_Yp> &&__r);
#endif
#else // _LIBCPP_HAS_NO_RVALUE_REFERENCES
#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
        template <class _Yp>

        typename enable_if<
            !is_array<_Yp>::value &&
                is_convertible<_Yp *, element_type *>::value,
            my_shared_ptr &>::type
        operator=(auto_ptr<_Yp> __r);
#endif
#endif
        template <class _Yp, class _Dp>
        typename enable_if<
            !is_array<_Yp>::value &&
                is_convertible<typename unique_ptr<_Yp, _Dp>::pointer, element_type *>::value,
            my_shared_ptr &>::type
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

        operator=(unique_ptr<_Yp, _Dp> &&__r);
#else // _LIBCPP_HAS_NO_RVALUE_REFERENCES

        operator=(unique_ptr<_Yp, _Dp> __r);
#endif

        void swap(my_shared_ptr &__r) noexcept;

        void reset() noexcept;
        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            void>::type

        reset(_Yp *__p);
        template <class _Yp, class _Dp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            void>::type

        reset(_Yp *__p, _Dp __d);
        template <class _Yp, class _Dp, class _Alloc>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            void>::type

        reset(_Yp *__p, _Dp __d, _Alloc __a);

        element_type *get() const noexcept { return __ptr_; }

        element_type *operator->() const noexcept { return __ptr_; }

        long use_count() const noexcept { return __cntrl_ ? __cntrl_->use_count() : 0; }

        bool unique() const noexcept { return use_count() == 1; }

        _LIBCPP_EXPLICIT operator bool() const noexcept { return get() != 0; }
        template <class _Up>

        bool owner_before(my_shared_ptr<_Up> const &__p) const noexcept
        {
            return __cntrl_ < __p.__cntrl_;
        }
        template <class _Up>

        bool owner_before(my_weak_ptr<_Up> const &__p) const noexcept
        {
            return __cntrl_ < __p.__cntrl_;
        }

        bool
        __owner_equivalent(const my_shared_ptr &__p) const
        {
            return __cntrl_ == __p.__cntrl_;
        }

#ifndef _LIBCPP_NO_RTTI
        template <class _Dp>

        _Dp *__get_deleter() const noexcept
        {
            return static_cast<_Dp *>(__cntrl_
                                          ? const_cast<void *>(__cntrl_->__get_deleter(typeid(_Dp)))
                                          : nullptr);
        }
#endif // _LIBCPP_NO_RTTI

        template <class _Yp, class _CntrlBlk>
        static my_shared_ptr<_Tp>
        __create_with_control_block(_Yp *__p, _CntrlBlk *__cntrl)
        {
            std::cout << __func__ << ":" << __LINE__ << std::endl;
            my_shared_ptr<_Tp> __r;
            __r.__ptr_ = __p;
            __r.__cntrl_ = __cntrl;
            __r.__enable_weak_this(__r.__ptr_, __r.__ptr_);
            return __r;
        }

        template <class _Alloc, class... _Args>
        static my_shared_ptr<_Tp>
        allocate_shared(const _Alloc &__a, _Args &&... __args);

    private:
        template <class _Yp, bool = is_function<_Yp>::value>
        struct __shared_ptr_default_allocator
        {
            typedef allocator<_Yp> type;
        };

        template <class _Yp>
        struct __shared_ptr_default_allocator<_Yp, true>
        {
            typedef allocator<__shared_ptr_dummy_rebind_allocator_type> type;
        };

        template <class _Yp, class _OrigPtr>

        typename enable_if<is_convertible<_OrigPtr *,
                                                    const my_enable_shared_from_this<_Yp> *>::value,
                                void>::type
        __enable_weak_this(const my_enable_shared_from_this<_Yp> *__e,
                           _OrigPtr *__ptr) noexcept
        {
            std::cout << __func__ << ":" << __LINE__ << std::endl;
            typedef typename remove_cv<_Yp>::type _RawYp;
            if (__e && __e->__weak_this_.expired())
            {
                __e->__weak_this_ = my_shared_ptr<_RawYp>(*this,
                                                          const_cast<_RawYp *>(static_cast<const _Yp *>(__ptr)));
            }
        }

        void __enable_weak_this(...) noexcept {}

        template <class _Up>
        friend class my_shared_ptr;
        template <class _Up>
        friend class my_weak_ptr;
    };

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp>
    class my_weak_ptr
    {
    public:
        typedef _Tp element_type;

    private:
        element_type *__ptr_;
        __shared_weak_count *__cntrl_;

    public:
        constexpr my_weak_ptr() noexcept;
        
        // template <class _Yp>
        // my_weak_ptr(my_shared_ptr<_Yp> const &__r,
        //             typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type = my_shared_ptr<_Tp>::__nat()) noexcept;

        my_weak_ptr(my_weak_ptr const &__r) noexcept;

        // template <class _Yp>
        // my_weak_ptr(my_weak_ptr<_Yp> const &__r,
        //             typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type = my_shared_ptr<_Tp>::__nat()) noexcept;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

        my_weak_ptr(my_weak_ptr &&__r) noexcept;

        // template <class _Yp>
        // my_weak_ptr(my_weak_ptr<_Yp> &&__r,
        //             typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type = my_shared_ptr<_Tp>::__nat()) noexcept;
#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES
        ~my_weak_ptr();

        my_weak_ptr &operator=(my_weak_ptr const &__r) noexcept;
        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            my_weak_ptr &>::type

        operator=(my_weak_ptr<_Yp> const &__r) noexcept;

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

        my_weak_ptr &operator=(my_weak_ptr &&__r) noexcept;
        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            my_weak_ptr &>::type

        operator=(my_weak_ptr<_Yp> &&__r) noexcept;

#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

        template <class _Yp>
        typename enable_if<
            is_convertible<_Yp *, element_type *>::value,
            my_weak_ptr &>::type

        operator=(my_shared_ptr<_Yp> const &__r) noexcept;

        void swap(my_weak_ptr &__r) noexcept;

        void reset() noexcept;

        long use_count() const noexcept
        {
            return __cntrl_ ? __cntrl_->use_count() : 0;
        }

        bool expired() const noexcept
        {
            return __cntrl_ == 0 || __cntrl_->use_count() == 0;
        }
        my_shared_ptr<_Tp> lock() const noexcept;
        template <class _Up>
        bool owner_before(const my_shared_ptr<_Up> &__r) const noexcept
        {
            return __cntrl_ < __r.__cntrl_;
        }
        template <class _Up>
        bool owner_before(const my_weak_ptr<_Up> &__r) const noexcept
        {
            return __cntrl_ < __r.__cntrl_;
        }

        template <class _Up>
        friend class my_weak_ptr;
        template <class _Up>
        friend class my_shared_ptr;
    };

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp>
    class my_enable_shared_from_this
    {
        mutable my_weak_ptr<_Tp> __weak_this_;

    protected:
        constexpr my_enable_shared_from_this() noexcept {}

        my_enable_shared_from_this(my_enable_shared_from_this const &) noexcept {}

        my_enable_shared_from_this &operator=(my_enable_shared_from_this const &) noexcept
        {
            return *this;
        }

        ~my_enable_shared_from_this() {}

    public:
        my_shared_ptr<_Tp> shared_from_this()
        {
            return my_shared_ptr<_Tp>(__weak_this_);
        }

        my_shared_ptr<_Tp const> shared_from_this() const
        {
            return my_shared_ptr<const _Tp>(__weak_this_);
        }

#if _LIBCPP_STD_VER > 14

        my_weak_ptr<_Tp> weak_from_this() noexcept
        {
            return __weak_this_;
        }

        my_weak_ptr<const _Tp> weak_from_this() const noexcept
        {
            return __weak_this_;
        }
#endif // _LIBCPP_STD_VER > 14

        template <class _Up>
        friend class my_shared_ptr;
    };

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp, class... _Args>
    inline
        typename enable_if<
            !is_array<_Tp>::value,
            my_shared_ptr<_Tp>>::type
        my_make_shared(_Args &&... __args)
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        static_assert(is_constructible<_Tp, _Args...>::value, "Can't construct object in make_shared");
        typedef __shared_ptr_emplace<_Tp, allocator<_Tp>> _CntrlBlk;
        typedef allocator<_CntrlBlk> _A2;
        typedef __allocator_destructor<_A2> _D2;

        _A2 __a2;
        unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
        ::new (__hold2.get()) _CntrlBlk(__a2, forward<_Args>(__args)...);

        _Tp *__ptr = __hold2.get()->get();
        return my_shared_ptr<_Tp>::__create_with_control_block(__ptr, __hold2.release());
    }

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp>
    inline constexpr my_shared_ptr<_Tp>::my_shared_ptr() noexcept
        : __ptr_(0),
          __cntrl_(0)
    {
    }

    template <class _Tp>
    inline constexpr my_shared_ptr<_Tp>::my_shared_ptr(nullptr_t) noexcept
        : __ptr_(0),
          __cntrl_(0)
    {
    }

    template <class _Tp>
    template <class _Yp>
    my_shared_ptr<_Tp>::my_shared_ptr(_Yp *__p,
                                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type)
        : __ptr_(__p)
    {
        unique_ptr<_Yp> __hold(__p);
        typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
        typedef __shared_ptr_pointer<_Yp *, default_delete<_Yp>, _AllocT> _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__p, default_delete<_Yp>(), _AllocT());
        __hold.release();
        __enable_weak_this(__p, __p);
    }

    template <class _Tp>
    template <class _Yp, class _Dp>
    my_shared_ptr<_Tp>::my_shared_ptr(_Yp *__p, _Dp __d,
                                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type)
        : __ptr_(__p)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif // _LIBCPP_NO_EXCEPTIONS
            typedef typename __shared_ptr_default_allocator<_Yp>::type _AllocT;
            typedef __shared_ptr_pointer<_Yp *, _Dp, _AllocT> _CntrlBlk;
            __cntrl_ = new _CntrlBlk(__p, __d, _AllocT());
            __enable_weak_this(__p, __p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __d(__p);
            throw;
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    template <class _Tp>
    template <class _Dp>
    my_shared_ptr<_Tp>::my_shared_ptr(nullptr_t __p, _Dp __d)
        : __ptr_(0)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif // _LIBCPP_NO_EXCEPTIONS
            typedef typename __shared_ptr_default_allocator<_Tp>::type _AllocT;
            typedef __shared_ptr_pointer<nullptr_t, _Dp, _AllocT> _CntrlBlk;
            __cntrl_ = new _CntrlBlk(__p, __d, _AllocT());
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __d(__p);
            throw;
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    template <class _Tp>
    template <class _Yp, class _Dp, class _Alloc>
    my_shared_ptr<_Tp>::my_shared_ptr(_Yp *__p, _Dp __d, _Alloc __a,
                                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type)
        : __ptr_(__p)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif // _LIBCPP_NO_EXCEPTIONS
            typedef __shared_ptr_pointer<_Yp *, _Dp, _Alloc> _CntrlBlk;
            typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
            typedef __allocator_destructor<_A2> _D2;
            _A2 __a2(__a);
            unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
            ::new (static_cast<void *>(_VSTD::addressof(*__hold2.get())))
                _CntrlBlk(__p, __d, __a);
            __cntrl_ = _VSTD::addressof(*__hold2.release());
            __enable_weak_this(__p, __p);
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __d(__p);
            throw;
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    template <class _Tp>
    template <class _Dp, class _Alloc>
    my_shared_ptr<_Tp>::my_shared_ptr(nullptr_t __p, _Dp __d, _Alloc __a)
        : __ptr_(0)
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try
        {
#endif // _LIBCPP_NO_EXCEPTIONS
            typedef __shared_ptr_pointer<nullptr_t, _Dp, _Alloc> _CntrlBlk;
            typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
            typedef __allocator_destructor<_A2> _D2;
            _A2 __a2(__a);
            unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
            ::new (static_cast<void *>(_VSTD::addressof(*__hold2.get())))
                _CntrlBlk(__p, __d, __a);
            __cntrl_ = _VSTD::addressof(*__hold2.release());
#ifndef _LIBCPP_NO_EXCEPTIONS
        }
        catch (...)
        {
            __d(__p);
            throw;
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    template <class _Tp>
    template <class _Yp>
    inline my_shared_ptr<_Tp>::my_shared_ptr(const my_shared_ptr<_Yp> &__r, element_type *__p) noexcept
        : __ptr_(__p),
          __cntrl_(__r.__cntrl_)
    {
        if (__cntrl_)
            __cntrl_->__add_shared();
    }

    template <class _Tp>
    inline my_shared_ptr<_Tp>::my_shared_ptr(const my_shared_ptr &__r) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        if (__cntrl_)
            __cntrl_->__add_shared();
    }

    template <class _Tp>
    template <class _Yp>
    inline my_shared_ptr<_Tp>::my_shared_ptr(const my_shared_ptr<_Yp> &__r,
                                             typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        if (__cntrl_)
            __cntrl_->__add_shared();
    }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    inline my_shared_ptr<_Tp>::my_shared_ptr(my_shared_ptr &&__r) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        __r.__ptr_ = 0;
        __r.__cntrl_ = 0;
    }

    template <class _Tp>
    template <class _Yp>
    inline my_shared_ptr<_Tp>::my_shared_ptr(my_shared_ptr<_Yp> &&__r,
                                             typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        __r.__ptr_ = 0;
        __r.__cntrl_ = 0;
    }

#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
    template <class _Tp>
    template <class _Yp>
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    my_shared_ptr<_Tp>::my_shared_ptr(auto_ptr<_Yp> &&__r,
#else
    my_shared_ptr<_Tp>::my_shared_ptr(auto_ptr<_Yp> __r,
#endif
                                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type)
        : __ptr_(__r.get())
    {
        typedef __shared_ptr_pointer<_Yp *, default_delete<_Yp>, allocator<_Yp>> _CntrlBlk;
        __cntrl_ = new _CntrlBlk(__r.get(), default_delete<_Yp>(), allocator<_Yp>());
        __enable_weak_this(__r.get(), __r.get());
        __r.release();
    }
#endif

    template <class _Tp>
    template <class _Alloc, class... _Args>
    my_shared_ptr<_Tp>
    my_shared_ptr<_Tp>::allocate_shared(const _Alloc &__a, _Args &&... __args)
    {
        static_assert(is_constructible<_Tp, _Args...>::value, "Can't construct object in allocate_shared");
        typedef __shared_ptr_emplace<_Tp, _Alloc> _CntrlBlk;
        typedef typename __allocator_traits_rebind<_Alloc, _CntrlBlk>::type _A2;
        typedef __allocator_destructor<_A2> _D2;
        _A2 __a2(__a);
        unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
        ::new (static_cast<void *>(_VSTD::addressof(*__hold2.get())))
            _CntrlBlk(__a, _VSTD::forward<_Args>(__args)...);
        my_shared_ptr<_Tp> __r;
        __r.__ptr_ = __hold2.get()->get();
        __r.__cntrl_ = _VSTD::addressof(*__hold2.release());
        __r.__enable_weak_this(__r.__ptr_, __r.__ptr_);
        return __r;
    }

    template <class _Tp>
    my_shared_ptr<_Tp>::~my_shared_ptr()
    {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        if (__cntrl_)
            __cntrl_->__release_shared();
    }

    template <class _Tp>
    inline my_shared_ptr<_Tp> &
    my_shared_ptr<_Tp>::operator=(const my_shared_ptr &__r) noexcept
    {
        my_shared_ptr(__r).swap(*this);
        return *this;
    }

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            my_shared_ptr<_Tp> &>::type
        my_shared_ptr<_Tp>::operator=(const my_shared_ptr<_Yp> &__r) noexcept
    {
        my_shared_ptr(__r).swap(*this);
        return *this;
    }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    inline my_shared_ptr<_Tp> &
    my_shared_ptr<_Tp>::operator=(my_shared_ptr &&__r) noexcept
    {
        my_shared_ptr(_VSTD::move(__r)).swap(*this);
        return *this;
    }

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            my_shared_ptr<_Tp> &>::type
        my_shared_ptr<_Tp>::operator=(my_shared_ptr<_Yp> &&__r)
    {
        my_shared_ptr(_VSTD::move(__r)).swap(*this);
        return *this;
    }

#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            !is_array<_Yp>::value &&
                is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            my_shared_ptr<_Tp>>::type &
        my_shared_ptr<_Tp>::operator=(auto_ptr<_Yp> &&__r)
    {
        my_shared_ptr(_VSTD::move(__r)).swap(*this);
        return *this;
    }
#endif

#else // _LIBCPP_HAS_NO_RVALUE_REFERENCES

#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            !is_array<_Yp>::value &&
                is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            my_shared_ptr<_Tp> &>::type
        my_shared_ptr<_Tp>::operator=(auto_ptr<_Yp> __r)
    {
        my_shared_ptr(__r).swap(*this);
        return *this;
    }
#endif

#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    inline void
    my_shared_ptr<_Tp>::swap(my_shared_ptr &__r) noexcept
    {
        _VSTD::swap(__ptr_, __r.__ptr_);
        _VSTD::swap(__cntrl_, __r.__cntrl_);
    }

    template <class _Tp>
    inline void
    my_shared_ptr<_Tp>::reset() noexcept
    {
        my_shared_ptr().swap(*this);
    }

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            void>::type
        my_shared_ptr<_Tp>::reset(_Yp *__p)
    {
        my_shared_ptr(__p).swap(*this);
    }

    template <class _Tp>
    template <class _Yp, class _Dp>
    inline
        typename enable_if<
            is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            void>::type
        my_shared_ptr<_Tp>::reset(_Yp *__p, _Dp __d)
    {
        my_shared_ptr(__p, __d).swap(*this);
    }

    template <class _Tp>
    template <class _Yp, class _Dp, class _Alloc>
    inline
        typename enable_if<
            is_convertible<_Yp *, typename my_shared_ptr<_Tp>::element_type *>::value,
            void>::type
        my_shared_ptr<_Tp>::reset(_Yp *__p, _Dp __d, _Alloc __a)
    {
        my_shared_ptr(__p, __d, __a).swap(*this);
    }

    template <class _Tp, class... _Args>
    inline
        typename enable_if<
            !is_array<_Tp>::value,
            my_shared_ptr<_Tp>>::type
        make_shared(_Args &&... __args)
    {
        static_assert(is_constructible<_Tp, _Args...>::value, "Can't construct object in make_shared");
        typedef __shared_ptr_emplace<_Tp, allocator<_Tp>> _CntrlBlk;
        typedef allocator<_CntrlBlk> _A2;
        typedef __allocator_destructor<_A2> _D2;

        _A2 __a2;
        unique_ptr<_CntrlBlk, _D2> __hold2(__a2.allocate(1), _D2(__a2, 1));
        ::new (__hold2.get()) _CntrlBlk(__a2, _VSTD::forward<_Args>(__args)...);

        _Tp *__ptr = __hold2.get()->get();
        return my_shared_ptr<_Tp>::__create_with_control_block(__ptr, __hold2.release());
    }

    template <class _Tp, class _Alloc, class... _Args>
    inline
        typename enable_if<
            !is_array<_Tp>::value,
            my_shared_ptr<_Tp>>::type
        allocate_shared(const _Alloc &__a, _Args &&... __args)
    {
        return my_shared_ptr<_Tp>::allocate_shared(__a, _VSTD::forward<_Args>(__args)...);
    }

    template <class _Tp, class _Up>
    inline bool
    operator==(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
        return __x.get() == __y.get();
    }

    template <class _Tp, class _Up>
    inline bool
    operator!=(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
        return !(__x == __y);
    }

    template <class _Tp, class _Up>
    inline bool
    operator<(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
#if _LIBCPP_STD_VER <= 11
        typedef typename common_type<_Tp *, _Up *>::type _Vp;
        return less<_Vp>()(__x.get(), __y.get());
#else
        return less<>()(__x.get(), __y.get());
#endif
    }

    template <class _Tp, class _Up>
    inline bool
    operator>(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
        return __y < __x;
    }

    template <class _Tp, class _Up>
    inline bool
    operator<=(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
        return !(__y < __x);
    }

    template <class _Tp, class _Up>
    inline bool
    operator>=(const my_shared_ptr<_Tp> &__x, const my_shared_ptr<_Up> &__y) noexcept
    {
        return !(__x < __y);
    }

    template <class _Tp>
    inline bool
    operator==(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return !__x;
    }

    template <class _Tp>
    inline bool
    operator==(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return !__x;
    }

    template <class _Tp>
    inline bool
    operator!=(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return static_cast<bool>(__x);
    }

    template <class _Tp>
    inline bool
    operator!=(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return static_cast<bool>(__x);
    }

    template <class _Tp>
    inline bool
    operator<(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return less<_Tp *>()(__x.get(), nullptr);
    }

    template <class _Tp>
    inline bool
    operator<(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return less<_Tp *>()(nullptr, __x.get());
    }

    template <class _Tp>
    inline bool
    operator>(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return nullptr < __x;
    }

    template <class _Tp>
    inline bool
    operator>(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return __x < nullptr;
    }

    template <class _Tp>
    inline bool
    operator<=(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return !(nullptr < __x);
    }

    template <class _Tp>
    inline bool
    operator<=(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return !(__x < nullptr);
    }

    template <class _Tp>
    inline bool
    operator>=(const my_shared_ptr<_Tp> &__x, nullptr_t) noexcept
    {
        return !(__x < nullptr);
    }

    template <class _Tp>
    inline bool
    operator>=(nullptr_t, const my_shared_ptr<_Tp> &__x) noexcept
    {
        return !(nullptr < __x);
    }

    template <class _Tp>
    inline void
    swap(my_shared_ptr<_Tp> &__x, my_shared_ptr<_Tp> &__y) noexcept
    {
        __x.swap(__y);
    }

    template <class _Tp, class _Up>
    inline
        typename enable_if<
            !is_array<_Tp>::value && !is_array<_Up>::value,
            my_shared_ptr<_Tp>>::type
        static_pointer_cast(const my_shared_ptr<_Up> &__r) noexcept
    {
        return my_shared_ptr<_Tp>(__r, static_cast<_Tp *>(__r.get()));
    }

    template <class _Tp, class _Up>
    inline
        typename enable_if<
            !is_array<_Tp>::value && !is_array<_Up>::value,
            my_shared_ptr<_Tp>>::type
        dynamic_pointer_cast(const my_shared_ptr<_Up> &__r) noexcept
    {
        _Tp *__p = dynamic_cast<_Tp *>(__r.get());
        return __p ? my_shared_ptr<_Tp>(__r, __p) : my_shared_ptr<_Tp>();
    }

    template <class _Tp, class _Up>
    typename enable_if<
        is_array<_Tp>::value == is_array<_Up>::value,
        my_shared_ptr<_Tp>>::type
    const_pointer_cast(const my_shared_ptr<_Up> &__r) noexcept
    {
        typedef typename remove_extent<_Tp>::type _RTp;
        return my_shared_ptr<_Tp>(__r, const_cast<_RTp *>(__r.get()));
    }

#ifndef _LIBCPP_NO_RTTI

    template <class _Dp, class _Tp>
    inline _Dp *
    get_deleter(const my_shared_ptr<_Tp> &__p) noexcept
    {
        return __p.template __get_deleter<_Dp>();
    }

#endif // _LIBCPP_NO_RTTI

    ///////////////////////////////////////////////////////////////////////////////

    template <class _Tp>
    inline constexpr my_weak_ptr<_Tp>::my_weak_ptr() noexcept
        : __ptr_(0),
          __cntrl_(0)
    {
    }

    template <class _Tp>
    inline my_weak_ptr<_Tp>::my_weak_ptr(my_weak_ptr const &__r) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        if (__cntrl_)
            __cntrl_->__add_weak();
    }

    // template <class _Tp>
    // template <class _Yp>
    // inline my_weak_ptr<_Tp>::my_weak_ptr(my_shared_ptr<_Yp> const &__r,
    //                                      typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type) noexcept
    //     : __ptr_(__r.__ptr_),
    //       __cntrl_(__r.__cntrl_)
    // {
    //     if (__cntrl_)
    //         __cntrl_->__add_weak();
    // }

    // template <class _Tp>
    // template <class _Yp>
    // inline my_weak_ptr<_Tp>::my_weak_ptr(my_weak_ptr<_Yp> const &__r,
    //                                      typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type) noexcept
    //     : __ptr_(__r.__ptr_),
    //       __cntrl_(__r.__cntrl_)
    // {
    //     if (__cntrl_)
    //         __cntrl_->__add_weak();
    // }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    inline my_weak_ptr<_Tp>::my_weak_ptr(my_weak_ptr &&__r) noexcept
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_)
    {
        __r.__ptr_ = 0;
        __r.__cntrl_ = 0;
    }

    // template <class _Tp>
    // template <class _Yp>
    // inline my_weak_ptr<_Tp>::my_weak_ptr(my_weak_ptr<_Yp> &&__r,
    //                                      typename enable_if<is_convertible<_Yp *, _Tp *>::value, my_shared_ptr<_Tp>::__nat *>::type) noexcept
    //     : __ptr_(__r.__ptr_),
    //       __cntrl_(__r.__cntrl_)
    // {
    //     __r.__ptr_ = 0;
    //     __r.__cntrl_ = 0;
    // }

#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    my_weak_ptr<_Tp>::~my_weak_ptr()
    {
        if (__cntrl_)
            __cntrl_->__release_weak();
    }

    template <class _Tp>
    inline my_weak_ptr<_Tp> &
    my_weak_ptr<_Tp>::operator=(my_weak_ptr const &__r) noexcept
    {
        my_weak_ptr(__r).swap(*this);
        return *this;
    }

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, _Tp *>::value,
            my_weak_ptr<_Tp> &>::type
        my_weak_ptr<_Tp>::operator=(my_weak_ptr<_Yp> const &__r) noexcept
    {
        my_weak_ptr(__r).swap(*this);
        return *this;
    }

#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    inline my_weak_ptr<_Tp> &
    my_weak_ptr<_Tp>::operator=(my_weak_ptr &&__r) noexcept
    {
        my_weak_ptr(move(__r)).swap(*this);
        return *this;
    }

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, _Tp *>::value,
            my_weak_ptr<_Tp> &>::type
        my_weak_ptr<_Tp>::operator=(my_weak_ptr<_Yp> &&__r) noexcept
    {
        my_weak_ptr(move(__r)).swap(*this);
        return *this;
    }

#endif // _LIBCPP_HAS_NO_RVALUE_REFERENCES

    template <class _Tp>
    template <class _Yp>
    inline
        typename enable_if<
            is_convertible<_Yp *, _Tp *>::value,
            my_weak_ptr<_Tp> &>::type
        my_weak_ptr<_Tp>::operator=(my_shared_ptr<_Yp> const &__r) noexcept
    {
        my_weak_ptr(__r).swap(*this);
        return *this;
    }

    template <class _Tp>
    inline void
    my_weak_ptr<_Tp>::swap(my_weak_ptr &__r) noexcept
    {
        swap(__ptr_, __r.__ptr_);
        swap(__cntrl_, __r.__cntrl_);
    }

    template <class _Tp>
    inline void
    swap(my_weak_ptr<_Tp> &__x, my_weak_ptr<_Tp> &__y) noexcept
    {
        __x.swap(__y);
    }

    template <class _Tp>
    inline void
    my_weak_ptr<_Tp>::reset() noexcept
    {
        my_weak_ptr().swap(*this);
    }

    template <class _Tp>
    template <class _Yp>
    my_shared_ptr<_Tp>::my_shared_ptr(const my_weak_ptr<_Yp> &__r,
                                      typename enable_if<is_convertible<_Yp *, element_type *>::value, my_shared_ptr<_Tp>::__nat>::type)
        : __ptr_(__r.__ptr_),
          __cntrl_(__r.__cntrl_ ? __r.__cntrl_->lock() : __r.__cntrl_)
    {
        if (__cntrl_ == 0)
            __throw_bad_weak_ptr();
    }

    template <class _Tp>
    my_shared_ptr<_Tp>
    my_weak_ptr<_Tp>::lock() const noexcept
    {
        my_shared_ptr<_Tp> __r;
        __r.__cntrl_ = __cntrl_ ? __cntrl_->lock() : __cntrl_;
        if (__r.__cntrl_)
            __r.__ptr_ = __ptr_;
        return __r;
    }

} // namespace zy
