﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <limits.h>

void saveToFile(int* arr, int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи!\n");
        return;
    }
    fprintf(file, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
    }
    fprintf(file, "\n");
    fclose(file);
    printf("Массив сохранён в файл: %s\n", filename);
}

// LSD Radix Sort
void radixSortLSD(int* arr, int n) {
    int max = getMax(arr, n);
    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSortLSD(arr, n, exp);
    }
}

// MSD Radix Sort
void radixSortMSD(int* arr, int n) {
    int max = getMax(arr, n);
    int exp = 1;
    while (max / exp > 0) {
        exp *= 10;
    }
    exp /= 10;
    radixSortMSDHelper(arr, 0, n - 1, exp);
}
void manualInput(int** arr, int* n) {
    printf("Введите количество элементов: ");
    scanf("%d", n);
    *arr = (int*)malloc(*n * sizeof(int));
    printf("Введите %d чисел:\n", *n);
    for (int i = 0; i < *n; i++) {
        scanf("%d", &(*arr)[i]);
    }
    saveToFile(*arr, *n, "input.txt");
}

void randomInput(int** arr, int* n) {
    int min, max;
    printf("Введите количество элементов: ");
    scanf("%d", n);
    printf("Введите минимальное значение: ");
    scanf("%d", &min);
    printf("Введите максимальное значение: ");
    scanf("%d", &max);
    *arr = (int*)malloc(*n * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < *n; i++) {
        (*arr)[i] = rand() % (max - min + 1) + min;
    }
    saveToFile(*arr, *n, "input.txt");
}

