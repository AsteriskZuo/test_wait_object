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
        if (!buf->append_num(id)) {
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
        if (!out_buf->get_num(out_id)) {
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
