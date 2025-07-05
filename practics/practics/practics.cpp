#define _CRT_SECURE_NO_WARNINGS
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

int main() {
    setlocale(LC_ALL, "Russian");

    int* arr = NULL;
    int n = 0;
    int choice;
    clock_t start, end;
    double time_taken;

    do {
        printf("\nМеню:\n");
        printf("1. Ввод чисел вручную\n");
        printf("2. Генерация случайных чисел\n");
        printf("3. Сортировка LSD (по младшим разрядам)\n");
        printf("4. Сортировка MSD (по старшим разрядам)\n");
        printf("5. Вывод массива\n");
        printf("6. Выход\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            manualInput(&arr, &n);
            break;
        case 2:
            randomInput(&arr, &n);
            break;
        case 3:
            if (n == 0) {
                printf("Массив пуст! Сначала введите или сгенерируйте массив.\n");
                break;
            }
            swap_count = 0;
            start = clock();
            radixSortLSD(arr, n);
            end = clock();
            time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Сортировка LSD завершена.\n");
            printf("Время сортировки: %.6f секунд\n", time_taken);
            printf("Количество перестановок: %d\n", swap_count);
            saveToFile(arr, n, "output_lsd.txt");
            break;
        case 4:
            if (n == 0) {
                printf("Массив пуст! Сначала введите или сгенерируйте массив.\n");
                break;
            }
            swap_count = 0;
            start = clock();
            radixSortMSD(arr, n);
            end = clock();
            time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Сортировка MSD завершена.\n");
            printf("Время сортировки: %.6f секунд\n", time_taken);
            printf("Количество перестановок: %d\n", swap_count);
            saveToFile(arr, n, "output_msd.txt");
            break;
        case 5:
            if (n == 0) {
                printf("Массив пуст.\n");
            }
            else {
                printf("Массив:\n");
                printArray(arr, n);
            }
            break;
        case 6:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    } while (choice != 6);

    if (arr != NULL) {
        free(arr);
    }

    return 0;
}