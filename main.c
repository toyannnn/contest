#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"
#include "work.h"
//подключение необходимых библиотек
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
        int params_count = countParams(arg_num, arg);  //вызов функции для подсчета количества параметров
        Param* entered_params = parametersReader(params_count, arg, params_storage_length, params_storage);   //вызов функции для чтения и обработки введенных параметров
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
            switch (command[0]) {  //обработка первого символа введенной команды
            case '-':
                switch (command[1]) {   //обработка второго символа команды
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
        free(entered_params); //освобождение памяти, выделенной для параметров
        return 0;
    }
}
