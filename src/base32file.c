// Copyright 2023 Alexander Fedotov
#include "base32file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "util.h"
#include "base32buffer.h"

// Chunk size must be divisible by 5 to prevent corruption on data "seams"
// on chunk edges. Data is processed in 5 bits per iteration during encoding.
#define CHUNK_SIZE 16*1020
#define BASE32_CHUNK_SIZE ((CHUNK_SIZE * 8) + 4) / 5

int decode_file(FILE* fin, FILE* fout) {
    if (fin == NULL) {
        printf("Input file is NULL. Unable to read from NULL.\n");
        return -1;
    }
    if (fout == NULL) {
        printf("Output file is NULL. Unable to write to NULL.\n");
        return -1;
    }
    char buffer[CHUNK_SIZE];
    if (buffer == NULL) {
        printf("Unable to allocate memory for writing decoded chunks.\n");
        return -2;
    }
    char base32_buffer[BASE32_CHUNK_SIZE];
    if (base32_buffer == NULL) {
        printf("Unable to allocate memory for reading encoded chunks.\n");
        free(buffer);
        return -2;
    }
    size_t bytes_read = BASE32_CHUNK_SIZE;
    size_t bytes_decoded;

    while (bytes_read == BASE32_CHUNK_SIZE) {
        bytes_read = fread(base32_buffer, 1, BASE32_CHUNK_SIZE, fin);
        if (ferror(fin)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            return -3;
        }
        // In case total number of bytes is divisible by chunk size (empty chunk)
        if (bytes_read == 0) {
            break;
        }
        // Empty buffer will not be passed
        bytes_decoded = decode_base32(base32_buffer, bytes_read, buffer);
        if (bytes_decoded == 0) {
            printf("Error decoding chunk.\n");
            return -4;
        }
        fwrite(buffer, 1, bytes_decoded, fout);
        if (ferror(fout)) {
            printf("Error writing to file.\n");
            printf("Error: %s\n", strerror(errno));
            return -5;
        }
    }

    return 0;
}

int encode_file(FILE* fin, FILE* fout) {
    if (fin == NULL) {
        printf("Input file is NULL. Unable to read from NULL.\n");
        return -1;
    }
    if (fout == NULL) {
        printf("Output file is NULL. Unable to write to NULL.\n");
        return -1;
    }
    char buffer[CHUNK_SIZE];
    if (buffer == NULL) {
        printf("Unable to allocate memory for reading chunks.\n");
        return -2;
    }
    char base32_buffer[BASE32_CHUNK_SIZE];
    if (base32_buffer == NULL) {
        printf("Unable to allocate memory for writing encoded chunks.\n");
        free(buffer);
        return -2;
    }
    size_t bytes_read = CHUNK_SIZE;
    size_t bytes_wrote = 0;  // To calculate padding in the end
    size_t bytes_encoded;
    while (bytes_read == CHUNK_SIZE) {
        bytes_read = fread(buffer, 1, CHUNK_SIZE, fin);
        if (ferror(fin)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            return -3;
        }
        if (bytes_read == 0) {
            break;
        }
        bytes_encoded = encode_base32(buffer, bytes_read, base32_buffer);
        if (bytes_encoded == 0) {
            printf("Error encoding chunk.\n");
            return -4;
        }
        bytes_wrote += fwrite(base32_buffer, 1, bytes_encoded, fout);
        if (ferror(fout)) {
            printf("Error writing to file.\n");
            printf("Error: %s\n", strerror(errno));
            return -5;
        }
    }
    uint8_t padding = 8 - (bytes_wrote % 8);
    if (padding != 8) {
        for (uint8_t i = 0; i < padding; i++) {
            fputc('=', fout);
        }
    }

    return 0;
}
