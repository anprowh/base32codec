#include <check.h>
#include <stdio.h>
#include "../src/base32file.h"

int compare_files(FILE* f1, FILE* f2);

// Check that when encoding enmpty file, encoded file is also empty
START_TEST(test_empty_file) {
    // print in case of error to see in which test it happened
    printf("Test empty file\n");
    FILE* fin = fopen("../tests/input/empty.txt", "rb");
    ck_assert_ptr_nonnull(fin);
    FILE* fout = fopen("../tests/output/empty.txt.b32", "wb");
    ck_assert_ptr_nonnull(fout);
    ck_assert_int_eq(encode_file(fin, fout), 0);
    fclose(fin);
    fclose(fout);
    fin = fopen("../tests/output/empty.txt.b32", "r");
    ck_assert_int_eq(fgetc(fin), EOF);
    fclose(fin);
}
END_TEST

// Check that when decoding enmpty file, decoded file is also empty
START_TEST(test_empty_file_decode) {
    printf("Test empty file decode\n");
    FILE* fin = fopen("../tests/input/empty.txt", "rb");
    FILE* fout = fopen("../tests/output/empty.txt.decoded", "wb");
    ck_assert_int_eq(decode_file(fin, fout), 0);
    fclose(fin);
    fclose(fout);
    fin = fopen("../tests/output/empty.txt.decoded", "rb");
    ck_assert_int_eq(fgetc(fin), EOF);
    fclose(fin);
}
END_TEST

// Check that when encoding normal file, encoded file is equal to expected
START_TEST(test_normal_file) {
    printf("Test normal file\n");
    FILE* fin = fopen("../tests/input/normal_file.data", "rb");
    FILE* fout = fopen("../tests/output/normal_file.data.b32", "wb");
    ck_assert_int_eq(encode_file(fin, fout), 0);
    fclose(fin);
    fclose(fout);

    // Compare files
    FILE* f1 = fopen("../tests/output/normal_file.data.b32", "rb");
    FILE* f2 = fopen("../tests/input/normal_file.data.b32", "rb"); 
    ck_assert_int_eq(compare_files(f1, f2), 0);
}
END_TEST

// Check that when decoding normal file, decoded file is equal to expected
START_TEST(test_normal_file_decode) {
    printf("Test normal file decode\n");
    FILE* fin = fopen("../tests/input/normal_file.data.b32", "rb");
    FILE* fout = fopen("../tests/output/normal_file.data.decoded", "wb");
    ck_assert_int_eq(decode_file(fin, fout), 0);
    fclose(fin);
    fclose(fout);

    // Compare files
    FILE* f1 = fopen("../tests/output/normal_file.data.decoded", "rb");
    FILE* f2 = fopen("../tests/input/normal_file.data", "rb"); 
    ck_assert_int_eq(compare_files(f1, f2), 0);
}
END_TEST

// Check that when decoding file with illegal characters, decoding fails
START_TEST(test_illegal_chars_decode) {
    printf("Test illegal chars decode\n");
    FILE* fin = fopen("../tests/input/illegal_symbol.data.b32", "rb");
    FILE* fout = fopen("../tests/output/illegal_symbol.data.decoded", "wb");
    ck_assert_int_eq(decode_file(fin, fout), -4);
    fclose(fin);
    fclose(fout);
}
END_TEST

int main() {

    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_empty_file);
    tcase_add_test(tc1_1, test_empty_file_decode);
    tcase_add_test(tc1_1, test_normal_file);
    tcase_add_test(tc1_1, test_normal_file_decode);
    tcase_add_test(tc1_1, test_illegal_chars_decode);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}


#define CHUNK_SIZE_COMPARE 16*1024
// Compares files in chunks
int compare_files(FILE* f1, FILE* f2) {
    if (f1 == NULL) {
        printf("Input file is NULL. Unable to read from NULL.\n");
        return -1;
    }
    if (f2 == NULL) {
        printf("Output file is NULL. Unable to write to NULL.\n");
        return -1;
    }
    char buffer1[CHUNK_SIZE_COMPARE];
    if (buffer1 == NULL) {
        printf("Unable to allocate memory for reading file.\n");
        return -2;
    }
    char buffer2[CHUNK_SIZE_COMPARE];
    if (buffer2 == NULL) {
        printf("Unable to allocate memory for reading file.\n");
        return -2;
    }
    size_t bytes_read1 = CHUNK_SIZE_COMPARE;
    size_t bytes_read2 = CHUNK_SIZE_COMPARE;
    size_t chunk_num = 0;
    while (bytes_read1 == CHUNK_SIZE_COMPARE && bytes_read2 == CHUNK_SIZE_COMPARE) {
        bytes_read1 = fread(buffer1, 1, CHUNK_SIZE_COMPARE, f1);
        bytes_read2 = fread(buffer2, 1, CHUNK_SIZE_COMPARE, f2);
        if (ferror(f1)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            return -3;
        }
        if (ferror(f2)) {
            printf("Error reading from file.\n");
            printf("Error: %s\n", strerror(errno));
            return -3;
        }
        if (bytes_read1 != bytes_read2) {
            printf("Files are not equal.\n");
            printf("Chunk number: %llu\n", chunk_num);
            printf("Bytes read from file 1: %llu\n", bytes_read1);
            printf("Bytes read from file 2: %llu\n", bytes_read2);
            return -4;
        }
        for (size_t i = 0; i < bytes_read1; i++) {
            if (buffer1[i] != buffer2[i]) {
                printf("Files are not equal.\n");
                printf("Chunk number: %llu\n", chunk_num);
                printf("Byte number: %llu\n", i);
                printf("Byte from file 1: %c\n", buffer1[i]);
                printf("Byte from file 2: %c\n", buffer2[i]);
                return -4;
            }
        }
        chunk_num++;
    }
    return 0;
}