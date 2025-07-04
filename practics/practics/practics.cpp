#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <limits.h>

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
