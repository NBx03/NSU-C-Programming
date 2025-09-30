#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define limit 12

unsigned long long Power(int x, int y); // Функция возведения в степень
void Swap(char *s); // Функция переворачивания символов, начиная с конца
int Convert_in_number(char s); // Превращает символ в целое число
char Convert_in_string(int d); // Превращает целое число в символ
void Unite(char *str, char c); // Объединяет строку и символ

int main(void) {
    char stringIn[100] = "", wholePartIn[100] = "", fracPartIn[100] = "", wholePartOut[100] = "", fracPartOut[100] = "", stringOut[100];
    int b1, b2, counter_num = 0, counter_dot = 0, holder = 0, rememberer = 0;

    if(scanf("%d %d", &b1, &b2) != 2)
        exit(0); // Ввод исходной и конечной систем счисления
    if(scanf("%49s", stringIn) != 1)
        exit(0); // Ввод b1-ричных цифр
    // printf("%i %i %s\n", b1, b2, s);

    if(b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
        printf("bad input");
        exit(0);
    } // Проверка на то, являются ли значения b1 и b2 между 2 и 16

    for (unsigned int i = 0; i < strlen(stringIn); i++) {
        if (Convert_in_number(stringIn[i]) >= b1 && stringIn[i] != '.') {
            printf("bad input");
            exit(0);
        } // Проверка на то, b1-ричная ли запись числа во второй строке
        else if (stringIn[i] != '.')
            counter_num += 1;
        else
            counter_dot += 1;
    }
    if(counter_num > limit || counter_dot > 1) {
        printf("bad input");
        exit(0);
    } // Проверка на то, 12 ли цифр во второй строке

    if(stringIn[0] == '.' || stringIn[strlen(stringIn) - 1] == '.') {
        printf("bad input");
        exit(0);
    }
    else {
        for(unsigned int i = 0; i < strlen(stringIn); i++) {
            if (stringIn[i] != '.') {
                wholePartIn[i] = stringIn[i];
                holder = i;
            } else {
                holder += 2;
                rememberer = 1;
                break;
            }
        }
    } // В этой части я распределил целую часть (до точки) в отдельный массив

    if(rememberer == 1) {
        for(unsigned int i = 0; i < strlen(stringIn); i++) {
            if (holder <= 12) {
                fracPartIn[i] = stringIn[holder];
                holder += 1;
            } else
                break;
        }
    } // В этой части я распределил дробную часть (после точки) в другой массив (если она есть)

    unsigned long long nWhole10 = 0;
    for(unsigned int i = 0; i < strlen(wholePartIn); i++) {
        nWhole10 += Convert_in_number(wholePartIn[i]) * Power(b1,strlen(wholePartIn) - i - 1);
    } // Здесь я перевёл целую часть из b1-ричной с.с. в промежуточное число

    unsigned long long nFrac10 = 0, dFrac10 = 1;
    for(unsigned int i = 0; i < strlen(fracPartIn); i++) {
        unsigned long long cn = Convert_in_number(fracPartIn[i]);
        dFrac10 *= b1;
        nFrac10 = nFrac10 * b1 + cn;
    }
    // Здесь я перевёл дробную часть из b1-ричной с.с. в промежуточное число


    int i = 0;
    while(nWhole10 > 0) {
        wholePartOut[i] = Convert_in_string(nWhole10 % b2);
        nWhole10 /= b2;
        i++;
    }
    Swap(wholePartOut);
    // Здесь я перевёл целую часть из промежуточного числа в b2-ричную

    i = 0;
    while(nFrac10 > 0) {
        unsigned long c = nFrac10 * b2 / dFrac10;
        nFrac10 = nFrac10 * b2 - c * dFrac10;
        fracPartOut[i] = Convert_in_string(c);
        i++;
        if(i > 11)
            break;
    }
    // Здесь я перевёл дробную часть из промежуточного числа в b2-ричную
    for(unsigned int k = 0; i < limit; i++) {
        if(fracPartOut[k] != '\0')
            continue;
        else
            fracPartOut[k] = '0';
    }

    if(strlen(fracPartOut) > 0) {
        if(strlen(wholePartOut) == 0)
            Unite(wholePartOut, '0');
        Unite(wholePartOut, '.'); // Соединил точку с целой частью
    }
    strcat(wholePartOut, fracPartOut); // Соединил целую часть с точкой и дробную часть
    strcpy(stringOut, wholePartOut); // Скопировал всё в отдельный массив для красоты, чтобы не держать выводимое значение в рабочем массиве

    printf("%s\n", stringOut); // Вывод!!!
    return EXIT_SUCCESS;
}
unsigned long long Power(int x, int y) {
    unsigned long long res = 1;
    for(int i = 1; i <= y; i++) {
        res *= x;
    }
    return res;
} // Возведение в степень (замена pow() из math.h)

void Swap(char *s) {
    int length = strlen(s);
    for (int i = 0, j = length - 1, c; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
} // Переворачивание символов (подглядел в книге Кернигана и Ритчи)

int Convert_in_number(char ch) {
    if (tolower(ch) >= 'a' && tolower(ch) <= 'f') {
        return tolower(ch) - 'a' + 10;
    }
    else if (ch >= '0' && ch <= '9') {
        return tolower(ch) - '0';
    }
    else {
        return 16;
    }
} // Перевод из символа в целое число

char Convert_in_string(int d) {
    if (d >= 10 && d <= 15) {
        return d + 'a' - 10;
    }
    else if (d >= 0 && d <= 9) {
        return d + '0';
    }
    else {
        return 0;
    }
} // Перевод из целого числа в символ

void Unite(char *str, char c) {
    char tmp[2] = { c , 0 };
    strcat(str, tmp);
} // Объединение строки и одного отдельного символа в одну строку
