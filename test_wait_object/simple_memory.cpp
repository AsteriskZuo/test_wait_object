#include "simple_memory.h"
#include <iostream>

static void test1();
static void test2();
void test_simple_memory()
{
    test2();
}

struct XXXXXXX
{
    XXXXXXX() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    XXXXXXX(int a) {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
        a1 = a;
    }
    ~XXXXXXX() {
        std::cout << __func__ << ":" << __LINE__ << std::endl;
    }
    int a1;
};
static void test1()
{
    XXXXXXX x1(3);
    std::tuple<XXXXXXX, XXXXXXX*> t1 = std::make_tuple<XXXXXXX, XXXXXXX*>(XXXXXXX(1), &x1);
//    std::tuple<XXXXXXX, XXXXXXX*> t2 = std::make_tuple(XXXXXXX{.a1 = 1}, &(XXXXXXX{.a1 = 3}));//error
    auto ret1 = std::get<0>(t1);
    auto ret2 = std::get<1>(t1);
    
    std::cout << "ret1:" << typeid(ret1).name() << ":" << ret1.a1 << std::endl;
    std::cout << "ret2:" << typeid(ret2).name() << ":" << ret2->a1 << std::endl;
}


static void test2()
{
    zy2::simple_shared_ptr<XXXXXXX> sptr = zy2::make_simple_shared<XXXXXXX>();
    std::cout << "sptr:" << sptr.use_count() << std::endl;
}
