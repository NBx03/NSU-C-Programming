#include <stdio.h>
#include <stdlib.h>
void QuickSort(int *input_array, int first, int last);
int main() {
    int size;
    if (scanf("%d", &size) != 1) exit(0);
    int *arr = malloc(sizeof(int) *size);
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) exit(0);
    }
    QuickSort(arr, 0, size - 1);
    for(int i = 0; i < size; i++) {
        printf("%d\n", arr[i]);
    }
    free(arr);
    return EXIT_SUCCESS;
}
void QuickSort(int *input_array, int first, int last) {
    if (first < last) {
        int i = first, j = last, x = input_array[(i + j) / 2];
        do {
            while (input_array[i] < x) i++;
            while (input_array[j] > x) j--;
            if (i <= j) {
                int tmp = input_array[i];
                input_array[i] = input_array[j];
                input_array[j] = tmp;
                i++;
                j--;
            }
        } while (i <= j);
        QuickSort(input_array, first, j);
        QuickSort(input_array, i, last);
    }
}
