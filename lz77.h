#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WINDOW_SIZE 4095
#define MAX_LOOKAHEAD_SIZE 15
typedef struct {
    int offset;
    int length;
    char nextChar;
} LZ77Tuple;
int findMatch(const char* buffer, int bufferSize, const char* searchString, int searchLength) {
    for (int i = 0; i <= bufferSize - searchLength; i++) {
        if (strncmp(buffer + i, searchString, searchLength) == 0) {
            return i;
        }
    }
    return -1;
}
LZ77Tuple* compressLZ77(const char* input, int inputSize, int* compressedSize) {
    LZ77Tuple* output = NULL;
    *compressedSize = 0;
    int* outputSize = 0;
    char window[MAX_WINDOW_SIZE];
    int windowStart = 0;
    int windowEnd = 0;
    int i = 0;
    output = (LZ77Tuple*)malloc(sizeof(LZ77Tuple) * inputSize);
    if (output == NULL) {
        printf("Memory allocation error!\n");
        return NULL;
        while (i < inputSize) {
            int longestMatchLength = 0;
            int longestMatchOffset = 0;
            for (int j = 1; j <= MAX_LOOKAHEAD_SIZE && i + j <= inputSize; j++) {
                int matchOffset = findMatch(window + windowStart, windowEnd - windowStart, input + i, j);
                if (matchOffset != -1 && j > longestMatchLength) {
                    longestMatchLength = j;
                    longestMatchOffset = windowEnd - windowStart - matchOffset;
                }
            }
            if (longestMatchLength > 0) {
                output[*outputSize].offset = longestMatchOffset;
                output[*outputSize].length = longestMatchLength;
                output[*outputSize].nextChar = input[i + longestMatchLength];
                (*outputSize)++;
                i += longestMatchLength + 1;
            }
            else {
                output[*outputSize].offset = 0;
                output[*outputSize].length = 0;
                output[*outputSize].nextChar = input[i];
                (*outputSize)++;
                i++;
            }
            for (int j = 0; j <= longestMatchLength; j++) {
                window[windowEnd++] = input[i - longestMatchLength - 1 + j];
                if (windowEnd - windowStart > MAX_WINDOW_SIZE) {
                    windowStart++;
                }
            }
        }
        LZ77Tuple* temp = (LZ77Tuple*)realloc(output, sizeof(LZ77Tuple) * (*compressedSize));
        if (temp == NULL && *compressedSize > 0) {
            printf("Memory allocation error!\n");
            free(output);
            return NULL;
        }
        output = temp;
        return output;
    }
}
        char* decompressLZ77(const LZ77Tuple * compressedData, int compressedSize, int* decompressedSize) {
            char* output = NULL;
            *decompressedSize = 0;
            output = (char*)malloc(sizeof(char) * (MAX_WINDOW_SIZE + MAX_LOOKAHEAD_SIZE));
            if (output == NULL) {
                printf("Memory allocation error!\n");
                return NULL;
            }
            for (int i = 0; i < compressedSize; i++) {
                if (compressedData[i].length > 0) {
                    int copyStart = *decompressedSize - compressedData[i].offset;
                    for (int j = 0; j < compressedData[i].length; j++) {
                        output[*decompressedSize] = output[copyStart + j];
                        (*decompressedSize)++;
                        if (*decompressedSize >= MAX_WINDOW_SIZE + MAX_LOOKAHEAD_SIZE) {
                            printf("Error: Buffer overflow during decompression!\n");
                            free(output);
                            return NULL;
                        }
                    }
                }
                output[*decompressedSize] = compressedData[i].nextChar;
                (*decompressedSize)++;
                if (*decompressedSize >= MAX_WINDOW_SIZE + MAX_LOOKAHEAD_SIZE) {
                    printf("Error: Buffer overflow during decompression!!\n");
                    free(output);
                    return NULL;
                }
            }
            char* temp = (char*)realloc(output, sizeof(char) * (*decompressedSize + 1));
            if (temp == NULL && *decompressedSize > 0) {
                printf("Memory overallocation error!\n");
                free(output);
                return NULL;
            }
            output = temp;
            output[*decompressedSize] = '\0';
            return output;
        }