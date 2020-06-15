//
//  test_buffer.hpp
//  test_wait_object
//
//  Created by zuoyu on 2020/6/15.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#ifndef test_buffer_hpp
#define test_buffer_hpp

#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#define BUFFER_DEFAULT_SIZE 0x200
#define BUFFER_DEFAULT_MAX_SIZE 0x1fffff

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
        this->move(o);
    }
    basic_buffer &operator=(basic_buffer &&o) {
        if (*this != o) {
            this->move(o);
        }
        return *this;
    }

protected:
    void resize(const std::size_t new_size) {
        this->reallocate(new_size);
    }
    std::size_t size() const { return buf_size_; }
    char_t *data() const { return buf_; }
    void reset() {
        std::memset(buf_, 0, buf_size_);
        buf_size_ = 0;
    }

private:
    void allocate(const std::size_t size) {
        if (size >= BUFFER_DEFAULT_MAX_SIZE) {
            throw std::length_error("The requested memory exceeds the specified length.");
        }
        buf_ = (char_t *)::malloc(size);
        buf_size_ = size;
    }
    void reallocate(const std::size_t new_size) {
        if (new_size >= BUFFER_DEFAULT_MAX_SIZE) {
            throw std::length_error("The requested memory exceeds the specified length.");
        } else {
            if (0 == new_size) {
                this->free();
            } else {
                void *new_buf = std::realloc(buf_, new_size);
                if (nullptr == new_buf) {
                    throw std::bad_alloc();
                }
                buf_ = (char_t *)new_buf;
                buf_size_ = new_size;
            }
        }
    }
    void free() {
        if (buf_) {
            ::free(buf_);
        }
        buf_ = nullptr;
        buf_size_ = 0;
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
    // Disallow copying and assignment.
    basic_buffer(const basic_buffer &) = delete;
    basic_buffer &operator=(const basic_buffer &) = delete;
};

template <std::size_t length>
class byte_order_convert {
public:
    static bool convert(const unsigned char (&input)[length], unsigned char (&output)[length]) {
        if (1 == length) {
            (void)nullptr;
        } else if (2 == length) {
            output[0] = input[1];
            output[1] = input[0];
        } else if (4 == length) {
            output[0] = input[3];
            output[1] = input[2];
            output[2] = input[1];
            output[3] = input[0];
        } else if (8 == length) {
            output[0] = input[7];
            output[1] = input[6];
            output[2] = input[5];
            output[3] = input[4];
            output[4] = input[3];
            output[5] = input[2];
            output[6] = input[1];
            output[7] = input[0];
        } else {
            throw std::logic_error("This length is not supported.");
            return false;
        }
        return true;
    }
    template <typename number_t>
    static bool convert(const number_t &input, number_t &output) {
        unsigned char input_bytes[length] = {0};
        unsigned char output_bytes[length] = {0};
        memcpy(input_bytes, &input, length);
        bool ret = convert(input_bytes, output_bytes);
        if (ret) {
            memcpy(&output, output_bytes, length);
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

template <typename char_t>
class write_buffer : public basic_buffer<char_t> {
private:
    char_t *data_;
    std::size_t data_size_;

public:
    write_buffer() : basic_buffer<char_t>(), data_(nullptr), data_size_(0) { data_ = this->buf_; }
    ~write_buffer() {}

public:
    template <typename number_t>
    bool append_num(const number_t num) {
        bool ret = false;
        do {
            number_t convert_num = 0;
            if (!byte_order_convert<sizeof(number_t)>::convert_v2(num, convert_num)) {
                break;
            }
            if (!append_byte((const char *)&convert_num, sizeof(number_t))) {
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
            if (!append_num(str_size)) {
                break;
            }
            if (!append_byte(str.data(), str_size)) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool append_variable1_length(const std::uint32_t &vl) {
        bool ret = false;
        do {
            uint8_t lenBuf[4] = {0};
            uint8_t digit = 0;
            uint8_t pos = 0;
            std::uint32_t tmp = vl;
            do {
                digit = tmp % 128;
                tmp = tmp / 128;
                if (tmp > 0) {
                    digit |= 0x80;
                }
                lenBuf[pos++] = digit;
            } while (tmp > 0);
            if (!append_byte((const char *)lenBuf, pos)) {
                break;
            }
            ret = true;
        } while (false);
        return ret;
    }
    bool append_byte(const char_t *data, const std::size_t data_size) {
        bool ret = false;
        do {
            if (this->buf_ != data_) {
                memmove(this->buf_, data_, data_size_);
            }
            std::size_t idle_size = this->buf_size_ - data_size_;
            if (data_size > idle_size) {
                std::size_t new_size = (std::max)(this->buf_size_ + BUFFER_DEFAULT_SIZE, data_size + data_size_);
                try {
                    this->resize(new_size);
                } catch (const std::exception &e) {
                    break;
                }
            }
            memcpy(data_ + data_size_, data, data_size);
            data_size_ += data_size;
            ret = true;
        } while (false);
        return ret;
    }
    bool get_byte(char_t *&data, std::size_t &data_size) {
        bool ret = false;
        do {
            data = data_;
            data_size = data_size_;
            data_ = 0;
            data_size_ = 0;
            ret = true;
        } while (false);
        return ret;
    }
};

template <typename char_t>
class read_buffer : public basic_buffer<char_t> {
private:
    char_t *data_;
    std::size_t data_size_;

public:
    read_buffer() : basic_buffer<char_t>(), data_(nullptr), data_size_(0) {}
    virtual ~read_buffer() {}

public:
    bool append_data(const char_t *data, const std::size_t data_size) {
        /**
         * |------|------------|----------|
         */
        bool ret = false;
        do {
            if (this->buf_ != data_) {
                memmove(this->buf_, data_, data_size_);
            }
            std::size_t idle_size = this->buf_size_ - data_size_;
            if (data_size > idle_size) {
                std::size_t new_size = (std::max)(this->buf_size_ + BUFFER_DEFAULT_SIZE, data_size + data_size_);
                try {
                    this->resize(new_size);
                } catch (const std::exception &e) {
                    break;
                }
            }
            memcpy(data_ + data_size_, data, data_size);
            ret = true;
        } while (false);
        return ret;
    }
    char_t *get_byte(const std::size_t request_size) {
        char_t *ret = nullptr;
        if (data_ && data_size_ > request_size) {
            ret = data_;
            data_ += request_size;
            data_size_ -= request_size;
        }
        return ret;
    }
    template <typename number_t>
    bool get_num(number_t &num) {
        bool ret = false;
        do {
            char_t *byte_num = get_byte(sizeof(number_t));
            if (nullptr == byte_num) {
                break;
            }
            number_t original_num = 0;
            memcpy(&original_num, byte_num, sizeof(number_t));
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
            if (!get_num(str_length)) {
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
    bool get_variable1_length(std::uint32_t &vl) {
        // TODO:
        return false;
    }
};

#endif /* test_buffer_hpp */
