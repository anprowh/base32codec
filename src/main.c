// Copyright 2023 Alexander Fedotov
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "base32file.h"

int main(int argc, char** argv1) {
    wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    // Handle arguments
    if (argc >= 2 && (wcscmp(argv[1], L"-h") == 0 ||
                    wcscmp(argv[1], L"--help") == 0)) {
        wprintf(L"Usage: %ls input.dat output.dat", argv[0]);
        return 0;
    }

    if (argc < 3) {
        printf("Wrong number of arguments. Expected 2, got %i."
               "\nUsage: %ls input.dat output.dat", argc, argv[0]);
        return -1;
    }

    // Choose encode or decode
    uint8_t encode = 1;
    if (argc >= 4) {
        if (wcscmp(argv[3], L"-d") == 0 || wcscmp(argv[3], L"--decode") == 0) {
            encode = 0;
        }
    }

    // Open files
    FILE* fin = _wfopen(argv[1], L"rb");
    if (fin == NULL) {
        wprintf(L"Could not open file %ls for reading.\n", argv[1]);
        printf("Error: %s\n", strerror(errno));
        return -2;
    }

    FILE* fout = _wfopen(argv[2], L"wb");
    if (fout == NULL) {
        wprintf(L"Could not open file %ls for writing.\n", argv[2]);
        printf("Error: %s\n", strerror(errno));
        fclose(fin);
        return -3;
    }

    // Timer to output execution time
    char use_timer = 1;
    clock_t start_time, end_time;
    start_time = clock();
    if (start_time == -1) {
        printf("Unable to get start time. Proceeding without timer.\n");
        use_timer = 0;
    }

    int status;

    // Encode or decode
    if (encode) {
        status = encode_file(fin, fout);
    } else {
        status = decode_file(fin, fout);
    }
    if (status != 0) {
        fclose(fin);
        fclose(fout);
        return status;
    }


    if (use_timer) {
        end_time = clock();

        if (end_time == -1) {
            printf("Unable to get end time. Proceeding without timer.\n");
            use_timer = 0;
        }
    }
    if (use_timer) {
        printf("Program took %.2f s to execute.\n",
                ((double) (end_time - start_time)) / CLOCKS_PER_SEC);
    }

    // Close files
    fclose(fin);
    fclose(fout);

    // Free memory
    free(argv);
    printf("Done.\n");
    return 0;
}
