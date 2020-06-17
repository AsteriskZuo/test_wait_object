//
//  test_buffer.cpp
//  test_wait_object
//
//  Created by zuoyu on 2020/6/15.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "test_buffer.hpp"

TEST_CASE("convert num", "[byte_order_convert]") {

    SECTION("test 4 bytes") {
        REQUIRE(sizeof(std::uint32_t) == 4);
        std::uint32_t input = 3689926330;
        std::uint32_t output = 0;
        std::uint32_t output_output = 0;
        byte_order_convert<sizeof(std::uint32_t)>::convert_v2(input, output);
        byte_order_convert<sizeof(std::uint32_t)>::convert_v2(output, output_output);
        REQUIRE(input == output_output);
    }
    SECTION("test 1 byte") {
        REQUIRE(sizeof(char) == 1);
        char input = 124;
        char output = 0;
        char output_output = 0;
        byte_order_convert<sizeof(char)>::convert_v2(input, output);
        byte_order_convert<sizeof(char)>::convert_v2(output, output_output);
        REQUIRE(input == output_output);
    }
    SECTION("test 2 bytes") {
        REQUIRE(sizeof(std::uint16_t) == 2);
        std::uint16_t input = 48362;
        std::uint16_t output = 0;
        std::uint16_t output_output = 0;
        byte_order_convert<sizeof(uint16_t)>::convert_v2(input, output);
        byte_order_convert<sizeof(uint16_t)>::convert_v2(output, output_output);
        REQUIRE(input == output_output);
    }
    SECTION("test 8 bytes") {
        REQUIRE(sizeof(std::uint64_t) == 8);
        std::uint64_t input = 207738368868302;
        std::uint64_t output = 0;
        std::uint64_t output_output = 0;
        byte_order_convert<sizeof(uint64_t)>::convert_v2(input, output);
        byte_order_convert<sizeof(uint64_t)>::convert_v2(output, output_output);
        REQUIRE(input == output_output);
    }
}

static void test_read_write_buffer_internal(char head, char checknum, std::uint32_t &id, std::string &name, std::string &binary_data) {
    std::size_t binary_data_size = binary_data.size();
    std::uint32_t vl = data_buffer<char>::test_get_num_length(id) + data_buffer<char>::test_get_string_length(name) + binary_data_size;
    bool ret = false;
    do {

        write_buffer<char> *buf = new write_buffer<char>();
        if (!buf->append_byte(&head, 1)) {
            break;
        }
        if (!buf->append_checksum_and_variable_length(vl, checknum)) {
            break;
        }
        if (!buf->append_number(id)) {
            break;
        }
        if (!buf->append_string(name)) {
            break;
        }
        if (!buf->append_byte(binary_data.data(), binary_data_size)) {
            break;
        }

        char out_head = 0;
        char out_checknum = head;
        std::uint32_t out_vl = 0;
        std::uint32_t out_id = 0;
        std::string out_name = "";

        read_buffer<char> *out_buf = new read_buffer<char>();

        char *data = nullptr;
        std::size_t data_size = 0;

        if (!buf->get_all_byte(data, data_size)) {
            break;
        }
        if (!out_buf->append_byte(data, data_size)) {
            break;
        }
        char *head_ptr = out_buf->get_byte(1);
        if (nullptr == head_ptr) {
            break;
        }
        out_head = *head_ptr;
        if (!out_buf->get_checksum_and_variable_length(out_vl, out_checknum)) {
            break;
        }
        if (!out_buf->get_number(out_id)) {
            break;
        }
        if (!out_buf->get_string(out_name)) {
            break;
        }
        std::size_t out_binary_data_size = out_buf->get_data_size();
        char *out_binary_data = out_buf->get_byte(out_buf->get_data_size());
        if (nullptr == out_binary_data) {
            break;
        }
        std::string test_out_binary_data = std::string(out_binary_data, out_binary_data_size);
        ret = true;

        delete buf;
        delete out_buf;

        REQUIRE(true == ret);
        REQUIRE(head == out_head);
        REQUIRE(checknum == out_checknum);
        REQUIRE(vl == out_vl);
        REQUIRE(id == out_id);
        REQUIRE(name == out_name);
        REQUIRE(out_vl == (data_buffer<char>::test_get_num_length(out_id) + data_buffer<char>::test_get_string_length(name) + test_out_binary_data.size()));
        REQUIRE(binary_data_size == test_out_binary_data.size());

    } while (false);

    REQUIRE(true == ret);
}
static void test_read_write_buffer(int section_index) {
    char head = 123;
    char checknum = head;
    std::uint32_t vl = 102;
    std::uint32_t id = 1231234423;
    std::string name = "onevariablelength";
    if (1 == section_index) {
        std::string binary_data = u8"123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"; //102
        test_read_write_buffer_internal(head, checknum, id, name, binary_data);
    } else if (2 == section_index) {
        std::string binary_data = u8"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"; //302
        test_read_write_buffer_internal(head, checknum, id, name, binary_data);
    } else if (3 == section_index) {
    }
}

TEST_CASE("read and write buffer", "[data_buffer]") {

    /**
     * ||char:head||char:checknum||char[1-3]variable_length||int:id|str:name|byte:xxx||
     */

    SECTION("test one byte variable length data") {
        test_read_write_buffer(1);
    }
    SECTION("test two byte variable length data") {
        test_read_write_buffer(2);
    }
    SECTION("test three byte variable length data") {
        test_read_write_buffer(3);
    }
}

static void test_buffer_copy() {
    data_buffer<char> buf1;
    data_buffer<char> buf2;
    // buf1 = buf2;//syntax error is ok
}
static void test_buffer_copy2() {
    data_buffer<char> buf1;
    // data_buffer<char> buf2(buf1);//syntax error is ok
}
static void test_buffer_move() {
    data_buffer<char> buf1;
    data_buffer<char> &&buf2(std::move(buf1));
    data_buffer<char> &&buf3 = std::move(buf1);
}
static void test_buffer_move2() {
    data_buffer<char> buf1;
    data_buffer<char> &&buf2(std::move(buf1));
    data_buffer<char> &&buf3 = std::move(buf1);
    //    buf2 = std::move(buf1);
}

TEST_CASE("test buffer copy", "[data_buffer]") {
}

//class test_move_class;
//static void swap(test_move_class &l, test_move_class &r) {
//    std::swap(r.ptr, l.ptr);
//    std::swap(r.ptr_size, l.ptr_size);
//}

class test_move_class {
public:
    int *ptr;
    std::size_t ptr_size;
    test_move_class() : ptr(nullptr), ptr_size(0) {
        std::cout << __func__ << ":" << __LINE__ << ":" << reinterpret_cast<int *>(this) << std::endl;
    }
    test_move_class(const int value) : ptr(new int(value)), ptr_size(1) {
        std::cout << __func__ << ":" << __LINE__ << ":" << reinterpret_cast<int *>(this) << std::endl;
    }
    virtual ~test_move_class() {
        std::cout << __func__ << ":" << __LINE__ << ":" << reinterpret_cast<int *>(this) << std::endl;
        if (ptr) {
            delete ptr;
        }
        ptr = nullptr;
    }
    test_move_class(const test_move_class &) = delete;
    test_move_class &operator=(const test_move_class &) = delete;
    test_move_class(test_move_class &&o) {
        if (false) {
            this->move(std::move(o));
            o.move(test_move_class());
        }
        if (true) {
            o.swap(*this);
            test_move_class().swap(o);
        }
    }
    test_move_class &operator=(test_move_class &&o) {
        if (this != &o) {
            if (false) {
                this->move(std::move(o));
                o.move(test_move_class());
            }
            if (true) {
                o.swap(*this);
                // o.swap(test_move_class());// tip error is ok
                test_move_class().swap(o);
            }
        }
        return *this;
    }
    void swap(test_move_class &o) {
        // 交换这种方式测试通过
        if (this != &o) {
            std::swap(this->ptr, o.ptr);
            std::swap(this->ptr_size, o.ptr_size);
        }
    }
    void move(test_move_class &&o) {
        // 内存过度释放，可能原因是使用不当或者标准库存在问题
        if (this != &o) {
            if (this->ptr) {
                delete this->ptr;
                this->ptr = nullptr;
            }
            this->ptr = std::move(o.ptr);
            this->ptr_size = std::move(o.ptr_size);
        }
    }
};

static void test_move_class_case1() {
    test_move_class a(3);
    test_move_class b(4);
    //test_move_class c(b);// tip error is ok
    //a = b;// tip error is ok
    a = std::move(b); // ok
    //a = std::move(a);// ok
    //test_move_class c(std::move(b));// ok
}

TEST_CASE("test move class", "[test_move_class]") {
    test_move_class_case1();
}

static void test_data_buffer_move_op() {
    // 自动调用父类构造函数、等函数
    data_buffer<char> a;
    data_buffer<char> b;
    a = std::move(b);
}
TEST_CASE("test data buffer class", "[data_buffer]") {
    test_data_buffer_move_op();
}

static void test_read_and_write_buffer_move_op() {
    // 自动调用父类构造函数、等函数
    // a、b各需要3个相同内存块
    read_buffer<char> a;
    read_buffer<char> b;
    a = std::move(b);
    write_buffer<char> c;
    write_buffer<char> d;
    c = std::move(d);
}
TEST_CASE("test final buffer class", "[final_buffer]") {
    test_read_and_write_buffer_move_op();
}

static void test_read_and_write_buffer_move_op2() {
    // 自动调用父类构造函数、等函数
    // a、b各需要1个相同内存块
    read_buffer<char> a;
    read_buffer<char> b(std::move(a));
    write_buffer<char> c;
    write_buffer<char> d(std::move(c));
}
TEST_CASE("test final buffer class2", "[final_buffer]") {
    test_read_and_write_buffer_move_op2();
}

static void test_read_and_write_buffer_append_large_data(int num) {
    read_buffer<char> a;
    std::string str = u8"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; //100
    bool ret = false;
    for (size_t i = 0; i < num; i++) {
        ret = a.append_byte(str.data(), str.size());
        if (false == ret) {
            break;
        }
    }
    std::string result = std::string(a.get_byte(str.size() * num), str.size() * num);
    int b = 0;
}
static void test_read_and_write_buffer_append_512() {
    read_buffer<char> a;
    std::string str = u8"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012"; //512
    a.append_byte(str.data(), str.size());
    a.append_byte(str.data(), 1);
}
TEST_CASE("test add large data", "[final_buffer]") {
    SECTION("512 byte") {
        test_read_and_write_buffer_append_512();
    }
    SECTION("100 byte") {
        test_read_and_write_buffer_append_large_data(1);
    }
    SECTION("600 byte") {
        test_read_and_write_buffer_append_large_data(6);
    }
    SECTION("6000 byte") {
        test_read_and_write_buffer_append_large_data(60);
    }
}
