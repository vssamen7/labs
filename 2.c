#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Функция для обмена местами двух элементов в файле
void swap(FILE *file, int pos1, int pos2);
// Функция для сортировки положительных чисел в файле
void sort_positives(FILE *file);

// Функция для проверки корректности ввода
int get_valid_input();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]); //printf("Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "wb+");
    if (!file) {
        perror("Unable to open file"); //perror("Не удалось открыть файл");
        return 1;
    }

    int n, temp;

    // Получение корректного значения для количества чисел
    printf("Enter the number of integers: "); // printf("Введите количество чисел: ");
    n = get_valid_input();

    // Запись чисел в файл
    printf("Enter %d integers:\n", n); // printf("Введите %d чисел:\n", n);
    for (int i = 0; i < n; i++) {
        temp = get_valid_input(); // Используем проверку для каждого числа
        fwrite(&temp, sizeof(int), 1, file);
    }

    // Чтение и вывод чисел из файла
    printf("Data in the file:\n");
    rewind(file);
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        printf("%d ", temp);
    }
    printf("\n");

    // Поиск максимального и минимального значений
    rewind(file);
    int max = INT_MIN, min = INT_MAX, count_max = 0;
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        if (temp > max) {
            max = temp;
            count_max = 1;
        } else if (temp == max) {
            count_max++;
        }
        if (temp < min) {
            min = temp;
        }
    }
    printf("Max value: %d, Count: %d\n", max, count_max); // printf("Максимальное значение: %d, Количество: %d\n", max, count_max);

    // Замена всех максимальных значений на минимальное
    rewind(file);
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        if (temp == max) {
            fseek(file, sizeof(int), SEEK_CUR); // Перемещаем указатель назад
            fwrite(&min, sizeof(int), 1, file);  // Записываем минимальное значение
        }
    }

    // Сортировка положительных чисел в файле
    sort_positives(file);

    // Вывод изменённых данных из файла
    printf("Modified data in the file:\n"); // printf("Изменённые данные в файле:\n");
    rewind(file);
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        printf("%d ", temp);
    }
    printf("\n");

    fclose(file);
    return 0;
}

// Реализация функции проверки ввода
int get_valid_input() {
    int task;
    while (1) {
        char b;
        if (scanf("%d%c", &task, &b) == 2 && b == '\n') {
            break; // Корректный ввод
        } else {
            printf("Invalid input. Please try again: "); // printf("Некорректный ввод. Повторите еще раз: ");
            while (getchar() != '\n'); // Очистка буфера
        }
    }
    return task;
}

// Реализация функции обмена элементов
void swap(FILE *file, int pos1, int pos2) {
    int val1, val2;
    fseek(file, pos1 * sizeof(int), SEEK_SET);
    fread(&val1, sizeof(int), 1, file);
    fseek(file, pos2 * sizeof(int), SEEK_SET);
    fread(&val2, sizeof(int), 1, file);

    fseek(file, pos1 * sizeof(int), SEEK_SET);
    fwrite(&val2, sizeof(int), 1, file);
    fseek(file, pos2 * sizeof(int), SEEK_SET);
    fwrite(&val1, sizeof(int), 1, file);
}

// Реализация функции сортировки положительных чисел
void sort_positives(FILE *file) {
    int temp1, temp2, size;
    rewind(file);

    // Вычисление количества чисел в файле
    fseek(file, 0, SEEK_END);
    size = ftell(file) / sizeof(int);

    // Пузырьковая сортировка только для положительных чисел
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            fseek(file, j * sizeof(int), SEEK_SET);
            fread(&temp1, sizeof(int), 1, file);
            fread(&temp2, sizeof(int), 1, file);
            if (temp1 > 0 && temp2 > 0 && temp1 > temp2) {
                swap(file, j, j + 1);
            }
        }
    }
}