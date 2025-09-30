#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 256

typedef struct Tree {
    int count;
    unsigned char value;
    struct Tree* left;
    struct Tree* right;
} Tree;

typedef struct QueuePriority {
    Tree* treeNode;
    struct QueuePriority *next;
} PQueue;

void Encode(FILE* in, FILE* out);
void Decode(FILE* in, FILE* out);

int main() {
    FILE* in = fopen("in.txt", "rb");
    FILE* out = fopen("out.txt", "wb");

    char mode;
    if(!(mode = fgetc(in))) {
        fclose(in);
        fclose(out);
        return EXIT_SUCCESS;
    }
    if(mode == 'c')
        Encode(in, out);
    else if(mode == 'd')
        Decode(in, out);

    fclose(in);
    fclose(out);
    return EXIT_SUCCESS;
}

int HasNoChild(Tree* tree) {
    if(tree->left == NULL && tree->right == NULL)
        return 1;
    return 0;
}

void FreeTree(Tree* tree) {
    if(tree->right)
        FreeTree(tree->right);
    if(tree->left)
        FreeTree(tree->left);
    free(tree);
}

PQueue* PushQueue(PQueue* topNode, Tree* tree) {
    if(topNode == NULL) {
        topNode = malloc(sizeof(PQueue));
        topNode->treeNode = malloc(sizeof(Tree));
        memcpy(topNode->treeNode, tree, sizeof(Tree));
        topNode->next = NULL;
    }
    else {
        if(topNode->treeNode->count < tree->count) {
            topNode->next = PushQueue(topNode->next, tree);
        }
        else {
            PQueue* newNode = malloc(sizeof(PQueue));
            newNode->treeNode = malloc(sizeof(Tree));
            memcpy(newNode->treeNode, tree, sizeof(Tree));
            newNode->next = topNode;
            topNode = newNode;
        }
    }
    return topNode;
}

void PopQueue(PQueue** topNode, Tree* subTree) {
    memcpy(subTree, (*topNode)->treeNode, sizeof(Tree));
    PQueue* tempNode = (*topNode)->next;
    free((*topNode)->treeNode);
    free(*topNode);
    *topNode = tempNode;
}

PQueue* GetQueue(Tree* tree, int charNum) {
    PQueue* queue = NULL;
    for(int i = 0; i < charNum; i++)
        if(tree[i].count) {
            queue = PushQueue(queue, &tree[i]);
        }
    return queue;
}

Tree* SortTree(PQueue** queue) {
    while((*queue)->next != NULL) {
        Tree* subTreeLeft = malloc(sizeof(Tree));
        Tree* subTreeRight = malloc(sizeof(Tree));
        PopQueue(queue, subTreeLeft);
        PopQueue(queue, subTreeRight);

        Tree* newTree = malloc(sizeof(Tree));
        newTree->count = subTreeLeft->count + subTreeRight->count;
        newTree->value = 0;
        newTree->left = subTreeLeft;
        newTree->right = subTreeRight;
        *queue = PushQueue(*queue, newTree);
        free(newTree);
    }
    return (*queue)->treeNode;
}

void GetCode(Tree* tree, unsigned long long* codeTable, char* codeLengthTable, char codeLength, unsigned long long code, char bitToWrite) {
    if(bitToWrite == 1) {
        code |= (1llu << (64 - codeLength));
    }
    if(!HasNoChild(tree)) {
        if(tree->left) {
            GetCode(tree->left, codeTable, codeLengthTable, codeLength + 1, code, 0);
        }
        if(tree->right) {
            GetCode(tree->right, codeTable, codeLengthTable, codeLength + 1, code, 1);
        }
    }
    else {
        codeTable[(int)tree->value] = code;
        codeLengthTable[(int)tree->value] = codeLength;
    }
}

int CountExtraZeros(const int frequency_table[ASCII_SIZE], const unsigned char characters[ASCII_SIZE], int charNum, const char* codeLengthTable) {
    int rest = 0;
    for(int i = 0; i < charNum; i++) {
        rest += (frequency_table[(int)characters[i]] * (int)codeLengthTable[characters[i]]);
        rest %= 8;
    }

    rest += charNum * 2; // Формула нахождения длины закодированного дерева
    rest = (8 - (rest % 8)) % 8;
    return rest;
}

void PrintEncodedByte(FILE* out, unsigned char* byte, int* offset) {
    fprintf(out, "%c", *byte);
    *byte = 0;
    *offset = 0;
}

void AddBitToByte(FILE* out, unsigned char* byte, int* offset, int bit) {
    if(bit == 1) {
        *byte |= (1u << (7 - *offset));
    }
    (*offset)++;
    if(*offset == 8)
        PrintEncodedByte(out, byte, offset);
}

void PrintEncodedTree(FILE* out, Tree* tree, unsigned char* byte, int* offset) {
    if(HasNoChild(tree)) {
        AddBitToByte(out, byte, offset, 1);
        for(int i = 0; i < 8; i++) {
            if(tree->value & (1u << (7 - i))) {
                AddBitToByte(out, byte, offset, 1);
            }
            else {
                AddBitToByte(out, byte, offset, 0);
            }
        }
    }
    else {
        AddBitToByte(out, byte, offset, 0);
        if(tree->left)
            PrintEncodedTree(out, tree->left, byte, offset);
        if(tree->right)
            PrintEncodedTree(out, tree->right, byte, offset);
    }
}

void PrintEncodedText(FILE* in, FILE* out, const unsigned long long* codeTable, const char* codeLengthTable, unsigned char* byte, int* offset) {
    unsigned char cur = fgetc(in);
    while(!feof(in)) {
        for(int i = 0; i < codeLengthTable[(int)cur]; i++) {
            if(codeTable[(int)cur] & (1llu << (63 - i)))
                AddBitToByte(out, byte, offset, 1);
            else
                AddBitToByte(out, byte, offset, 0);
        }
        cur = fgetc(in);
    }
}

void Encode(FILE* in, FILE* out) {
    unsigned char cur = fgetc(in);
    if(feof(in)) {
        return;
    }

    int frequency_table[ASCII_SIZE] = {0};
    unsigned char characters[ASCII_SIZE] = {0};
    int charNum = 0;
    while(!feof(in)) {
        if(!frequency_table[(int)cur]) {
            characters[charNum] = cur;
            charNum++;
        }
        frequency_table[(int)cur]++;
        cur = fgetc(in);
    }

    Tree* treeList = malloc(charNum * sizeof(Tree));
    for(int i = 0; i < charNum; i++) {
        treeList[i].count = frequency_table[(int) characters[i]];
        treeList[i].value = characters[i];
        treeList[i].left = NULL;
        treeList[i].right = NULL;
    }
    PQueue* queue = GetQueue(treeList, charNum);
    Tree* sortedTree = SortTree(&queue);
    free(queue);

    unsigned long long codeTable[ASCII_SIZE] = {0};
    char codeLengthTable[ASCII_SIZE] = {0};
    if(HasNoChild(sortedTree)) {
        codeTable[(int)sortedTree->value] = 0;
        codeLengthTable[(int)sortedTree->value] = 1;
    }
    else {
        unsigned long long code = 0;
        GetCode(sortedTree, codeTable, codeLengthTable, 0, code, -1);
    }

    int offset = CountExtraZeros(frequency_table, characters, charNum, codeLengthTable); // Подсчёт незначащих нулей в конце
    fseek(in, 1l, SEEK_SET);

    unsigned char byte = 0;
    AddBitToByte(out, &byte, &offset, 1); // Перенос незначащих нулей в начало, для удобства вычисления при раскодировке

    PrintEncodedTree(out, sortedTree, &byte, &offset);
    FreeTree(sortedTree);
    free(treeList);

    PrintEncodedText(in, out, codeTable, codeLengthTable, &byte, &offset);
}

void ReadNewByte(FILE* in, unsigned char* byte, int* offset) {
    *byte = fgetc(in);
    *offset = 0;
}

Tree *GenerateTree(FILE* in, unsigned char *byte, int *offset) {
    if (*byte & (1u << (7 - *offset))) {
        (*offset)++;
        if(*offset == 8)
            ReadNewByte(in, byte, offset);
        unsigned char value = 0;
        value |= (*byte << *offset);
        *byte = fgetc(in);
        value |= (*byte >> (8 - *offset));
        Tree *tree = malloc(sizeof(Tree));
        tree->count = 0;
        tree->value = value;
        tree->left = NULL;
        tree->right = NULL;
        return tree;
    }
    else {
        (*offset)++;
        if(*offset == 8)
            ReadNewByte(in, byte, offset);
        Tree* left = GenerateTree(in, byte, offset);
        Tree* right = GenerateTree(in, byte, offset);

        Tree *tree = malloc(sizeof(Tree));
        tree->count = 0;
        tree->value = 0;
        tree->left = left;
        tree->right = right;
        return tree;
    }
}

void PrintDecodedText(FILE* in, FILE* out, Tree* tree, unsigned char* byte, int* offset) {
    Tree* tempTree = tree;
    while(!feof(in)) {
        if(*offset == 8) {
            ReadNewByte(in, byte, offset);
            if(feof(in))
                break;
        }

        if(*byte & (1u << (7 - *offset))) {
            tempTree = tempTree->right;
        }
        else if(tempTree->left){
            tempTree = tempTree->left;
        }
        if(HasNoChild(tempTree)) {
            fprintf(out, "%c", tempTree->value);
            tempTree = tree;
        }
        (*offset)++;
    }
}

void Decode(FILE* in, FILE* out) {
    unsigned char byte = fgetc(in);
    if(feof(in))
        return;

    int offset = 0;
    while(!(byte & (1u << (8 - offset)))) {
        offset++;
    }
    if(offset == 8)
        ReadNewByte(in, &byte, &offset);

    Tree* tree = GenerateTree(in, &byte, &offset);
    PrintDecodedText(in, out, tree, &byte, &offset);
    FreeTree(tree);
}
