// Copyright 2023 Alexander Fedotov
#include "base32file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "util.h"
#include "base32buffer.h"

#define CHUNK_SIZE 16*1020

int decode_file(FILE* fin, FILE* fout) {
    if (fin == NULL) {
        printf("Input file is NULL. Unable to read from NULL.\n");
    }
    if (fout == NULL) {
        printf("Output file is NULL. Unable to write to NULL.\n");
    }
    size_t chunk_size = CHUNK_SIZE;
    size_t base32_chunk_size = get_base32_size(chunk_size);
    char* buffer = malloc(chunk_size+1);
    if (buffer == NULL) {
        printf("Unable to allocate memory for writing decoded chunks.\n");
        return -1;
    }
    char* base32_buffer = malloc(base32_chunk_size + 1);
    if (base32_buffer == NULL) {
        printf("Unable to allocate memory for reading encoded chunks.\n");
        free(buffer);
        return -1;
    }
    size_t bytes_read = base32_chunk_size;
    size_t bytes_wrote = 0;  // To calculate padding in the end
    size_t bytes_decoded;

    while (bytes_read == base32_chunk_size) {
        // printf("Reading %llu bytes", base32_chunk_size);
        bytes_read = fread(base32_buffer, 1, base32_chunk_size, fin);
        // printf("Read %llu bytes", bytes_read);
        if (ferror(fin)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            return -1;
        }
        bytes_decoded = decode_base32(base32_buffer, bytes_read, buffer);
        if (bytes_decoded == 0) {
            return -2;
        }
        bytes_wrote += fwrite(buffer, 1, bytes_decoded, fout);
        if (ferror(fout)) {
            printf("Error writing to file.\n");
            printf("Error: %s\n", strerror(errno));
            return -1;
        }
    }
    free(buffer);
    free(base32_buffer);
    return 0;
}

int encode_file(FILE* fin, FILE* fout) {
    if (fin == NULL) {
        printf("Input file is NULL. Unable to read from NULL.\n");
    }
    if (fout == NULL) {
        printf("Output file is NULL. Unable to write to NULL.\n");
    }
    size_t chunk_size = CHUNK_SIZE;
    size_t base32_chunk_size = get_base32_size(chunk_size);
    char* buffer = malloc(chunk_size+1);
    if (buffer == NULL) {
        printf("Unable to allocate memory for reading chunks.\n");
        return -1;
    }
    char* base32_buffer = malloc(base32_chunk_size + 1);
    if (base32_buffer == NULL) {
        printf("Unable to allocate memory for writing encoded chunks.\n");
        free(buffer);
        return -1;
    }
    size_t bytes_read = chunk_size;
    size_t bytes_wrote = 0;  // To calculate padding in the end
    size_t bytes_encoded;
    while (bytes_read == chunk_size) {
        bytes_read = fread(buffer, 1, chunk_size, fin);
        if (ferror(fin)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            free(buffer);
            free(base32_buffer);
            break;
        }
        bytes_encoded = encode_base32(buffer, bytes_read, base32_buffer);
        if (bytes_encoded == 0) {
            printf("Error encoding chunk.\n");
            return -2;
        }
        bytes_wrote += fwrite(base32_buffer, 1, bytes_encoded, fout);
        if (ferror(fout)) {
            printf("Error writing to file.\n");
            printf("Error: %s\n", strerror(errno));
            free(buffer);
            free(base32_buffer);
            break;
        }
    }
    uint8_t padding = 8 - (bytes_wrote % 8);
    if (padding != 8) {
        for (uint8_t i = 0; i < padding; i++) {
            fputc('=', fout);
        }
    }

    free(buffer);
    free(base32_buffer);
    return 0;
}
