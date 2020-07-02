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
    int ret = 0; //0.success 1.fail 2.need more 3.eof
    do {
        ssize_t read_data_size = recv(sock, _org_buffer, READ_BYTE_BUFFER_MAX_SIZE, 0);
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
            std::shared_ptr<citylife_protocol> cp_buf = this->back();
            if (nullptr == cp_buf) {
                cp_buf = std::make_shared<citylife_protocol>();
            }
            if (cp_buf->cp_buffer_current_size == cp_buf->cp_data_size && 0 != cp_buf->cp_buffer_current_size) {
                cp_buf = std::make_shared<citylife_protocol>();
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

            if (2 == cp_buf->cp_buffer_current_size) {
                // variable length
                std::size_t vl_byte_size = 0;
                int _ret = read_buffer<char>::peek_variable_length(rd, rds, vl_byte_size);
                if (0 == _ret) {
                    if (!cp_buf->cp_buffer->append_byte(rd, vl_byte_size)) {
                        ret = 1;
                        break;
                    }
                    cp_buf->vl_byte_size = vl_byte_size;
                    char calcu_checknum = cp_buf->head;
                    if (!read_buffer<char>::get_checksum_and_variable_length(rd, cp_buf->vl_byte_size, calcu_checknum, cp_buf->vl_num)) {
                        ret = 1;
                        break;
                    }
                    if (calcu_checknum != cp_buf->checknum) {
                        ret = 1;
                        break;
                    }
                    cp_buf->cp_buffer_size = 2 + cp_buf->vl_byte_size + cp_buf->vl_num;
                    cp_buf->cp_buffer_current_size += vl_byte_size;
                    rd += vl_byte_size;
                    rds -= vl_byte_size;
                } else if (1 == _ret) {
                    ret = 1;
                    break;
                } else if (2 == _ret) {
                    ret = 2;
                    break;
                } else {
                    // TODO:error
                    ret = 1;
                    break;
                }
                if (2 + cp_buf->vl_byte_size == cp_buf->cp_buffer_size) {
                    // TODO: add one citylife_protocol
                }
                if (0 == rds) {
                    ret = 2;
                    break;
                }
            }

            if (2 <= cp_buf->cp_buffer_current_size) {
                if (0 == cp_buf->vl_num) {
                    char vl_byte[4] = {0};
                    char *cp_data = cp_buf->cp_buffer->get_data();
                    std::size_t cp_data_size = cp_buf->cp_buffer->get_data_size();
                    if (5 <= cp_data_size && 2 > cp_data_size) {
                        // cp_data_size = 2 or 3 or 4
                        ret = 1;
                        break;
                    }
                    if (cp_data_size > 2) {
                        memcpy(vl_byte, cp_data, cp_data_size - 2);
                    }
                    if (2 > cp_data_size) {
                        memcpy(vl_byte + cp_data_size - 2, rd, 2 - cp_data_size < rds ? 2 - cp_data_size : rds);
                    }

                    std::size_t vl_byte_size = 0;
                    int _ret = read_buffer<char>::peek_variable_length(rd, rds, vl_byte_size);
                    if (0 == _ret) {
                        if (!cp_buf->cp_buffer->append_byte(rd, vl_byte_size)) {
                            ret = 1;
                            break;
                        }
                        cp_buf->vl_byte_size = vl_byte_size;
                        char calcu_checknum = cp_buf->head;
                        if (!read_buffer<char>::get_checksum_and_variable_length(rd, cp_buf->vl_byte_size, calcu_checknum, cp_buf->vl_num)) {
                            ret = 1;
                            break;
                        }
                        if (calcu_checknum != cp_buf->checknum) {
                            ret = 1;
                            break;
                        }
                        cp_buf->cp_buffer_size = 2 + cp_buf->vl_byte_size + cp_buf->vl_num;
                        cp_buf->cp_buffer_current_size += vl_byte_size;
                        rd += vl_byte_size;
                        rds -= vl_byte_size;
                    } else if (1 == _ret) {
                        ret = 1;
                        break;
                    } else if (2 == _ret) {
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
                char *cp_data = cp_buf->cp_buffer->get_data();
                std::size_t cp_data_size = cp_buf->cp_buffer->get_data_size();
                std::size_t body_data_size = cp_buf->cp_buffer_size - 2 - cp_buf->vl_byte_size;
                std::size_t missing_data_size = cp_buf->cp_buffer_size - cp_data_size;
                if (cp_buf->cp_buffer_size < cp_data_size) {
                    ret = 1;
                    break;
                }
                if (0 == missing_data_size) {
                    // TODO: add one citylife_protocol
                } else {
                    if (rds >= missing_data_size) {
                        if (!cp_buf->cp_buffer->append_byte(rd, missing_data_size)) {
                            ret = 1;
                            break;
                        }
                        rd += missing_data_size;
                        rds -= missing_data_size;
                        // TODO: add one citylife_protocol
                    } else {
                        if (!cp_buf->cp_buffer->append_byte(rd, rds)) {
                            ret = 1;
                            break;
                        }
                        rd += rds;
                        rds = 0;
                        ret = 2;
                        break;
                    }
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
