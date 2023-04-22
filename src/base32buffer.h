// Copyright 2023 Alexander Fedotov
#ifndef _BASE32BUFFER_H
#define _BASE32BUFFER_H

#include <stddef.h>

size_t encode_base32(const char* data, size_t data_size, char* result);
size_t decode_base32(const char* data, size_t size, char* result);

#endif  // _BASE32BUFFER_H
