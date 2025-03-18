#include <stdio.h>
#include <stdlib.h>

// Функция для обмена местами двух элементов в файле
void swap(FILE *file, int pos1, int pos2);
// Функция для сортировки положительных чисел в файле
void sort_positives(FILE *file);

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
    printf("Enter the number of integers: "); // printf("Введите количество чисел: ");
    scanf("%d", &n);

    // Запись чисел в файл
    printf("Enter %d integers: ", n); //printf("Введите %d чисел: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &temp);
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
    printf("Max value: %d, Count: %d\n", max, count_max);//printf("Максимальное значение: %d, Количество: %d\n", max, count_max);

    // Замена всех максимальных значений на минимальное
    rewind(file);
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        if (temp == max) {
            fseek(file, sizeof(int), SEEK_CUR);
            fwrite(&min, sizeof(int), 1, file);
        }
    }

     // Сортировка положительных чисел в файле
    sort_positives(file);

    // Вывод изменённых данных из файла
    printf("Modified data in the file:\n");//printf("Изменённые данные в файле:\n");
    rewind(file);
    for (int i = 0; i < n; i++) {
        fread(&temp, sizeof(int), 1, file);
        printf("%d ", temp);
    }
    printf("\n");

    fclose(file);
    return 0;
}

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