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
    std::uint32_t vl = data_buffer<char>::get_number_length(id) + data_buffer<char>::get_string_length(name) + binary_data_size;
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
        REQUIRE(out_vl == (data_buffer<char>::get_number_length(out_id) + data_buffer<char>::get_string_length(name) + test_out_binary_data.size()));
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

/**
 * 从服务器接收消息：拆包、粘包测试
 * case1：单个协议：变长为0
 * case2：单个协议：变长不为0
 * case3：两个协议：1.协议体为0+协议不为0 2.协议不为0+协议为0
 * case4：单个不完整协议：只有head
 * case5：单个不完整协议：只有head和checknum
 * case6：单个不完整协议：只有head、checknum、和变长第一个字节（一共两个字节）
 * case7：单个不完整协议：只有head、checknum、和变长完整字节
 * case8：单个不完整协议：只有head、checknum、变长完整字节和部分协议体
 * case9：单个协议，加上单个不完整协议：只有head、checknum、变长完整字节和部分协议体
 * case10：变长解析：1个字节的变长
 * case11：变长解析：2个字节的变长
 * case12：变长解析：3个字节的变长
 * case13：变长解析：4个字节的变长
 * case14：变长解析：变长字节不符合规范
 * case15：变长解析：变长需要2个字节，但是只给1个字节
 * case16：变长解析：变长需要3个字节，但是只给1个字节
 * case17：变长解析：变长需要3个字节，但是只给2个字节
 * case18：单个协议：一个一个字节的接收和解析
 * case19：单个协议：2个2个字节的接收和解析
 * case20：错误协议：没有head
 * case21：错误协议：没有checknum
 * case22：错误协议：变长错误
 * case23：错误协议：变长比协议体多
 * case24：错误协议：变长比协议体少
 * case25：单个协议（没有协议体） + 错误协议（没有head）
 * case26：错误协议（没有head）+ 单个协议（没有协议体）
 * case27：数据测试：写入数据，变长为0的协议体的数据（3字节），并且在读取它
 * case28：数据测试：写入数据，变长为88字节的协议体数据，并且读取它
 * case29：数据测试：写入数据，变长为450字节的协议体数据，并且读取它
 * case30：数据测试：写入数据，变长为80000字节的协议体数据，并且读取它 （8322815）（0x7efeff 约等于7.9MB） (131072) (0x20000 约定于128KB)
 * case31：数据测试：写入数据，变长为200000字节的协议体数据，并且读取它 （8322815）（0x7efeff 约等于7.9MB） (131072) (0x20000 约定于128KB)
 *
 */
    
    //test

static bool create_protocol_buffer(const std::uint32_t body_size, write_buffer<char> &wbuf, std::size_t &vl_byte_size) {
    bool ret = false;
    do {
        std::uint64_t size_t_size = std::numeric_limits<std::size_t>::max();
        if (body_size > size_t_size) {
            break;
        }
        char head = (char)64;
        wbuf.append_byte((const char *)(&head), 1);
        wbuf.append_checksum_and_variable_length(body_size, head);
        vl_byte_size = wbuf.get_data_size() - 2;
        if (0 < body_size) {
            std::uint32_t body_size_tmp = body_size;
            int body_size_digit = 0;
            while (0 < body_size_tmp) {
                body_size_tmp /= 10;
                ++body_size_digit;
            }
            std::string body_size_size = std::to_string(body_size);
            std::string body = body_size_size;
            body.append(std::string(body_size - 2 * body_size_digit, 'c'));
            body.append(body_size_size);
            bool _ret = wbuf.append_byte(body.data(), body.size());
            if (false == _ret) {
                break;
            }
            body.clear();
        }
        ret = true;
    } while (false);
    return ret;
}
static void data_buffer_test_case(const std::uint32_t body_size) {
    write_buffer<char> wbuf;
    char *buffer = 0;
    std::size_t buffer_size = 0;
    std::size_t vl_byte_size = 0;
    bool ret = create_protocol_buffer(body_size, wbuf, vl_byte_size);
    REQUIRE(true == ret);
    wbuf.get_all_byte(buffer, buffer_size);

    read_buffer<char> rbuf;
    ret = rbuf.append_byte(buffer, buffer_size);
    REQUIRE(true == ret);
    char out_head = *rbuf.get_byte(1);
    std::size_t out_vl_num = 0;
    char checknum = 0;
    ret = rbuf.get_checksum_and_variable_length((std::uint32_t &)out_vl_num, checknum);
    REQUIRE(true == ret);
    REQUIRE(buffer[0] == (int)out_head);
    REQUIRE(buffer[0] == checknum);
    REQUIRE(body_size == out_vl_num);
}

TEST_CASE("test read and write buffer", "[data_buffer]") {
    SECTION("case27：数据测试：写入数据，变长为0的协议体的数据（3字节），并且在读取它") {
        data_buffer_test_case(0);
    }
    SECTION("case28：数据测试：写入数据，变长为88字节的协议体数据，并且读取它") {
        data_buffer_test_case(88);
    }
    SECTION("case29：数据测试：写入数据，变长为450字节的协议体数据，并且读取它") {
        data_buffer_test_case(450);
    }
    SECTION("case30：数据测试：写入数据，变长为80000字节的协议体数据，并且读取它 （8322815）（0x7efeff 约等于7.9MB） (131072) (0x20000 约定于128KB)") {
        data_buffer_test_case(80000);
    }
    SECTION("数据测试：写入数据，变长为200000字节的协议体数据，并且读取它 （8322815）（0x7efeff 约等于7.9MB） (131072) (0x20000 约定于128KB)") {
        data_buffer_test_case(200000);
    }
}

TEST_CASE("test variable length", "[data_buffer]") {
    SECTION("case10：变长解析：1个字节的变长") {
        std::uint32_t vl_num = 8;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(vl_num == output_vl_num);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(0 == output_ret2);
        REQUIRE(1 == output_vl_byte_size);
    }
    SECTION("case11：变长解析：2个字节的变长") {
        std::uint32_t vl_num = 2445;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(vl_num == output_vl_num);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(0 == output_ret2);
        REQUIRE(2 == output_vl_byte_size);
    }
    SECTION("case12：变长解析：3个字节的变长") {
        std::uint32_t vl_num = 22644;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(vl_num == output_vl_num);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(0 == output_ret2);
        REQUIRE(3 == output_vl_byte_size);
    }
    SECTION("case13：变长解析：4个字节的变长") {
        std::uint32_t vl_num = 725972140;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        bool input_ret = write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        vl_byte_size = 4; // for test
        bool output_ret = read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(false == input_ret);
        REQUIRE(false == output_ret);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(1 == output_ret2);
    }
    SECTION("case14：变长解析：变长字节不符合规范") {
        std::uint32_t vl_num = 725972140;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        bool input_ret = write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        vl_byte_size = 3; // for test
        bool output_ret = read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(false == input_ret);
        REQUIRE(false == output_ret);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(1 == output_ret2);
    }
    SECTION("case15：变长解析：变长需要2个字节，但是只给1个字节") {
        std::uint32_t vl_num = 2445;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        vl_byte_size = 1;
        bool output_ret = read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(false == output_ret);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(2 == output_ret2);
    }
    SECTION("case16：变长解析：变长需要3个字节，但是只给1个字节") {
        std::uint32_t vl_num = 22644;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        vl_byte_size = 1;
        bool output_ret = read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(false == output_ret);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(2 == output_ret2);
    }
    SECTION("case17：变长解析：变长需要3个字节，但是只给2个字节") {
        std::uint32_t vl_num = 22644;
        char vl_byte[4] = {0};
        std::size_t vl_byte_size = 0;
        char code = (char)0x00;
        write_buffer<char>::get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, code);

        char output_checknum = (char)0x00;
        std::uint32_t output_vl_num = 0;
        vl_byte_size = 2;
        bool output_ret = read_buffer<char>::get_checksum_and_variable_length(vl_byte, vl_byte_size, output_checknum, output_vl_num);
        REQUIRE(false == output_ret);

        std::size_t output_vl_byte_size = 0;
        int output_ret2 = read_buffer<char>::peek_variable_length(vl_byte, vl_byte_size, output_vl_byte_size);
        REQUIRE(2 == output_ret2);
    }
}

class test_protocol_factory {
public:
    struct create_protocol_info {
        bool is_whole_protocol; //true = yes false = no
                                //
                                //        int is_exist_head;     // 0.not exist 1.exit part
                                //        int is_exist_checksum; // 0.not exist 1.exit part
                                //
                                //        int is_exist_vl; // 0.not exist 1.exit part 2.exit whole
                                //        int vl_whole_length;
                                //        int vl_part_length;
                                //
                                //        int is_exist_body; // 0.not exist 1.exit part 2.exit whole
        int body_whole_length;
        //        int body_part_length;
    };
    struct create_error_protocol_info {
        int error_format; //1.没有head 2.没有check 3.变长错误 4.协议体和变长不一致:变长多 5.协议体和变长不一致:变长少
    };

public:
    test_protocol_factory(/* args */) {}
    ~test_protocol_factory() {}

    /**
     * 创建正确的协议字节流
     * @param info 创建协议需求
     * @param out_data 协议流
     * @param out_data_size 协议流长度
     * @return bool 是否创建成功 true.成功
     */
    static bool create_buffer_from_message(const create_protocol_info &info, char *&out_data, std::size_t &out_data_size) {
        bool ret = false;
        do {
            if (info.is_whole_protocol) {
                int body_whole_length = info.body_whole_length;

                char head = (char)(4 << 4);
                write_buffer<char> buffer;
                buffer.append_byte(&head, 1);
                buffer.append_checksum_and_variable_length(body_whole_length, head);

                if (0 != body_whole_length) {
                    int body_whole_length_digit = 0;
                    if (0 > body_whole_length) {
                        break;
                    }
                    while (0 < body_whole_length) {
                        int tmp = body_whole_length / 10;
                        ++body_whole_length_digit;
                        if (0 == tmp) {
                            break;
                        }
                        body_whole_length = tmp;
                    }
                    std::string body_whole_length_num = std::to_string(info.body_whole_length);
                    std::string body = body_whole_length_num;
                    body.append(std::string(info.body_whole_length - 2 * body_whole_length_digit, 'b'));
                    body.append(body_whole_length_num);
                    buffer.append_byte(body.c_str(), body.size());
                }
                out_data_size = buffer.get_data_size();
                out_data = new char[out_data_size];
                std::memcpy(out_data, buffer.get_data(), out_data_size);
                ret = true;
            }
        } while (false);
        return ret;
    }

    /**
     * @brief Create a error buffer from message object
     * 错误数据：用例：
     * 1.没有head
     * 2.没有checknum
     * 3.变长错误
     * 4.协议体和变长不一致:变长多
     * 5.协议体和变长不一致:协议体多
     * 
     * @param info 错误规则
     * @param out_data 输出错误数据流
     * @param out_data_size 错误数据流长度
     * @return true 创建成功
     * @return false 创建失败
     */
    static bool create_error_buffer_from_message(const create_error_protocol_info &info, char *&out_data, std::size_t &out_data_size) {
        bool ret = false;
        do {
            write_buffer<char> buffer;
            char *buffer_data = nullptr;
            std::size_t buffer_data_size = 0;
            if (1 == info.error_format) {
                char head = 64; //4
                const char *body = "123456";
                buffer.append_byte((const char *)&head, 1);
                buffer.append_checksum_and_variable_length((std::uint32_t)strlen(body), 0);
                buffer.append_byte(body, strlen(body));
                int _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
                buffer_data += 1;
                buffer_data_size -= 1;
            } else if (2 == info.error_format) {
                char head = 64; //4
                std::string body = std::string(6, 'b');
                buffer.append_byte((const char *)&head, 1);
                buffer.append_checksum_and_variable_length((std::uint32_t)body.size(), head);
                int _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
                buffer_data += 2;
                buffer_data_size -= 2;
                buffer.append_byte(&head, 1);
                buffer.append_byte(buffer_data, buffer_data_size);
                buffer.append_byte(body.data(), body.size());
                _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
            } else if (3 == info.error_format) {
                char head = 64; //4
                std::string body = std::string(450, 'b');
                buffer.append_byte((const char *)&head, 1);
                buffer.append_checksum_and_variable_length((std::uint32_t)body.size(), head);
                int _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
                buffer.append_byte(buffer_data, 3);
                buffer.append_byte(body.data(), body.size());
                _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
            } else if (4 == info.error_format) {
                char head = 64; //4
                std::string body = std::string(88, 'b');
                buffer.append_byte((const char *)&head, 1);
                buffer.append_checksum_and_variable_length((std::uint32_t)body.size(), head);
                int _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
                buffer.append_byte(buffer_data, buffer_data_size);
                body = body.substr(10);
                buffer.append_byte(body.data(), body.size());
                _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
            } else if (5 == info.error_format) {
                char head = 64; //4
                std::string body = std::string(88, 'b');
                buffer.append_byte((const char *)&head, 1);
                buffer.append_checksum_and_variable_length((std::uint32_t)body.size(), head);
                int _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
                buffer.append_byte(buffer_data, buffer_data_size);
                body.append(std::string(6, 'c'));
                buffer.append_byte(body.data(), body.size());
                _ret = buffer.get_all_byte(buffer_data, buffer_data_size);
            } else {
                break;
            }
            out_data = new char[buffer_data_size];
            out_data_size = buffer_data_size;
            std::memcpy(out_data, buffer_data, buffer_data_size);
            ret = true;
        } while (false);
        return ret;
    }
};

TEST_CASE("test read buffer from server", "[byte_buffer]") {
    SECTION("case1：单个协议：变长为0") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 0;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        read_buffer<char> buffer;
        REQUIRE(true == buffer.append_byte(data, data_size));
        char output_head = *buffer.get_byte(1);
        char output_checknum;
        std::size_t output_vl_num = 0;
        REQUIRE(true == buffer.get_checksum_and_variable_length((std::uint32_t &)output_vl_num, output_checknum));
        REQUIRE(0 == output_vl_num);
        REQUIRE(0 == buffer.get_data_size());
        REQUIRE(nullptr == buffer.get_data());
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, data_size);
        std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
        REQUIRE(nullptr != cp_buffer.get());
        REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
    }
    SECTION("case2：单个协议：变长不为0") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 6;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        read_buffer<char> buffer;
        REQUIRE(true == buffer.append_byte(data, data_size));
        char output_head = *buffer.get_byte(1);
        char output_checknum;
        std::size_t output_vl_num = 0;
        REQUIRE(true == buffer.get_checksum_and_variable_length((std::uint32_t &)output_vl_num, output_checknum));
        REQUIRE(info.body_whole_length == output_vl_num);
        char *output_body = buffer.get_byte(output_vl_num);
        REQUIRE(nullptr != output_body);
        REQUIRE(0 == buffer.get_data_size());
        REQUIRE(nullptr == buffer.get_data());
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, data_size);
        std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
        REQUIRE(nullptr != cp_buffer.get());
        REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
    }
    SECTION("case3：两个协议") {
        {
            byte_buffer_from_server byte_buffer;
            char *all_data = nullptr;
            std::size_t all_data_size = 0;
            std::size_t protocol_data_size = 0;
            {
                test_protocol_factory::create_protocol_info info;
                info.is_whole_protocol = true;
                info.body_whole_length = 0;
                char *data = nullptr;
                std::size_t data_size = 0;
                REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
                all_data = new char[data_size];
                all_data_size = data_size;
                std::memcpy(all_data, data, data_size);
            }
            {
                test_protocol_factory::create_protocol_info info;
                info.is_whole_protocol = true;
                info.body_whole_length = 6;
                char *data = nullptr;
                std::size_t data_size = 0;
                REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
                char *temp_data = new char[data_size + all_data_size];
                std::memcpy(temp_data, all_data, all_data_size);
                std::memcpy(temp_data + all_data_size, data, data_size);
                delete[] all_data;
                all_data = temp_data;
                all_data_size += data_size;
            }
            int ret = byte_buffer.read_buffer_from_server(all_data, all_data_size);
            int count = 0;
            while (true) {
                std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
                ++count;
                if (1 == count) {
                    REQUIRE(nullptr != cp_buffer.get());
                    REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
                } else if (2 == count) {
                    REQUIRE(nullptr != cp_buffer.get());
                    REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
                } else if (3 == count) {
                    REQUIRE(nullptr == cp_buffer.get());
                    break;
                }
            }
            delete[] all_data;
        }
        {
            byte_buffer_from_server byte_buffer;
            char *all_data = nullptr;
            std::size_t all_data_size = 0;
            std::size_t protocol_data_size = 0;
            {
                test_protocol_factory::create_protocol_info info;
                info.is_whole_protocol = true;
                info.body_whole_length = 6;
                char *data = nullptr;
                std::size_t data_size = 0;
                REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
                all_data = new char[data_size];
                all_data_size = data_size;
                std::memcpy(all_data, data, data_size);
            }
            {
                test_protocol_factory::create_protocol_info info;
                info.is_whole_protocol = true;
                info.body_whole_length = 0;
                char *data = nullptr;
                std::size_t data_size = 0;
                REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
                char *temp_data = new char[data_size + all_data_size];
                std::memcpy(temp_data, all_data, all_data_size);
                std::memcpy(temp_data + all_data_size, data, data_size);
                delete[] all_data;
                all_data = temp_data;
                all_data_size += data_size;
            }
            int ret = byte_buffer.read_buffer_from_server(all_data, all_data_size);
            int count = 0;
            while (true) {
                std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
                ++count;
                if (1 == count) {
                    REQUIRE(nullptr != cp_buffer.get());
                    REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
                } else if (2 == count) {
                    REQUIRE(nullptr != cp_buffer.get());
                    REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
                } else if (3 == count) {
                    REQUIRE(nullptr == cp_buffer.get());
                    break;
                }
            }
            delete[] all_data;
        }
    }
    SECTION("case4：单个不完整协议：只有head") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 0;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 1);
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 1, data_size - 1);
        REQUIRE(0 == ret);
    }
    SECTION("case5：单个不完整协议：只有head和checknum") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 0;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 2);
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 2, data_size - 2);
        REQUIRE(0 == ret);
    }
    SECTION("case6：单个不完整协议：只有head、checknum、和变长第一个字节（一共两个字节）") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 450;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 3);
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 3, data_size - 3);
        REQUIRE(0 == ret);
    }
    SECTION("case7：单个不完整协议：只有head、checknum、和变长完整字节") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 450;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 4);
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 4, data_size - 4);
        REQUIRE(0 == ret);
    }
    SECTION("case8：单个不完整协议：只有head、checknum、变长完整字节和部分协议体") {
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 450;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            byte_buffer_from_server byte_buffer;
            int ret = byte_buffer.read_buffer_from_server(data, 5);
            REQUIRE(2 == ret);
            ret = byte_buffer.read_buffer_from_server(data + 5, data_size - 5);
            REQUIRE(0 == ret);
        }
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 450;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            byte_buffer_from_server byte_buffer;
            int ret = byte_buffer.read_buffer_from_server(data, data_size - 1);
            REQUIRE(2 == ret);
            ret = byte_buffer.read_buffer_from_server(data + data_size - 1, 1);
            REQUIRE(0 == ret);
        }
    }
    SECTION("case9：单个协议，加上单个不完整协议：只有head、checknum、变长完整字节和部分协议体") {
        byte_buffer_from_server byte_buffer;
        char *all_data = nullptr;
        std::size_t all_data_size = 0;
        std::size_t protocol_data_size = 0;
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 0;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            all_data = new char[data_size];
            all_data_size = data_size;
            std::memcpy(all_data, data, data_size);
        }
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 450;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            char *temp_data = new char[data_size + all_data_size];
            std::memcpy(temp_data, all_data, all_data_size);
            std::memcpy(temp_data + all_data_size, data, data_size);
            delete[] all_data;
            all_data = temp_data;
            all_data_size += data_size;
        }
        int ret = byte_buffer.read_buffer_from_server(all_data, 6);
        int count = 0;
        while (true) {
            std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
            ++count;
            if (1 == count) {
                REQUIRE(nullptr != cp_buffer.get());
                REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
            } else if (2 == count) {
                REQUIRE(nullptr == cp_buffer.get());
                break;
            }
        }
        delete[] all_data;
    }
    SECTION("case18：单个协议：一个一个字节的接收和解析") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 450;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 1); // head
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 1, 1); // checksum
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 2, 1); // vl first byte
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 3, 1); // vl second byte
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 4, 1); // vl first body byte
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 5, data_size - 5); // vl remaining body byte
        REQUIRE(0 == ret);
        std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
        REQUIRE(nullptr != cp_buffer.get());
        REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
        REQUIRE(info.body_whole_length + 2 + 2 == cp_buffer->cp_buffer_size);
    }
    SECTION("case19：单个协议：2个2个字节的接收和解析") {
        test_protocol_factory::create_protocol_info info;
        info.is_whole_protocol = true;
        info.body_whole_length = 450;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        int ret = byte_buffer.read_buffer_from_server(data, 2); // head + checksum
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 2, 2); // vl two byte
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 4, 2); // body frist bytes
        REQUIRE(2 == ret);
        ret = byte_buffer.read_buffer_from_server(data + 6, data_size - 6); // body remaining bytes
        REQUIRE(0 == ret);
        std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
        REQUIRE(nullptr != cp_buffer.get());
        REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
        REQUIRE(info.body_whole_length + 2 + 2 == cp_buffer->cp_buffer_size);
    }
    SECTION("case20：错误协议：没有head") {
        test_protocol_factory::create_error_protocol_info info;
        info.error_format = 1;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        REQUIRE(1 == byte_buffer.read_buffer_from_server(data, data_size));
        REQUIRE(nullptr == byte_buffer.front_and_pop());
    }
    SECTION("case21：错误协议：没有checknum") {
        test_protocol_factory::create_error_protocol_info info;
        info.error_format = 2;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        REQUIRE(1 == byte_buffer.read_buffer_from_server(data, data_size));
        REQUIRE(nullptr == byte_buffer.front_and_pop());
    }
    SECTION("case22：错误协议：变长错误") {
        test_protocol_factory::create_error_protocol_info info;
        info.error_format = 3;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        REQUIRE(1 == byte_buffer.read_buffer_from_server(data, data_size));
        REQUIRE(nullptr == byte_buffer.front_and_pop());
    }
    SECTION("case23：错误协议：变长比协议体多") {
        test_protocol_factory::create_error_protocol_info info;
        info.error_format = 4;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        REQUIRE(2 == byte_buffer.read_buffer_from_server(data, data_size));
        REQUIRE(nullptr == byte_buffer.front_and_pop());
    }
    SECTION("case24：错误协议：变长比协议体少") {
        test_protocol_factory::create_error_protocol_info info;
        info.error_format = 5;
        char *data = nullptr;
        std::size_t data_size = 0;
        REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
        byte_buffer_from_server byte_buffer;
        REQUIRE(1 == byte_buffer.read_buffer_from_server(data, data_size));
        REQUIRE(nullptr != byte_buffer.front_and_pop());
    }
    SECTION("case25：单个协议（没有协议体） + 错误协议（没有head）") {
        byte_buffer_from_server byte_buffer;
        char *all_data = nullptr;
        std::size_t all_data_size = 0;
        std::size_t protocol_data_size = 0;
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 0;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            all_data = new char[data_size];
            all_data_size = data_size;
            std::memcpy(all_data, data, data_size);
        }
        {
            test_protocol_factory::create_error_protocol_info info;
            info.error_format = 1;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
            char *temp_data = new char[data_size + all_data_size];
            std::memcpy(temp_data, all_data, all_data_size);
            std::memcpy(temp_data + all_data_size, data, data_size);
            delete[] all_data;
            all_data = temp_data;
            all_data_size += data_size;
        }
        int ret = byte_buffer.read_buffer_from_server(all_data, all_data_size);
        REQUIRE(1 == ret);
        int count = 0;
        while (true) {
            std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
            ++count;
            if (1 == count) {
                REQUIRE(nullptr != cp_buffer.get());
                REQUIRE(cp_buffer->cp_buffer_current_size == cp_buffer->cp_buffer_size);
            } else if (2 == count) {
                REQUIRE(nullptr == cp_buffer.get());
                break;
            }
        }
        delete[] all_data;
    }
    SECTION("case25：错误协议（没有head）+ 单个协议（没有协议体）") {
        byte_buffer_from_server byte_buffer;
        char *all_data = nullptr;
        std::size_t all_data_size = 0;
        std::size_t protocol_data_size = 0;
        {
            test_protocol_factory::create_error_protocol_info info;
            info.error_format = 2;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_error_buffer_from_message(info, data, data_size));
            all_data = new char[data_size];
            all_data_size = data_size;
            std::memcpy(all_data, data, data_size);
        }
        {
            test_protocol_factory::create_protocol_info info;
            info.is_whole_protocol = true;
            info.body_whole_length = 6;
            char *data = nullptr;
            std::size_t data_size = 0;
            REQUIRE(true == test_protocol_factory::create_buffer_from_message(info, data, data_size));
            char *temp_data = new char[data_size + all_data_size];
            std::memcpy(temp_data, all_data, all_data_size);
            std::memcpy(temp_data + all_data_size, data, data_size);
            delete[] all_data;
            all_data = temp_data;
            all_data_size += data_size;
        }
        int ret = byte_buffer.read_buffer_from_server(all_data, all_data_size);
        REQUIRE(1 == ret);
        int count = 0;
        while (true) {
            std::shared_ptr<byte_buffer_from_server::citylife_im_protocol> cp_buffer = byte_buffer.front_and_pop();
            ++count;
            if (1 == count) {
                REQUIRE(nullptr == cp_buffer.get());
                break;
            }
        }
        delete[] all_data;
    }
}
