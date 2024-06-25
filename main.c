#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"
#include "work.h"
int main(int arg_num, char* arg[]) {
    if (arg_num == 1) {
        puts("no parameters");
        return 0;
    }
    const char* params_storage[] = {
        "-c", "--create",
        "-x", "--extract",
        "-l", "--list",
        "-a", "--add",
        "-d", "--delete",
        "-f", "--file",
        "-o", "--output",
        "-v", "--verbose",
        "-s", "--size",
        "-h", "--help",
    };
        const char* params_storage[] = { /* ... */ };
        const int params_storage_length = 20;
        int params_count = countParams(arg_num, arg);
        Param* entered_params = parametersReader(params_count, arg, params_storage_length, params_storage);
        void printMenu();
        int running = 1;
        while (running) {
            char command[10];
            printf("\nEnter a command: ");
            scanf("%s", command);
            if (strcmp(command, "0") == 0) {
                running = 0;
                continue;
            }
            switch (command[0]) {
            case '-':
                switch (command[1]) {
                case 'c':
                    createArchive(char* archiveName, int fileCount, char* fileNames[]);
                    break;
                case 'f':
                    inputFilePath();
                    break;
                case 'l':
                    viewArchive(char* archiveName);
                    break;
                case 'd':
                    deleteFileFromArchive(int ArgsCount, char* Args[]);
                    break;
                case 'v':
                    checkRequiredParams(int ArgsCount, char* Args[]);
                    break;
                case 's':
                    compression_level(char* command);
                    break;
                case 'x':
                    extracting_arch();
                    break;
                case 'o':
                    output_arch(int ArgsCount, char* Args[]);
                    break;
                case 'h':
                    help(int ArgsCount, char* Args[]);
                    break;
                default:
                    printf("Unknown command\n");
                }
                break;
            default:
                printf("Unknown command format\n");
            }
            printf("Do you want to continue? (1/0): ");
            scanf("%d", &running);
        }
        free(entered_params);
        return 0;
    }
}
