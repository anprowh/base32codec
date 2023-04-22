// Copyright 2023 Alexander Fedotov
#include "util.h"

size_t get_base32_size(size_t bytes_size) {
    return ((bytes_size * 8) + 4) / 5;  // Every 5 bits turn into 1 char
}

size_t get_bytes_size(size_t base32_size) {
    return ((base32_size * 5) + 7) / 8;  // Every 1 char turns into 5 bits
}

char get_byte_from_base32(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= '2' && c <= '7') {
        return c - '2' + 26;
    }
    return 0;
}
