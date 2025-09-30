#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1024

typedef struct TStack{
    int value[MAX_SIZE];
    int top;
} TStack;

void CheckString(char *s, int string_len);
int Calculate(char *s, int string_len);

int main() {
    FILE *file = fopen("in.txt", "r");
    char string_in[MAX_SIZE];
    if (fgets(string_in, MAX_SIZE, file) == 0)
        exit(0);
    int string_len = strlen(string_in);
    string_in[--string_len] = 0;
    CheckString(string_in, string_len);
    int res = Calculate(string_in, string_len);
    printf("%d", res);
    fclose(file);
    return EXIT_SUCCESS;
}

void PrintBadInput() {
    printf("syntax error");
    exit(0);
}

void Push(TStack *stack, int val) {
    stack->top++;
    stack->value[stack->top] = val;
}

int IsEmpty(TStack *stack) {
    return stack->top < 0;
}

int Pop(TStack *stack) {
    if (IsEmpty(stack)) PrintBadInput();
    return stack->value[stack->top--];
}

int Peek(TStack *stack) {
    if (IsEmpty(stack)) PrintBadInput();
    return stack->value[stack->top];
}

int IsOperation(char ch) {
    return ch == '+' || ch == '-' || ch == '/' || ch == '*';
}

void CheckString(char *s, int string_len) {
    for(int i = 0; i < string_len; i++)
        if(!IsOperation(s[i]) && !isdigit(s[i]) && s[i] != '(' && s[i] != ')')
            PrintBadInput();
}

void DividedByZero() {
    printf("division by zero");
    exit(0);
}

void ApplyOp(TStack *digits, char op) {
    int digit2 = Pop(digits);
    int digit1 = Pop(digits);
    if(op == '+') {
        Push(digits, digit1 + digit2);
    }
    else if(op == '-') {
        Push(digits, digit1 - digit2);
    }
    else if(op == '*') {
        Push(digits, digit1 * digit2);
    }
    else if(op == '/') {
        if(!digit2)
            DividedByZero();
        Push(digits, digit1 / digit2);
    }
    else PrintBadInput();
}

int GetDigit(char *s, int *top, int string_len) {
    int digit = 0;
    while (*top < string_len && isdigit(s[*top])) {
        digit = digit * 10 + s[*top] - '0';
        ++*top;
    }
    --*top;
    return digit;
}

int Calculate(char *s, int string_len) {
    TStack digits = { .top = -1 };
    TStack ops = { .top = -1 };
    char op_priority[256] = {0};
    op_priority['*'] = 2;
    op_priority['/'] = 2;
    op_priority['+'] = 1;
    op_priority['-'] = 1;
    for (int i = 0; i < string_len; i++) {
        if(s[i] == '(')
            Push(&ops, '(');
        else if(s[i] == ')') {
            if(!i || s[i - 1] == '(')
                PrintBadInput();
            char tmp;
            while((tmp = (char)Pop(&ops)) != '(')
                ApplyOp(&digits, tmp);
        }
        else if(IsOperation(s[i])) {
            while(!IsEmpty(&ops) && op_priority[Peek(&ops)] >= op_priority[(int)s[i]])
                ApplyOp(&digits, (char)Pop(&ops));
            Push(&ops,(int)s[i]);
        }
        else if(isdigit(s[i])) {
            int digit = GetDigit(s, &i, string_len);
            Push(&digits, digit);
        }
    }
    while(!IsEmpty(&ops))
        ApplyOp(&digits, (char)Pop(&ops));
    return Pop(&digits);
}
