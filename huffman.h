#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct Node {
    unsigned char ch;
    int freq;
    struct Node* left;
    struct Node* right;
} Node;
typedef struct Heap {
    int size;
    Node** nodes;
} Heap;
typedef struct Code {
    unsigned char ch;
    char code[100];
} Code;
Node* newNode(unsigned char ch, int freq) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}
Heap* createHeap(int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->size = 0;
    heap->nodes = (Node**)malloc(capacity * sizeof(Node*));
    return heap;
}
void swapNodes(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}
void heapify(Heap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->nodes[left]->freq < heap->nodes[smallest]->freq) {
        smallest = left;
    }
    if (right < heap->size && heap->nodes[right]->freq < heap->nodes[smallest]->freq) {
        smallest = right;
    }
    if (smallest != idx) {
        swapNodes(&heap->nodes[idx], &heap->nodes[smallest]);
        heapify(heap, smallest);
    }
}
Node* extractMin(Heap* heap) {
    Node* min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return min;
}
void insertHeap(Heap* heap, Node* node) {
    heap->size++;
    int i = heap->size - 1;
    while (i && node->freq < heap->nodes[(i - 1) / 2]->freq) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->nodes[i] = node;
}
Node* buildHuffmanTree(unsigned char data[], int freq[], int size) {
    Heap* heap = createHeap(size);
    for (int i = 0; i < size; ++i) {
        insertHeap(heap, newNode(data[i], freq[i]));
    }
    while (heap->size > 1) {
        Node* left = extractMin(heap);
        Node* right = extractMin(heap);
        Node* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertHeap(heap, top);
    }
    return extractMin(heap);
}
void generateCodes(Node* root, char code[], int index, Code codes[], int* codesIndex) {
    if (root->left) {
        code[index] = '0';
        generateCodes(root->left, code, index + 1, codes, codesIndex);
    }
    if (root->right) {
        code[index] = '1';
        generateCodes(root->right, code, index + 1, codes, codesIndex);
    }
    if (!root->left && !root->right) {
        code[index] = '\0';
        codes[*codesIndex].ch = root->ch;
        strcpy(codes[*codesIndex].code, code);
        (*codesIndex)++;
    }
}
void printEncodedStringToFile(unsigned char data[], Code codes[], int size, int codesSize, FILE* outputFile) {
    FILE* encodedOutput = fopen("encoded_output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: the file is not open!\n");
        return;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < codesSize; j++) {
            if (data[i] == codes[j].ch) {
                fprintf(outputFile, "%s", codes[j].code);
                break;
            }
        }
    }
    fclose(encodedOutput);
    printf("The encoded string is written to a file 'encoded_output.txt '\n");
}
void decodeHuffman(Node* root, FILE* inputFile, FILE* outputFile) {
    Node* current = root;
    char bit;
    if (inputFile == NULL) {
        printf("Error: the file is not open!\n");
        return;
    }
    while ((bit = fgetc(inputFile)) != EOF) {
        if (bit == '0') {
            current = current->left;
        }
        else if (bit == '1') {
            current = current->right;
        }
        if (current->left == NULL && current->right == NULL) {
            fprintf(outputFile, "%c", current->ch);
            current = root;
        }
    }
    printf("The decoded string is written to a file\n");
}
unsigned long long int binaryStringToDecimal(char* binaryString) {
    unsigned long long int decimal = 0;
    int length = strlen(binaryString);

    for (int i = length - 1, j = 0; i >= 0; i--, j++) {
        if (binaryString[i] == '1') {
            decimal += (1ULL << j);
        }
    }
    return decimal;
}