#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 
typedef struct wordD
{
    unsigned char offset;
    unsigned char count;
    unsigned char s;
};
int decompression(char* in, char* out) {
    FILE* input = fopen(in, "rb");
    if (input == NULL) {
        perror("Error opening the input file");
        return 1;
    }
    FILE* output = fopen(out, "w");
    if (output == NULL) {
        perror("Error opening the output file");
        fclose(input);
        return 1;
    }
    wordD* arrayWord = NULL;
    try {
        arrayWord = (wordD*)malloc(sizeof(wordD) * 2100000);
        if (arrayWord == NULL) {
            throw "Memory allocation error for arrayWord";
        }
        int countReads = 0;
        while (fread(&arrayWord[countReads].offset, sizeof(unsigned char), 1, input) == 1) {
            fread(&arrayWord[countReads].count, sizeof(unsigned char), 1, input);
            fread(&arrayWord[countReads].s, sizeof(unsigned char), 1, input);
            countReads++;
        }
        for (int i = 0; i < countReads; i++) {
            if (arrayWord[i].offset == 0) {
                fputc(arrayWord[i].s, output);
            }
            else {
                int startIndex = i - arrayWord[i].offset;
                for (int j = 0; j < arrayWord[i].count; j++) {
                    fputc(arrayWord[startIndex + j].s, output);
                }
            }
        }
        free(arrayWord);
        fclose(input);
        fclose(output);
        return 0;
    }
    catch (const char* error) {
        fprintf(stderr, "%s\n", error);
        free(arrayWord);
        fclose(input);
        fclose(output);
        return 1;
    }
}