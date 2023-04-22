// Copyright 2023 Alexander Fedotov
#include "base32buffer.h"
#include <inttypes.h>
#include <stdio.h>
#include "util.h"

const char BASE32_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

size_t encode_base32(const char* data, size_t data_size, char* result) {
    size_t num_output_chars = get_base32_size(data_size);
    // "Pointers" to data and result buffers that we will go through
    size_t input_idx = 0;
    size_t output_idx = 0;
    // To process data we will gradually add bits to buffer and process them
    size_t bits_remaining = 0;
    uint32_t buffer = 0;
    while (output_idx < num_output_chars) {
        if (bits_remaining < 5) {
            if (input_idx >= data_size) {  // Needed in the end for padding
                buffer <<= (5 - bits_remaining);
                bits_remaining = 5;
                continue;
            }
            buffer <<= 8;
            buffer &= 0xFFFFFF00;
            buffer |= data[input_idx] & 0x000000FF;
            bits_remaining += 8;
            input_idx++;
        }
        result[output_idx] =
                BASE32_CHARS[(buffer >> (bits_remaining - 5)) & 0x1F];
        bits_remaining -= 5;
        output_idx++;
    }
    result[output_idx] = '\0';
    return output_idx;
}

size_t decode_base32(const char* data, size_t size, char* result) {
    size_t num_output_bytes = get_bytes_size(size);
    size_t input_idx = 0;
    size_t output_idx = 0;
    size_t bits_remaining = 0;
    uint32_t buffer = 0;
    while (output_idx < num_output_bytes) {
        if (bits_remaining < 8) {
            if (input_idx >= size) {
                buffer <<= (8 - bits_remaining);
                bits_remaining = 8;
                continue;
            }
            if (data[input_idx] == '=') {
                break;
            }
            buffer <<= 5;
            buffer |= get_byte_from_base32(data[input_idx]);
            bits_remaining += 5;
            input_idx++;
        } else {
            result[output_idx] = (buffer >> (bits_remaining - 8)) & 0xFF;
            bits_remaining -= 8;
            output_idx++;
        }
    }
    return output_idx;
}
