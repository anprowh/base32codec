// Copyright 2023 Alexander Fedotov
#ifndef _BASE32_UTIL_H
#define _BASE32_UTIL_H

#include <stddef.h>

size_t get_base32_size(size_t bytes_size);
size_t get_bytes_size(size_t base32_size);
char get_byte_from_base32(char c);

#endif  // _BASE32_UTIL_H
