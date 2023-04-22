// Copyright 2023 Alexander Fedotov
#ifndef _BASE32FILE_H
#define _BASE32FILE_H

#include <stdio.h>

int decode_file(FILE* fin, FILE* fout);
int encode_file(FILE* fin, FILE* fout);

#endif  // _BASE32FILE_H
