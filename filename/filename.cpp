#include "filename.h"

int FileNameVerify(const char* filename, const char* extention)
{
    size_t filename_len = strlen(filename);

        // Находим позицию последней точки в имени файла (для нахождения расширения)
    const char *last_dot = strrchr(filename, '.');
        // Если точка не найдена или она является последним символом, выводим сообщение об ошибке
    if (last_dot == NULL || last_dot == filename + filename_len - 1) {
        printf("Error: file must have extension .txt\n");
        return 1;
    }

    // Проверяем, что расширение файла - это ".txt"
    if (strcmp(last_dot, extention) != 0) {
        printf("Error: file must have extension .txt\n");
        return 1;
    }

    return 0;
}

void MakeCodeFileName(const char* assm_filename, char* code_filename)
{
    size_t assm_name_len = strlen(assm_filename);

    // Копируем имя входного файла в выходной файл
    strncpy(code_filename, assm_filename, assm_name_len);

    // Добавляем расширение ".out" к имени выходного файла
    strncpy(code_filename + assm_name_len - 4, ".code\0", 6);
}