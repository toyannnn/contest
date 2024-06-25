#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 
    typedef struct wordC
    {
        unsigned char offset;
        unsigned char count;
        unsigned char s;
    };
    void buildBadCharTable(const char* pattern, int patternLen, int* badChar) {
        int i;
        for (i = 0; i < 256; i++) {
            badChar[i] = patternLen;
        }
        for (i = 0; i < patternLen - 1; i++) {
            badChar[(unsigned char)pattern[i]] = patternLen - 1 - i;
        }
    }
    int boyerMooreSearch(const char* text, int textLen, const char* pattern, int patternLen) {
        int badChar[256];
        int i;
        buildBadCharTable(pattern, patternLen, badChar);
        int shift = 0;
        while (shift <= (textLen - patternLen)) {
            i = patternLen - 1;
            while (i >= 0 && pattern[i] == text[shift + i]) {
                --i;
            }
            if (i < 0) {
                return shift;
            }
            else {
                shift += badChar[(unsigned char)text[shift + i]] > (patternLen - i) ?
                    badChar[(unsigned char)text[shift + i]] : 1;
            }
        }
        return -1;
    }
    int compression(char* in, char* out) {
        FILE* input = fopen(in, "r");
        if (input == NULL) {
            perror("Error opening the input file");
            return 1;
        }
        FILE* output = fopen(out, "wb");
        if (output == NULL) {
            perror("Error opening the output file");
            fclose(input);
            return 1;
        }
        wordC* arrayWord = NULL;
        char* originalString = NULL;
        char* assistStringCode = NULL;
        char* assistStr = NULL;
        try {
            arrayWord = (wordC*)malloc(sizeof(wordC) * 2100000);
            if (arrayWord == NULL) {
                throw "Memory allocation error for arrayWord";
            }
            originalString = (char*)malloc(sizeof(char) * (2100000 + 1));
            if (originalString == NULL) {
                throw "Memory allocation error for OriginalString";
            }
            assistStringCode = (char*)malloc(sizeof(char) * (11000 + 1));
            if (assistStringCode == NULL) {
                throw "Memory allocation error for assistStringCode";
            }
            assistStr = (char*)malloc(sizeof(char) * (20000 + 1));
            if (assistStr == NULL) {
                throw "Memory allocation error for assistStr";
            }
            fseek(input, 0, SEEK_END);
            long len = ftell(input);
            fseek(input, 0, SEEK_SET);
            if (fread(originalString, sizeof(char), len, input) != len) {
                throw "Error reading the input file";
            }
            originalString[len] = '\0';
            int countWritten = 0;
            int indexAssistStringCode = 0;
            int countWrites = 0;
            char current[256] = { 0 };
            int currentLen = 0;
            while (countWritten < len) {
                current[currentLen++] = originalString[countWritten];
                current[currentLen] = '\0'; 
                int matchIndex = boyerMooreSearch(assistStringCode, indexAssistStringCode, current, currentLen);
                if (matchIndex == -1) {
                    assistStringCode[indexAssistStringCode++] = current[currentLen - 1];
                    arrayWord[countWrites].offset = 0;
                    arrayWord[countWrites].count = 0;
                    arrayWord[countWrites].s = current[currentLen - 1];
                    countWrites++;
                    countWritten++;
                    currentLen = 0;
                }
                else {
                while (countWritten < len) {
                    current[currentLen++] = originalString[countWritten];
                    current[currentLen] = '\0';
                    int matchIndex = boyerMooreSearch(assistStringCode, indexAssistStringCode, current, currentLen);
                    if (matchIndex == -1) {
                        assistStringCode[indexAssistStringCode++] = current[currentLen - 1];
                        arrayWord[countWrites].offset = 0;
                        arrayWord[countWrites].count = 0;
                        arrayWord[countWrites].s = current[currentLen - 1];
                        countWrites++;
                        countWritten++;
                        currentLen = 0;
                    }
                    else {
                        arrayWord[countWrites].offset = indexAssistStringCode - matchIndex;
                        arrayWord[countWrites].count = currentLen;
                        arrayWord[countWrites].s = current[0];
                        countWrites++;
                        strcpy(assistStringCode + indexAssistStringCode, current);
                        indexAssistStringCode += currentLen;
                        countWritten += currentLen;
                        currentLen = 0;
                    }
                }
                }
            }
            for (int i = 0; i < countWrites; i++) {
                fwrite(&arrayWord[i].offset, sizeof(unsigned char), 1, output);
                fwrite(&arrayWord[i].count, sizeof(unsigned char), 1, output);
                fwrite(&arrayWord[i].s, sizeof(unsigned char), 1, output);
            }
            free(arrayWord);
            free(originalString);
            free(assistStringCode);
            free(assistStr);
            fclose(input);
            fclose(output);
            return 0;
        }
        catch (const char* error) {
            fprintf(stderr, "%s\n", error);
            free(arrayWord);
            free(originalString);
            free(assistStringCode);
            free(assistStr);
            fclose(input);
            fclose(output);
            return 1;
        }
}