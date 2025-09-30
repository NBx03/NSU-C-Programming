#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1024

void MakeShiftTable(unsigned char *substr, int *shift_table);
int SubstrSearch(unsigned char *string_in, unsigned char *substr, int *shift_table, int *pos);

int main(){
    FILE *file = fopen("in.txt", "r");
    unsigned char substr[17];
    if(fscanf(file, "%16[^\n]", substr) == 0) exit(0);
    if (fgetc(file) == 0) exit(0);
    int shift_table[256] = {0};
    MakeShiftTable(substr, shift_table);
    int shift = 0;
    int pos = 0;
    unsigned char string_in[MAX_LEN] = {0};
    while(!feof(file)) {
        int tmp;
        if ((tmp = fread(&string_in[shift], sizeof(char), MAX_LEN - shift - 1, file)) == 0) exit(0);
        string_in[shift + tmp] = '\0';
        shift = SubstrSearch(string_in, substr, shift_table, &pos);
        memmove(string_in, string_in + MAX_LEN - shift - 1, shift);
    }
    fclose(file);
    return EXIT_SUCCESS;
}
void MakeShiftTable(unsigned char *substr, int *shift_table) {
    int len = strlen((char*)substr);
    for (int i = len - 2; i >= 0; i--)
        if (shift_table[(int)substr[i]] == 0)
            shift_table[(int)substr[i]] = len - i - 1;
    for (int i = 0; i < 256; i++) {
        if (shift_table[i] == 0)
            shift_table[i] = len;
    }
}

int SubstrSearch(unsigned char *string_in, unsigned char *substr, int *shift_table, int *pos) {
    int size = strlen((char*)substr);
    int string_len = strlen((char*)string_in);
    int move = 0;
    while (string_len - move >= size){
        int i = size;
        do {
            printf("%d ", *pos + move + i);
            i--;
        } while (string_in[move + i] == substr[i] && i);
        move += shift_table[(int)string_in[move + size - 1]];
    }
    *pos += move;
    return string_len - move;
}
