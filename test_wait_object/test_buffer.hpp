//
//  test_buffer.hpp
//  test_wait_object
//
//  Created by zuoyu on 2020/6/15.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#ifndef test_buffer_hpp
#define test_buffer_hpp

#include <algorithm> // for std::max std::min
#include <cstdlib>
#include <cstdlib> // for std::malloc std::realloc std::free
#include <cstring> // for std::memset stdstd::memcpy std::memmove
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>

#define BUFFER_DEFAULT_SIZE 0x200
#define BUFFER_DEFAULT_MAX_SIZE 0x1fffff

template <std::size_t vl_num_tmp>
class byte_order_convert {
public:
    static bool convert(const unsigned char (&input)[vl_num_tmp], unsigned char (&output)[vl_num_tmp]) {
        if (1 == vl_num_tmp) {
            (void)nullptr;
        } else if (2 == vl_num_tmp) {
            output[0] = input[1];
            output[1] = input[0];
        } else if (4 == vl_num_tmp) {
            output[0] = input[3];
            output[1] = input[2];
            output[2] = input[1];
            output[3] = input[0];
        } else if (8 == vl_num_tmp) {
            output[0] = input[7];
            output[1] = input[6];
            output[2] = input[5];
            output[3] = input[4];
            output[4] = input[3];
            output[5] = input[2];
            output[6] = input[1];
            output[7] = input[0];
        } else {
            throw std::logic_error("This vl_num_tmp is not supported.");
            return false;
        }
        return true;
    }
    template <typename number_t>
    static bool convert(const number_t &input, number_t &output) {
        unsigned char input_bytes[vl_num_tmp] = {0};
        unsigned char output_bytes[vl_num_tmp] = {0};
        std::memcpy(input_bytes, &input, vl_num_tmp);
        bool ret = convert(input_bytes, output_bytes);
        if (ret) {
            std::memcpy(&output, output_bytes, vl_num_tmp);
        }
        return ret;
    }
    template <typename number_t>
    static bool convert_v2(const number_t &input, number_t &output) {
        std::size_t len = sizeof(number_t);
        if (1 == len) {
            output = input;
        } else if (2 == len) {
            output = (input & 0xff00) >> 8 | (input & 0x00ff) << 8;
        } else if (4 == len) {
            output = (input & 0xff000000) >> 24 | (input & 0x00ff0000) >> 8 | (input & 0x0000ff00) << 8 | (input & 0x000000ff) << 24;
        } else if (8 == len) {
            output = (input & 0xff00000000000000) >> 56 | (input & 0x00ff000000000000) >> 40 | (input & 0x0000ff0000000000) >> 24 | (input & 0x000000ff00000000) >> 8 | (input & 0x00000000ff000000) << 8 | (input & 0x0000000000ff0000) << 24 | (input & 0x000000000000ff00) << 40 | (input & 0x00000000000000ff) << 56;
        } else {
            return false;
        }
        return true;
    }

private:
    byte_order_convert(/* args */) = delete;
    ~byte_order_convert() = delete;
    byte_order_convert(const byte_order_convert &) = delete;
    byte_order_convert &operator=(const byte_order_convert &) = delete;
    byte_order_convert(byte_order_convert &&) = delete;
    byte_order_convert &operator=(byte_order_convert &&) = delete;
};

template <typename char_t, std::size_t n = BUFFER_DEFAULT_SIZE>
class basic_buffer {
protected:
    char_t *buf_;
    std::size_t buf_size_;

public:
    basic_buffer() {
        this->allocate(n);
    }
    virtual ~basic_buffer() {
        this->free();
    }
    basic_buffer(basic_buffer &&o) {
        buf_ = nullptr;
        buf_size_ = 0;
        this->swap(o);
        basic_buffer(true).swap(o);
    }
    basic_buffer &operator=(basic_buffer &&o) {
        if (this != &o) {
            this->swap(o);
            basic_buffer(true).swap(o);
        }
        return *this;
    }
    basic_buffer(bool is_temp_obj) : buf_(nullptr), buf_size_(0) {
        // to do nothing
    }

protected:
    void resize(const std::size_t new_size) {
        this->reallocate(new_size);
    }
    std::size_t buffer_size() const { return buf_size_; }
    char_t *buffer() const { return buf_; }
    void reset() {
        std::memset(buf_, 0, buf_size_);
    }
    void swap(basic_buffer &o) {
        std::swap(buf_, o.buf_);
        std::swap(buf_size_, o.buf_size_);
    }
    void move(basic_buffer &&o) {
        buf_ = std::move(o.buf_);
        buf_size_ = std::move(o.buf_size_);
    }

private:
    void allocate(const std::size_t size) {
        do {
            if (size >= BUFFER_DEFAULT_MAX_SIZE) {
                throw std::length_error("The requested memory exceeds the specified vl_num_tmp.");
                break;
            }
            buf_ = (char_t *)std::malloc(size);
            if (nullptr == buf_) {
                throw std::bad_alloc();
                break;
            }
            buf_size_ = size;
        } while (false);
    }
    void reallocate(const std::size_t new_size) {
        do {
            if (new_size >= BUFFER_DEFAULT_MAX_SIZE) {
                throw std::length_error("The requested memory exceeds the specified vl_num_tmp.");
                break;
            }
            if (0 == new_size) {
                this->free();
            } else {
                void *new_buf = std::realloc(buf_, new_size);
                if (nullptr == new_buf) {
                    this->free();
                    throw std::bad_alloc();
                    break;
                }
                buf_ = (char_t *)new_buf;
                buf_size_ = new_size;
            }
        } while (false);
    }
    void free() {
        if (buf_) {
            std::free(buf_);
        }
        buf_ = nullptr;
        buf_size_ = 0;
    }

private:
    // Disallow copying and assignment.
    basic_buffer(const basic_buffer &) = delete;
    basic_buffer &operator=(const basic_buffer &) = delete;
};

template <typename char_t>
class data_buffer : public basic_buffer<char_t> {
public:
    typedef basic_buffer<char_t> basic_buffer_t;

protected:
    char_t *data_;
    std::size_t data_size_;

public:
    data_buffer() : data_(nullptr), data_size_(0) {
        data_ = basic_buffer_t::buffer();
    }
    virtual ~data_buffer() {
        data_ = nullptr;
        data_size_ = 0;
    }
    data_buffer(data_buffer &&o) : basic_buffer_t(dynamic_cast<basic_buffer_t &&>(o)) {
        this->swap(o);
        data_buffer(true).swap(o);
    }
    data_buffer &operator=(data_buffer &&o) {
        basic_buffer_t::operator=(dynamic_cast<basic_buffer_t &&>(o));
        if (this != &o) {
            this->swap(o);
            data_buffer(true).swap(o);
        }
        return *this;
    }
    data_buffer(bool is_temp_obj) : basic_buffer_t(is_temp_obj) {
        // to do nothing
    }

protected:
    void swap(data_buffer &o) {
        std::swap(data_, o.data_);
        std::swap(data_size_, o.data_size_);
    }

public:
    char_t *get_data() { return data_; }
    std::size_t get_data_size() { return data_size_; }
    bool append_byte(const char_t *data, const std::size_t data_size) {
        bool ret = false;
        do {
            if (nullptr == data || 0 == data_size) {
                break;
            }
            if (data_size_ + data_size >= std::numeric_limits<std::size_t>::max()) {
                break;
            }
            if (basic_buffer_t::buffer() != data_) {
                std::memmove(basic_buffer_t::buffer(), data_, data_size_);
                data_ = basic_buffer_t::buffer();
            }
            std::size_t idle_size = basic_buffer_t::buffer_size() - data_size_;
            if (data_size > idle_size) {
                std::size_t new_size = (std::max)(basic_buffer_t::buffer_size() + BUFFER_DEFAULT_SIZE, data_size + data_size_);
                try {
                    std::size_t pre_size = data_ - basic_buffer_t::buffer();
                    this->resize(new_size);
                    data_ = basic_buffer_t::buffer() + pre_size;
                } catch (const std::exception &e) {
                    break;
                }
            }
            std::memcpy(data_ + data_size_, data, data_size);
            data_size_ += data_size;
            ret = true;
        } while (false);
        return ret;
    }
    void test_print_data() {
        char binary[9] = {0};
        for (int i = 0; i < data_size_; ++i) {
            const char &c = data_[i];
            binary[8] = 0;
            binary[7] = (0 != (c & 0x1) ? '1' : '0');
            binary[6] = (0 != (c & 0x2) ? '1' : '0');
            binary[5] = (0 != (c & 0x4) ? '1' : '0');
            binary[4] = (0 != (c & 0x8) ? '1' : '0');
            binary[3] = (0 != (c & 0x16) ? '1' : '0');
            binary[2] = (0 != (c & 0x32) ? '1' : '0');
            binary[1] = (0 != (c & 0x64) ? '1' : '0');
            binary[0] = (0 != (c & 0x128) ? '1' : '0');
            std::cout << binary << " ";
        }
        std::cout << std::endl;
    }
    template <typename number_t>
    static std::size_t get_number_length(const number_t) {
        return sizeof(number_t);
    }
    static std::size_t get_string_length(const std::string &str) {
        return str.size() + 2; //std::uint16_t
    }
};

template <typename char_t>
class write_buffer final : public data_buffer<char_t> {
public:
    typedef basic_buffer<char_t> basic_buffer_t;
    typedef data_buffer<char_t> data_buffer_t;

public:
    write_buffer() {}
    ~write_buffer() {}
    write_buffer(write_buffer &&o) : data_buffer_t(dynamic_cast<data_buffer_t &&>(o)) {}
    write_buffer &operator=(write_buffer &&o) {
        data_buffer_t::operator=(dynamic_cast<data_buffer_t &&>(o));
        return *this;
    }
    write_buffer(bool is_temp_obj) : data_buffer_t(is_temp_obj) {
        // to do nothing
    }

public:
    template <typename number_t>
    bool append_number(const number_t num) {
        bool ret = false;
        do {
            number_t convert_num = 0;
            if (!byte_order_convert<sizeof(number_t)>::convert_v2(num, convert_num)) {
                break;
            }
            if (!data_buffer_t::append_byte((const char *)&convert_num, sizeof(number_t))) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool append_string(const std::string &str) {
        bool ret = false;
        do {
            std::uint16_t str_size = str.size();
            if (!append_number(str_size)) {
                break;
            }
            if (!data_buffer_t::append_byte(str.data(), str_size)) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool append_checksum_and_variable_length(const std::uint32_t vl_num, const char_t checksum) {
        bool ret = false;
        do {
            char_t vl_byte[4] = {0};
            std::size_t vl_byte_size = 0;
            char_t checksum_tmp = checksum;
            if (!get_checksum_and_variable_length(vl_num, vl_byte, vl_byte_size, checksum_tmp)) {
                break;
            }
            if (!data_buffer_t::append_byte(&checksum_tmp, 1)) {
                break;
            }
            if (!data_buffer_t::append_byte(vl_byte, vl_byte_size)) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool get_all_byte(char_t *&data, std::size_t &data_size) {
        bool ret = false;
        do {
            data = data_buffer_t::data_;
            data_size = data_buffer_t::data_size_;
            data_buffer_t::data_ = nullptr;
            data_buffer_t::data_size_ = 0;
            ret = true;
        } while (false);
        return ret;
    }
    bool get_byte(const std::size_t request_data_size, char_t *&data, std::size_t &data_size) {
        bool ret = false;
        do {
            if (data_buffer_t::data_size_ <= request_data_size) {
                ret = get_all_byte(data, data_size);
                break;
            }
            data = data_buffer_t::data_;
            data_size = request_data_size;
            data_buffer_t::data_ = +data_size;
            data_buffer_t::data_size_ -= data_size;
            if (0 == data_buffer_t::data_size_) {
                data_buffer_t::data_ = nullptr;
            }
            ret = true;
        } while (false);
        return ret;
    }
    static bool get_checksum_and_variable_length(const std::uint32_t &vl_num, char_t *vl_byte, std::size_t &vl_byte_size, char_t &code) {
        bool ret = false;
        do {
            std::uint8_t vl_byte_tmp[4] = {0};
            std::uint8_t one_byte = 0;
            std::uint8_t vl_index = 0;
            std::uint32_t vl_num_tmp = vl_num;
            do {
                one_byte = vl_num_tmp % 0x80;
                vl_num_tmp = vl_num_tmp / 0x80;
                if (vl_num_tmp > 0) {
                    one_byte |= 0x80;
                }
                vl_byte_tmp[vl_index] = one_byte;
                code ^= vl_byte_tmp[vl_index];
                ++vl_index;
            } while (vl_num_tmp > 0 && 4 > vl_index);
            std::memcpy(vl_byte, vl_byte_tmp, vl_index);
            if (4 == vl_index) {
                break;
            }
            vl_byte_size = vl_index;
            ret = true;
        } while (false);
        return ret;
    }
};

template <typename char_t>
class read_buffer final : public data_buffer<char_t> {
public:
    typedef data_buffer<char_t> data_buffer_t;

public:
    read_buffer() {}
    virtual ~read_buffer() {}
    read_buffer(read_buffer &&o) : data_buffer_t(dynamic_cast<data_buffer_t &&>(o)) {}
    read_buffer &operator=(read_buffer &&o) {
        data_buffer_t::operator=(dynamic_cast<data_buffer_t &&>(o));
        return *this;
    }
    read_buffer(bool is_temp_obj) : data_buffer_t(is_temp_obj) {
        // to do nothing
    }

public:
    char_t *get_byte(const std::size_t request_size) {
        char_t *ret = nullptr;
        if (data_buffer_t::data_ && data_buffer_t::data_size_ >= request_size) {
            ret = data_buffer_t::data_;
            data_buffer_t::data_ += request_size;
            data_buffer_t::data_size_ -= request_size;
            if (0 == data_buffer_t::data_size_) {
                data_buffer_t::data_ = nullptr;
            }
        }
        return ret;
    }
    template <typename number_t>
    bool get_number(number_t &num) {
        bool ret = false;
        do {
            char_t *byte_num = get_byte(sizeof(number_t));
            if (nullptr == byte_num) {
                break;
            }
            number_t original_num = 0;
            std::memcpy(&original_num, byte_num, sizeof(number_t));
            if (!byte_order_convert<sizeof(number_t)>::convert_v2(original_num, num)) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool get_string(std::string &str) {
        bool ret = false;
        do {
            std::uint16_t str_length = 0;
            if (!get_number(str_length)) {
                break;
            }
            char_t *str_data = get_byte(str_length);
            if (!str_data) {
                break;
            }
            str = std::string(str_data, str_length);
            ret = true;
        } while (false);
        return ret;
    }
    bool get_checksum_and_variable_length(std::uint32_t &vl_num, char_t &checksum) {
        bool ret = false;
        do {
            char_t *checksum_org_ptr = get_byte(1);
            if (nullptr == checksum_org_ptr) {
                break;
            }
            char_t checksum_org = *checksum_org_ptr;
            char_t vl_byte[4] = {0};
            std::size_t vl_byte_size = 0;
            try {
                if (peek_variable_length(data_buffer_t::data_, data_buffer_t::data_size_, vl_byte_size)) {
                    break;
                }
            } catch (const std::exception &e) {
                break;
            }
            std::memcpy(vl_byte, data_buffer_t::data_, vl_byte_size);
            data_buffer_t::data_ += vl_byte_size;
            data_buffer_t::data_size_ -= vl_byte_size;
            if (0 == data_buffer_t::data_size_) {
                data_buffer_t::data_ = nullptr;
            }
            if (!get_checksum_and_variable_length(vl_byte, vl_byte_size, checksum_org, vl_num)) {
                break;
            }
            checksum = checksum_org;
            ret = true;
        } while (false);
        return ret;
    }
    /**
     * @brief peek variable length
     * 
     * @param data 
     * @param data_size 
     * @param vl_byte_size 
     * @return int 0.success 1.fail 2.not enough
     */
    static int peek_variable_length(const char_t *data, const std::size_t data_size, std::size_t &vl_byte_size) {
        int ret = 0; // 0.success 1.fail 2.not enough
        do {
            if (!data || 0 == data_size) {
                break;
            }
            vl_byte_size = 0;
            std::size_t data_size_tmp = data_size;
            const char_t *lv_byte_head = data;
            do {
                if (0 == ((*lv_byte_head) & 0x80)) {
                    ++vl_byte_size;
                    break;
                } else {
                    ++vl_byte_size;
                }
                --data_size_tmp;
                if (0 == data_size_tmp) {
                    if (0 != ((*lv_byte_head) & 0x80)) {
                        if (4 == vl_byte_size || 3 == vl_byte_size) {
                            ret = 1;
                            // throw std::logic_error("The data is too long.");
                        } else {
                            ret = 2;
                            // throw std::logic_error("The data is not enough.");
                        }
                    }
                    break;
                }
                if (4 == vl_byte_size) {
                    ret = 1;
                    // throw std::logic_error("The data is too long.");
                    break;
                }
                ++lv_byte_head;
            } while (0 < data_size_tmp);
        } while (false);
        return ret;
    }
    static bool get_checksum_and_variable_length(const char_t *vl_byte, const std::size_t vl_byte_size, char_t &checksum, std::uint32_t &vl_num) {
        bool ret = false;
        do {
            std::uint8_t one_byte = 0;
            std::uint32_t vl_num_tmp = 0;
            std::uint32_t multiplier = 1;
            int count = 0;
            bool is_ok = true;
            for (int i = 0; i < vl_byte_size && i < 3; ++i) {
                one_byte = vl_byte[i];
                checksum ^= vl_byte[i];
                vl_num_tmp += (one_byte & 0x7f) * multiplier;
                multiplier *= 0x80;
                ++count;
                if (count == vl_byte_size && one_byte & 0x80) {
                    is_ok = false;
                    break;
                }
                if (3 == count && one_byte & 0x80) {
                    is_ok = false;
                    break;
                }
            }
            if (false == is_ok) {
                break;
            }
            vl_num = vl_num_tmp;
            ret = true;
        } while (false);
        return ret;
    }
};

#define READ_BYTE_BUFFER_MAX_SIZE 0x800

class byte_buffer_from_server final {
public:
    struct citylife_im_protocol {
        citylife_im_protocol() : cp_buffer(std::make_shared<read_buffer<char>>()), cp_buffer_size(0), cp_buffer_current_size(0), head(0), checknum(0), vl_num(0), vl_byte_size(0) {}
        ~citylife_im_protocol() {}
        std::shared_ptr<read_buffer<char>> cp_buffer;
        std::size_t cp_buffer_size;
        std::size_t cp_buffer_current_size;
        char head;
        char checknum;
        std::size_t vl_num;
        std::size_t vl_byte_size;
    };

private:
    std::queue<std::shared_ptr<citylife_im_protocol>> _cp_list;
    char _org_buffer[READ_BYTE_BUFFER_MAX_SIZE];

public:
    byte_buffer_from_server(/* args */) { memset(_org_buffer, 0x00, READ_BYTE_BUFFER_MAX_SIZE); }
    ~byte_buffer_from_server() {}

public:
    void push(const std::shared_ptr<citylife_im_protocol> cp) {
        _cp_list.push(cp);
    }
    std::shared_ptr<citylife_im_protocol> front_and_pop() {
        if (_cp_list.size()) {
            auto ret = _cp_list.front();
            if (ret->cp_buffer_current_size != ret->cp_buffer_size) {
                return nullptr;
            }
            _cp_list.pop();
            return ret;
        }
        return nullptr;
    }
    std::shared_ptr<citylife_im_protocol> back() {
        if (_cp_list.size()) {
            auto ret = _cp_list.back();
            return ret;
        }
        return nullptr;
    }
    int read_buffer_from_server(const int sock);
    int read_buffer_from_server(const char *data, const long data_size);
};

#endif /* test_buffer_hpp */
