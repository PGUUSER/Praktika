﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <limits.h>

// Глобальные переменные для подсчёта перестановок
int swap_count = 0;

// Функция для сохранения массива в файл
void saveToFile(int* arr, int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи!\n");
        return;
    }

    fprintf(file, "%d\n", n); // Сохраняем количество элементов
    for (int i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
    }
    fprintf(file, "\n");

    fclose(file);
    printf("Массив сохранён в файл: %s\n", filename);
}

// Функция для получения максимального элемента в массиве
int getMax(int* arr, int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Вспомогательная функция для LSD Radix Sort (сортировка подсчётом)
void countingSortLSD(int* arr, int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));
    int count[10] = { 0 };

    // Подсчёт количества элементов для каждой цифры
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // Изменение count[i] так, чтобы он содержал позицию
    // следующего элемента с такой же цифрой
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Построение выходного массива
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        swap_count++; // Увеличиваем счётчик перестановок
    }

    // Копирование выходного массива в arr[]
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    free(output);
}

// LSD Radix Sort
void radixSortLSD(int* arr, int n) {
    int max = getMax(arr, n);

    // Применяем сортировку подсчётом для каждой цифры
    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSortLSD(arr, n, exp);
    }
}

// Вспомогательная функция для MSD Radix Sort (рекурсивная)
void radixSortMSDHelper(int* arr, int low, int high, int exp) {
    if (high <= low || exp == 0) {
        return;
    }

    int count[10] = { 0 };
    int* output = (int*)malloc((high - low + 1) * sizeof(int));

    // Подсчёт количества элементов для каждой цифры
    for (int i = low; i <= high; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // Преобразование count в индексы
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Заполнение выходного массива
    for (int i = high; i >= low; i--) {
        output[--count[(arr[i] / exp) % 10]] = arr[i];
        swap_count++; // Увеличиваем счётчик перестановок
    }

    // Копирование обратно в исходный массив
    for (int i = low; i <= high; i++) {
        arr[i] = output[i - low];
    }

    free(output);

    // Рекурсивная сортировка для каждой цифры
    for (int i = 0; i < 9; i++) {
        radixSortMSDHelper(arr, low + count[i], low + count[i + 1] - 1, exp / 10);
    }
}

// MSD Radix Sort
void radixSortMSD(int* arr, int n) {
    int max = getMax(arr, n);
    int exp = 1;
    while (max / exp > 0) {
        exp *= 10;
    }
    exp /= 10; // Находим максимальный разряд

    radixSortMSDHelper(arr, 0, n - 1, exp);
}

// Функция для вывода массива
void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Функция для ввода массива вручную
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

// Функция для генерации случайного массива
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
