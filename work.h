#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "lz77.h"
#define MAX_PATH_SIZE 100
typedef struct {
    char* name;
    char* value;
} Param;
int checkingTheFileCreate(char* value, int* codeError, int* resultOfTheCheck) {
    char* extension = strrchr(value, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".zip") == 0) {
            if (fopen(value, "rb") == NULL) {
                *resultOfTheCheck = 1;
                return *resultOfTheCheck;
            }
            else {
                *codeError = 1;
                return *codeError;
            }
        }
        else if (strcmp(extension, ".tar") == 0) {
            if (fopen(value, "rb") == NULL) {
                *resultOfTheCheck = 2;
                return *resultOfTheCheck;
            }
            else {
                *codeError = 2;
                return *codeError;
            }
        }
        else {
            *codeError = 3;
            return *codeError;
        }
    }
    else {
        *codeError = 4;
        return *codeError;
    }
}
int checkingTheFileExtract(char* value, int* codeError, int* resultOfTheCheck) {
    char* extension = strrchr(value, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".zip") == 0) {
            if (fopen(value, "rb") == NULL) {
                *codeError = 1;
                return *codeError;
            }
            else {
                *resultOfTheCheck = 1;
                return *resultOfTheCheck;
            }
        }
        else if (strcmp(extension, ".tar") == 0) {
            if (fopen(value, "rb") == NULL) {
                *codeError = 2;
                return *codeError;
            }
            else {
                *resultOfTheCheck = 2;
                return *resultOfTheCheck;
            }
        }
        else {
            *codeError = 3;
            return *codeError;
        }
    }
    else {
        *codeError = 4;
        return *codeError;
    }
}
int pathExists(char* value) {
    FILE* file = fopen(value, "rb");
    if (file) {
        fclose(file);
        return 1;
    }
    else {
        return 0;
    }
}
int createArchive(char* archiveName, int fileCount, char* fileNames[]) { 
    FILE* archive = fopen(archiveName, "wb");
    if (archive == NULL) {
        fprintf(stderr, "Error opening the archive output file: %s\n", archiveName);
        return -1;
    }
    fwrite("ARC", 1, 3, archive);
    fwrite(&fileCount, sizeof(int), 1, archive);
    for (int i = 0; i < fileCount; i++) {
        FILE* file = fopen(fileNames[i], "rb");
        if (file == NULL) {
            fprintf(stderr, "File opening error: %s\n", fileNames[i]);
            fclose(archive);
            return -1;
        }
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        fwrite(fileNames[i], 1, strlen(fileNames[i]) + 1, archive);
        fwrite(&fileSize, sizeof(long), 1, archive);
        char buffer[1024];
        while (fread(buffer, 1, 1024, file) > 0) {
            fwrite(buffer, 1, 1024, archive);
        }
        fclose(file);
    }
    fclose(archive);
    return 0;
}
void inputFilePath() {
    char option[3];
    char path[100];
    printf("Enter the command (-f to specify the path to the file): ");
    fgets(option, 3, stdin);
    if (strcmp(option, "-f") == 0) {
        printf("Enter the path to the file: ");
        fgets(path, 100, stdin);
        printf("You have entered the path to the file: %s\n", path);
    }
    else {
        printf("Invalid command\n");
    }
}
int viewArchive(char* archiveName) {
    FILE* archive = fopen(archiveName, "rb");
    if (archive == NULL) {
        fprintf(stderr, "Error opening the archive file: %s\n", archiveName);
        return -1;
    }
    char magicNumber[4];
    fread(magicNumber, 1, 3, archive);
    if (strcmp(magicNumber, "ARC") != 0) {
        fprintf(stderr, "Invalid archive format\n");
        fclose(archive);
        return -1;
    }
    int fileCount;
    fread(&fileCount, sizeof(int), 1, archive); 
    printf("Files in the archive:\n");
    for (int i = 0; i < fileCount; i++) {
        char fileName[256];
        fread(fileName, 1, 255, archive);
        long fileSize;
        fread(&fileSize, sizeof(long), 1, archive);
        printf("%s (%ld byte)\n", fileName, fileSize);
    }
    fclose(archive);
    return 0;
}
int deleteFileFromArchive(int ArgsCount, char* Args[]) {
    int f_found = 0;
    int d_found = 0;
    char* file_to_delete = NULL;
        for (int i = 1; i < ArgsCount; i++) {
            if (strcmp(Args[i], "-d") == 0) {
                d_found = 1;
            }
            else if (strcmp(Args[i], "-f") == 0 && d_found) {
                f_found = 1;
                i++;
                if (i >= ArgsCount) {
                    fprintf(stderr, "There is no file name to delete after the parameter -f\n");
                    return -1;
                }
                file_to_delete = Args[i];
            }
        }
    if (d_found && f_found) {
        printf("Deleting the %s file from the archive\n", file_to_delete);
        return 0;
    }
    fprintf(stderr, "Incorrect parameters for deleting a file from the archive\n");
    return -1;
}
int checkRequiredParams(int ArgsCount, char* Args[]) {
    int c_found = 0;
    int a_found = 0;
    int f_found = 0;
    char last_command[4] = ""; 
    for (int i = 1; i < ArgsCount; i++) {
        printf("Argument: %s\n", Args[i]);
        printf("c_found = %d, a_found = %d, f_found = %d\n", c_found, a_found, f_found);
        printf("last_command: %s\n", last_command);
    }
    return 0;
}
unsigned char* compression_level(char* command, unsigned char data[], Code codes[], int size, int codesSize, FILE* outputFile) {
    unsigned char* compressedData = NULL;
    if (strcmp(command, "-s") == 0) {
        printf("select - huffman compression - 1 or another compression level - 2: ");
        int choice;
        scanf_s("%d", &choice);
        if (choice == 1) {
            compressedData = printEncodedStringToFile(data, codes, size, codesSize, outputFile);
        }
        else if (choice == 2) {
            int compressedSize = 0;
            compressedData = compressLZ77(data, size, &compressedSize);
        }
        else {
            printf("Invalid choice\n");
        }
    }
    else {
        printf("Invalid command\n");
    }
    return compressedData;
}
int extracting_arch() {
    char choice;
    printf("Enter your choice (1 or 2): Huffman decoder 1 or another decoder 2 (depending on the encoding method) ");
    scanf(" %c", &choice);
    switch (choice) {
    case '1':
        decodeHuffman(Node * root, FILE * inputFile, FILE * outputFile);
        break;
    case '2':
        decompressLZ77(const LZ77Tuple * input, int inputSize, char* output, int* outputSize);
        break;
    default:
        printf("Invalid choice\n");
    }
    return;
}
void output_arch(int ArgsCount, char* Args[]) {
    char last_command[4] = "";
    char filePath[256];
    char* command;
    for (int i = 1; i < ArgsCount; i++) {
        if (strcmp(Args[i], "-x") == 0 || strcmp(Args[i], "-s") == 0) {
            strcpy(last_command, Args[i]);
        }
        else if (strcmp(Args[i], "-o") == 0 && strlen(last_command) > 0) {
            if (strcmp(last_command, "-x") == 0) {
                printf("Enter the path to the file to save the contents of the function: ");
                scanf("%s", filePath);
                FILE* file = fopen(filePath, "w");
                if (file == NULL) {
                    fprintf(stderr, "File opening errorя: %s\n", filePath);
                    return;
                }
                if (strcmp(last_command, "-x") == 0) {
                    extracting_arch();
                }
                else if (strcmp(last_command, "-s") == 0) {
                    compression_level(command);
                }
                fprintf(file, "%s", command);
                fclose(file);
            }
            else if (strcmp(last_command, "-s") == 0) {
                printf("Enter the path to the file to save the data of the processCommand function: ");
                scanf("%s", filePath);
                FILE* file = fopen(filePath, "w");
                if (file == NULL) {
                    fprintf(stderr, "File opening error: %s\n", filePath);
                    return;
                }
                fprintf(file, "%s", processCommand(command));
                fclose(file);
            }
        }
    }
}
void help(int ArgsCount, char* Args[]) {
    for (int i=0; i< ArgsCount; i++)
    {
        if (strcmp(Args[i], "-h") == 0) {
            printf("Information about the Archiver console application:\n Basic Commands:\n -c or --create – create an archive. \n -x or --extract – extract the archive. \n -l or --list – view the contents of the archive. \n -a or --add – add files to the archive. \n -d or --delete – delete files from the archive. \n -f or --file – specify the path to the archive or files to archive. \n -o or --output is the path to save the archive or extracted files. \n -v or --verbose is a mode with a detailed output of the operation process. \n -s or --size – specifies the compression level (for archiving only). \n If you want to shut down the program, enter 0");
        }
    }
}
void printMenu() {
    printf("Available commands : \n");
    printf(" -c or --create: create archive\n");
    printf(" -x or --extract: extract archive\n");
    printf(" -l or --list: view archive contents\n");
    printf(" -a or --add: add files to the archive\n");
    printf(" -d or --delete: delete files from the archive\n");
    printf(" -f or --file: specify the path to the archive or files to archive\n");
    printf(" -o or --output: path to save the archive or extracted files\n");
    printf(" -v or --verbose: mode with detailed output of the operation process\n");
    printf(" -s or --size: specifying the compression level (for archiving only)\n");
    printf(" -h or --help: calling help\n");
}
void parametersWorker(Param* dataParams, int count) {
    int codeError;
    int resultOfTheCheck;
    char pathToSave[MAX_PATH_SIZE];
    for (int i = 0; i < count; ++i) {
        if (strcmp(dataParams[i].name, "-c") == 0) {
            for (int j = i; j < count; ++j) {
                if (strcmp(dataParams[j].name, "-o") == 0) {
                    strcpy(pathToSave, dataParams[j].value);
                    strcat(pathToSave, dataParams[i].value);
                    if (pathExists(pathToSave)) {
                        checkingTheFileCreate(pathToSave, &codeError, &resultOfTheCheck);
                        if (resultOfTheCheck == 1) {
                            //fileManager(reference data for creating an archive.zip);
                        }
                        else if (resultOfTheCheck == 2) {
                            //fileManager(reference data for creating an archive .tar);
                        }
                        else {
                        }
                    }
                    else {
                    }
                }
                else {
                    strcpy(pathToSave, dataParams[i].value);
                    checkingTheFileCreate(pathToSave, &codeError, &resultOfTheCheck);
                    if (resultOfTheCheck == 1) {
                        //fileManager(reference data for creating an archive .zip);
                    }
                    else if (resultOfTheCheck == 2) {
                        //fileManager(reference data for creating an archive .tar);
                    }
                    else {
                    }
                }
            }
        }
    }
}