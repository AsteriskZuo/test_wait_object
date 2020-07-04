//
//  test_buffer.cpp
//  test_wait_object
//
//  Created by zuoyu on 2020/6/15.
//  Copyright © 2020 zuoyu. All rights reserved.
//

#include "test_buffer.hpp"
#include <sys/socket.h>

int byte_buffer_from_server::read_buffer_from_server(const int sock) {
    ssize_t read_data_size = recv(sock, _org_buffer, READ_BYTE_BUFFER_MAX_SIZE, 0);
    return read_buffer_from_server(_org_buffer, read_data_size);
}

int byte_buffer_from_server::read_buffer_from_server(const char *data, const long data_size) {
    int ret = 0; //0.success 1.fail 2.need more 3.eof
    do {
        if (data_size > READ_BYTE_BUFFER_MAX_SIZE) {
            ret = 1;
            break;
        }

        if (_org_buffer != data) {
            std::memcpy(_org_buffer, data, data_size);
        }
        auto read_data_size = data_size;

        if (0 > read_data_size) {
            ret = 1;
            break;
        }
        if (0 == read_data_size) {
            ret = 3;
            break;
        }

        const char *rd = _org_buffer;
        std::size_t rds = read_data_size;
        while (0 < rds) {
            std::shared_ptr<citylife_im_protocol> cp_buf = this->back();
            if (nullptr == cp_buf) {
                cp_buf = std::make_shared<citylife_im_protocol>();
                this->push(cp_buf);
            }
            if (cp_buf->cp_buffer_current_size == cp_buf->cp_buffer_size && 0 != cp_buf->cp_buffer_current_size) {
                cp_buf = std::make_shared<citylife_im_protocol>();
                this->push(cp_buf);
            }

            if (0 == cp_buf->cp_buffer_current_size) {
                // head
                if (!cp_buf->cp_buffer->append_byte(rd, 1)) {
                    ret = 1;
                    break;
                }
                cp_buf->head = *rd;
                ++cp_buf->cp_buffer_current_size;
                rd += 1;
                rds -= 1;
                if (0 == rds) {
                    ret = 2;
                    break;
                }
            }

            if (1 == cp_buf->cp_buffer_current_size) {
                // checknum
                if (!cp_buf->cp_buffer->append_byte(rd, 1)) {
                    ret = 1;
                    break;
                }
                cp_buf->checknum = *rd;
                ++cp_buf->cp_buffer_current_size;
                rd += 1;
                rds -= 1;
                if (0 == rds) {
                    ret = 2;
                    break;
                }
            }

            if (2 <= cp_buf->cp_buffer_current_size) {
                if (0 == cp_buf->vl_byte_size) {
                    char vl_byte_tmp[4] = {0};
                    std::size_t vl_byte_tmp_size = 0;
                    char *readed_data = cp_buf->cp_buffer->get_data();
                    std::size_t readed_data_size = cp_buf->cp_buffer->get_data_size();
                    assert(5 > readed_data_size && 2 <= readed_data_size); // readed_data_size = 2 or 3 or 4

                    std::size_t copy1_size = std::min<std::size_t>(readed_data_size - 2, 3);
                    if (0 < copy1_size) {
                        vl_byte_tmp_size += copy1_size;
                        std::memcpy(vl_byte_tmp, readed_data + 2, copy1_size);
                    }
                    assert(3 > copy1_size); // must be
                    std::size_t copy2_size = std::min<std::size_t>(3 - copy1_size, rds);
                    vl_byte_tmp_size += copy2_size;
                    std::memcpy(vl_byte_tmp + copy1_size, rd, copy2_size);

                    std::size_t vl_byte_size = 0;
                    int _ret = read_buffer<char>::peek_variable_length(vl_byte_tmp, vl_byte_tmp_size, vl_byte_size);
                    if (0 == _ret) {
                        char calcu_checknum = cp_buf->head;
                        if (!read_buffer<char>::get_checksum_and_variable_length(vl_byte_tmp, vl_byte_size, calcu_checknum, (std::uint32_t &)cp_buf->vl_num)) {
                            ret = 1;
                            break;
                        }
                        if (calcu_checknum != cp_buf->checknum) {
                            ret = 1;
                            break;
                        }
                        assert(vl_byte_size > copy1_size);
                        std::size_t need_copy_size = vl_byte_size - copy1_size;
                        if (!cp_buf->cp_buffer->append_byte(rd, need_copy_size)) {
                            ret = 1;
                            break;
                        }
                        cp_buf->vl_byte_size = vl_byte_size;
                        cp_buf->cp_buffer_size = 2 + cp_buf->vl_byte_size + cp_buf->vl_num;
                        cp_buf->cp_buffer_current_size += need_copy_size;
                        rd += need_copy_size;
                        rds -= need_copy_size;
                        if (0 == cp_buf->vl_num) {
                            // TODO: add one citylife_protocol
                            if (0 == rds) {
                                break;
                            }
                            continue;
                        }
                    } else if (1 == _ret) {
                        ret = 1;
                        break;
                    } else if (2 == _ret) {
                        if (!cp_buf->cp_buffer->append_byte(rd, rds)) {
                            ret = 1;
                            break;
                        }
                        cp_buf->cp_buffer_current_size += rds;
                        rd += rds;
                        rds = 0;
                        ret = 2;
                        break;
                    } else {
                        // TODO:error
                        ret = 1;
                        break;
                    }
                    if (0 == rds) {
                        ret = 2;
                        break;
                    }
                }
                char *readed_data = cp_buf->cp_buffer->get_data();
                std::size_t readed_data_size = cp_buf->cp_buffer->get_data_size();
                std::size_t body_data_size = cp_buf->cp_buffer_size - 2 - cp_buf->vl_byte_size;
                std::size_t missing_data_size = cp_buf->cp_buffer_size - readed_data_size;
                assert(cp_buf->cp_buffer_size > readed_data_size);
                assert(0 < missing_data_size);
                if (rds >= missing_data_size) {
                    if (!cp_buf->cp_buffer->append_byte(rd, missing_data_size)) {
                        ret = 1;
                        break;
                    }
                    cp_buf->cp_buffer_current_size += missing_data_size;
                    rd += missing_data_size;
                    rds -= missing_data_size;
                    // TODO: add one citylife_protocol
                    if (0 == rds) {
                        break;
                    }
                    continue;
                } else {
                    if (!cp_buf->cp_buffer->append_byte(rd, rds)) {
                        ret = 1;
                        break;
                    }
                    cp_buf->cp_buffer_current_size += rds;
                    rd += rds;
                    rds = 0;
                    ret = 2;
                    break;
                }
            }
            if (0 == rds) {
                ret = 2;
                break;
            }
        }

        if (0 != ret) {
            break;
        }
    } while (false);
    return ret;
}
