#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "work.h"
struct CommandPair {
    char long_form[20];
    char short_form[4];
};
struct CommandPair commands[] = {
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
const char* process_command(const char* input_command) {
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(input_command, commands[i].long_form) == 0 ||
            strcmp(input_command, commands[i].short_form) == 0) {
            return commands[i].short_form;
        }
    }
    return "The wrong command. Type -h or --help for help.";
}
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
typedef struct Param {
    char* name;
    char* value;
} Param;
Param enter_params(char* name, char* value) {
    Param param;
    param.name = name;
    param.value = value;
    return param;
}
char* equalizeParams(char* param, const int params_storage_length, const char* params_storage[]) {
    for (int i = 0; i < params_storage_length; i += 2) {
        if (strcmp(param, params_storage[i]) == 0) {
            return (char*)params_storage[i + 1];
        }
    }
    return param;
}
int countParams(int arg_num, char* arg[]) {
    int params_count = 0;
    for (int i = 1; i < arg_num; i++) {
        if (arg[i][0] == '-') {
            params_count++;
        }
    }
    return params_count;
}
Param* parametersReader(int ArgsCount, char* Args[], const int params_storage_length, const char* params_storage[]) {
    for (int i = 1; i < ArgsCount; i++) {
        if (strncmp(Args[i], "--", 2) == 0) {
            Args[i] = equalizeParams(Args[i], params_storage_length, params_storage);
        }
    }
    int params_count = (ArgsCount - 1) / 2;
    Param* entered_params = (Param*)malloc(params_count * sizeof(Param));
    if (entered_params == NULL) {
        perror("Memory allocation error");
        return NULL;
    }
    for (int i = 0, j = 1; i < params_count; i++, j += 2) {
        entered_params[i] = enter_params(Args[j], Args[j + 1]);
    }
    return entered_params;
}
