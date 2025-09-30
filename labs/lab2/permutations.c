#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Bad_Input(); // Печатает bad input и прекращает программу
int Shift_StringOut(char *str); // Делает перестановку элементов в лексикографическом порядке
void Swap(char *elem1, char *elem2); // Меняет элементы местами


int main(void) {
    char stringIn[100] = "";
    unsigned int num;
    int noRepeats[10] = {0};
    char *string_Check = fgets(stringIn, 99, stdin); // Ввод последовательности символов, состоящая из набора цифр 0123456789 в произвольном порядке
    if (string_Check[0] == '\n')
        Bad_Input();
    if(scanf("%u", &num) != 1) // Ввод числа, означающего, сколько перестановок нужно сделать
        Bad_Input();
    for(unsigned int i = 0; i < strlen(stringIn); i++) { // Проверка
        int numIn = stringIn[i] - '0';
        if (stringIn[i] == '\n') {
            stringIn[i] = 0;
            break;
        }
        if(numIn < 0 || numIn > 9)
            Bad_Input();
        if(noRepeats[numIn]-- < 0)
            Bad_Input();
    }
    unsigned int counter = 0;
    while(counter < num) { // Выполнение N перестановок после StringIn
        counter++;
        if(Shift_StringOut(stringIn) == 0) break;
    }
    return EXIT_SUCCESS;
}
void Bad_Input() { // Печатает bad input и выходит из программы
    printf("bad input");
    exit(0);
}
int Shift_StringOut(char *str) { // Перестановка символов в лексикографическом порядке
    for(unsigned int i = strlen(str) - 1; i > 0; i--) {
        if(str[i-1] < str[i]) {
            unsigned int holder = i;
            for(unsigned int j = holder; j < strlen(str); j++) {
                if(str[j] > str[i-1] && str[i-1] > str[j+1]) {
                    holder = j;
                    break;
                }
            }
            Swap(&str[i-1], &str[holder]);
            for(unsigned int j = strlen(str) - 1; i < j; i++, j--) {
                Swap(&str[i], &str[j]);
            }
            printf("%s\n", str);
            return 1;
        }
    }
    return 0;
}
void Swap(char *elem1, char *elem2) { // Меняет символы местами
    char t = *elem1;
    *elem1 = *elem2;
    *elem2 = t;
}
