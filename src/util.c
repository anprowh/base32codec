// Copyright 2023 Alexander Fedotov
#include "util.h"

size_t get_base32_size(size_t bytes_size) {
    return ((bytes_size * 8) + 4) / 5;  // Every 5 bits turn into 1 char
}

size_t get_bytes_size(size_t base32_size) {
    return ((base32_size * 5) + 7) / 8;  // Every 1 char turns into 5 bits
}
