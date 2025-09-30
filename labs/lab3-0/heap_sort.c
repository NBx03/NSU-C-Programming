#include <stdio.h>
#include <stdlib.h>

void HeapSort(int *input_array, int array_size);

int main() {
    int size;
    if (scanf("%d", &size) != 1) exit(0);
    int *arr = malloc(sizeof(int) *size);
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) exit(0);
    }
    HeapSort(arr, size);
    for(int i = 0; i < size; i++) {
        printf("%d\n", arr[i]);
    }
    free(arr);
    return EXIT_SUCCESS;
}

void Swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void Heapify(int *heapNumbers, int heapRoot, int heapBottom) { 
    int maxChild; 
    while(heapRoot * 2 <= heapBottom) {
        if(heapRoot * 2 == heapBottom || heapNumbers[heapRoot * 2] > heapNumbers[heapRoot * 2 + 1])
            maxChild = heapRoot * 2; // Если мы находимся в последнем ряду, то запоминаем левый потомок, в другом случае запоминаем больший потомок из двух
        else maxChild = heapRoot * 2 + 1;
        if(heapNumbers[heapRoot] < heapNumbers[maxChild]) {
            Swap(&heapNumbers[heapRoot], &heapNumbers[maxChild]); // Если верхний элемент меньше максимального потомка, меняем их местами
            heapRoot = maxChild;
        }
        else break;
    }
}

void HeapSort(int *input_array, int array_size) { 
    for(int i = array_size / 2; i >= 0; i--)
        Heapify(input_array, i, array_size - 1);
    for(int i = array_size - 1; i >= 1; i--) { // Проводим через кучу все оставшиеся элементы
        Swap(&input_array[0], &input_array[i]);
        Heapify(input_array, 0, i - 1);
    }
}
