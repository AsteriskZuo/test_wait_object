//
//  test_buffer.cpp
//  test_wait_object
//
//  Created by zuoyu on 2020/6/15.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#define CATCH_CONFIG_MAIN

#include "test_buffer.hpp"
#include "catch.hpp"

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

TEST_CASE("test read and write buffer", "[basic_buffer][write_buffer]") {
    SECTION("simple write and read") {
        /**
         * ||char:head||char[1-3]variable_length||int:id|str:name|byte:xxx||
         */

        char head = 123;
        std::uint32_t vl = 456;
        std::uint32_t id = 1231234423;
        std::string name = "test";

        write_buffer<char> *buf = new write_buffer<char>();
        buf->append_byte(&head, 1);
        buf->append_variable1_length(vl);
        buf->append_num(id);
        buf->append_string(name);

        char out_head[1] = {0};
        std::uint32_t out_vl = 0;
        std::uint32_t out_id = 0;
        std::string out_name = "";

        read_buffer<char> *out_buf = new read_buffer<char>();
        char *data = nullptr;
        std::size_t data_size = 0;
        buf->get_byte(data, data_size);
        out_buf->append_data(data, data_size);
        out_head[0] = *out_buf->get_byte(1);
        out_buf->get_variable1_length(out_vl);
        out_buf->get_num<std::uint32_t>(out_id);
        out_buf->get_string(out_name);
    }
}
