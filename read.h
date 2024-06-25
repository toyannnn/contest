#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "work.h"
struct CommandPair {  //структура для хранения пар команд (длинная и короткая форма)
    char long_form[20];
    char short_form[4];
};
struct CommandPair commands[] = {  //массив структур, содержащих длинные и короткие формы команд
    {"--create", "-c"},
    {"--extract", "-x"},
    {"--list", "-l"},
    {"--add", "-a"},
    {"--delete", "-d"},
    {"--file", "-f"},
    {"--output", "-o"},
    {"--verbose", "-v"},
    {"--size", "-s"}
};
const char* process_command(const char* input_command) {  //функция для обработки введенной команды и возврата короткой формы команды
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(input_command, commands[i].long_form) == 0 ||
            strcmp(input_command, commands[i].short_form) == 0) {
            return commands[i].short_form;
        }
    }
    return "The wrong command. Type -h or --help for help.";
}
//функция для проверки наличия обязательных параметров
int checkRequiredParams(int ArgsCount, char* Args[]) {
    int c_found = 0;
    int a_found = 0;
    int f_found = 0;
    char last_command[4] = "";
    for (int i = 1; i < ArgsCount; i++) {
        if (strcmp(Args[i], "-c") == 0) {
            c_found = 1;
        }
        else if (strcmp(Args[i], "-a") == 0 && c_found && !f_found) {
            a_found = 1;
            printf("adding an entry\n");
            i++;
            if (i >= ArgsCount) {
                fprintf(stderr, "The file name is missing after the -a parameter\n");
                return -1;
            }
        }
        else if (strcmp(Args[i], "-f") == 0 && a_found) {
            f_found = 1;
        }
        else if (f_found &&
            (strcmp(Args[i], "-x") == 0 ||
                strcmp(Args[i], "-l") == 0 ||
                strcmp(Args[i], "-d") == 0 ||
                strcmp(Args[i], "-s") == 0)) {
            strcpy(last_command, Args[i]);
        }
        else if (strcmp(Args[i], "-o") == 0 && f_found) {
            if (strcmp(last_command, "-x") == 0) {
                extracting_arch();
            }
            else if (strcmp(last_command, "-s") == 0) {
                compression_level(char* command);
            }
        }
        else {
                printf("you want to save a compressed archive or extracted files? write -s or -x respectively\n");
                if (i < ArgsCount - 1) {
                    i++;
                    if (strcmp(Args[i], "-x") == 0) {
                        extracting_arch();
                    }
                    else if (strcmp(Args[i], "-s") == 0) {
                        compression_level(char* command);
                    }
                    else {
                        printf("Error: Expected -x or -s\n");
                        return 0;
                    }
                }
                else {
                    printf("Error: The command is missing\n");
                    return 0;
                }
            }
        }
        else {
            printf("Error: Incorrect command order.\n");
            return 0;
        }
    }
    return 1;
    if (c_found && a_found && f_found) {
        return 1;
    }
    else {
        printf("Error: All required commands (-c -a -f) were not found.\n");
        return 0; 
    }
}

typedef struct Param {  //определение структуры Param для хранения имени и значения параметра
    char* name;
    char* value;
} Param;

Param enter_params(char* name, char* value) {  //функция для создания экземпляра структуры Param с указанным именем и значением
    Param param;
    param.name = name;
    param.value = value;
    return param;
}
char* equalizeParams(char* param, const int params_storage_length, const char* params_storage[]) {  //функция для замены длинной формы команды на короткую форму из заданного массива структур
    for (int i = 0; i < params_storage_length; i += 2) {
        if (strcmp(param, params_storage[i]) == 0) {
            return (char*)params_storage[i + 1];
        }
    }
    return param;
}
int countParams(int arg_num, char* arg[]) {  //функция для подсчета количества параметров, начинающихся с символа '-'
    int params_count = 0;
    for (int i = 1; i < arg_num; i++) {
        if (arg[i][0] == '-') {
            params_count++;
        }
    }
    return params_count;
}
Param* parametersReader(int ArgsCount, char* Args[], const int params_storage_length, const char* params_storage[]) {  //функция для чтения и обработки введенных параметров
    for (int i = 1; i < ArgsCount; i++) {  //цикл по всем введенным аргументам, начиная с 1
        if (strncmp(Args[i], "--", 2) == 0) {  //проверка на совпадение аргумента с "--" в начале
            Args[i] = equalizeParams(Args[i], params_storage_length, params_storage);  //вызов функции для приведения аргументов к стандартному виду
        }
    }
    int params_count = (ArgsCount - 1) / 2;  //вычисление количества параметров на основе числа аргументов
    Param* entered_params = (Param*)malloc(params_count * sizeof(Param));
    if (entered_params == NULL) {
        perror("Memory allocation error");  //вывод ошибки в случае неудачного выделения памяти
        return NULL;
    }
    for (int i = 0, j = 1; i < params_count; i++, j += 2) {
        entered_params[i] = enter_params(Args[j], Args[j + 1]);  //заполнение структур параметров данными из аргументов
    }
    return entered_params;  //возврат заполненного массива структур параметров
}
