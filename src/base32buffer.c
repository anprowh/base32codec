// Copyright 2023 Alexander Fedotov
#include "base32buffer.h"
#include <inttypes.h>
#include <stdio.h>
#include "util.h"

#define BASE32_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"
#define LOWERCASE_ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define LOWERCASE_ALPHABET_SIZE 26
// Check if alphabet (A,B,C) have sequential encoded representation
#define ALPHABET_SEQUENTIAL \
        'b' == 'a' + 1 && 'c' == 'b' + 1 && 'd' == 'c' + 1 && \
        'e' == 'd' + 1 && 'f' == 'e' + 1 && 'g' == 'f' + 1 && \
        'h' == 'g' + 1 && 'i' == 'h' + 1 && 'j' == 'i' + 1 && \
        'k' == 'j' + 1 && 'l' == 'k' + 1 && 'm' == 'l' + 1 && \
        'n' == 'm' + 1 && 'o' == 'n' + 1 && 'p' == 'o' + 1 && \
        'q' == 'p' + 1 && 'r' == 'q' + 1 && 's' == 'r' + 1 && \
        't' == 's' + 1 && 'u' == 't' + 1 && 'v' == 'u' + 1 && \
        'w' == 'v' + 1 && 'x' == 'w' + 1 && 'y' == 'x' + 1 && \
        'z' == 'y' + 1 && \
        'B' == 'A' + 1 && 'C' == 'B' + 1 && 'D' == 'C' + 1 && \
        'E' == 'D' + 1 && 'F' == 'E' + 1 && 'G' == 'F' + 1 && \
        'H' == 'G' + 1 && 'I' == 'H' + 1 && 'J' == 'I' + 1 && \
        'K' == 'J' + 1 && 'L' == 'K' + 1 && 'M' == 'L' + 1 && \
        'N' == 'M' + 1 && 'O' == 'N' + 1 && 'P' == 'O' + 1 && \
        'Q' == 'P' + 1 && 'R' == 'Q' + 1 && 'S' == 'R' + 1 && \
        'T' == 'S' + 1 && 'U' == 'T' + 1 && 'V' == 'U' + 1 && \
        'W' == 'V' + 1 && 'X' == 'W' + 1 && 'Y' == 'X' + 1 && \
        'Z' == 'Y' + 1 && \
        '3' == '2' + 1 && '4' == '3' + 1 && '5' == '4' + 1 && \
        '6' == '5' + 1 && '7' == '6' + 1


// Get 5 bits from base32 encoded char
// If alphabet is sequential will work faster
char get_byte_from_base32(char c, int* status) {
    #if ALPHABET_SEQUENTIAL
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        }
        if (c >= '2' && c <= '7') {
            return c - '2' + 26;
        }
        if (c >= 'a' && c <= 'z') {
            return c - 'a';
        }
        if (status != NULL) {
            *status = 1;
        }
        return 0;
    #else
        char idx = 0;
        for(; idx < 32; idx++) {
            if (c == BASE32_CHARS[idx]) {
                return idx;
            }
            if (c < LOWERCASE_ALPHABET_SIZE) {
                if (c == LOWERCASE_ALPHABET[idx]) {
                    return idx;
                }
            }
        }
        if (status != NULL) {
            *status = 1;
        }
        return 0;
    #endif
}


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
            // applying mask using & to ensure no extra bits are added
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
    return output_idx;
}

size_t decode_base32(const char* data, size_t size, char* result) {
    size_t num_output_bytes = get_bytes_size(size);
    size_t input_idx = 0;
    size_t output_idx = 0;
    size_t bits_remaining = 0;
    uint32_t buffer = 0;
    int status;
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
            buffer |= get_byte_from_base32(data[input_idx], &status);
            if (status) {
                return 0;
            }
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
