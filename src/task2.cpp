/**
 * @file task2.cpp
 * @brief Реализация Задания №2: динамический массив, new/delete, ссылка на указатель.
 * @author Vareshka86
 * @date 2026-10-02
 * @version 0.2
 */

#include "task2.h"
#include "input.h"
#include "random_number.h"

#include <iomanip>
#include <iostream>
#include <string>

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/// Ширина колонки при выводе одного элемента массива (без пробела-разделителя).
constexpr int COLUMN_WIDTH = 6;

/**
 * @brief Заполняет динамический массив случайными числами или с клавиатуры.
 * @details Способ заполнения выбирает пользователь. Случайные числа берутся
 * из отрезка [RANDOM_MIN; RANDOM_MAX], в нём есть и отрицательные числа.
 * @param arr  Указатель на первый элемент массива.
 * @param size Количество элементов в массиве.
 */
void fillDynamicArray(int* arr, int size)
{
    std::cout << "\nКак заполнить массив?\n"
              << "  1 - случайными числами от " << RANDOM_MIN << " до " << RANDOM_MAX << "\n"
              << "  2 - вручную с клавиатуры\n";
    const int choice = readIntInRange("Ваш выбор: ", 1, 2);

    if (choice == 1)
    {
        // По указателю на динамический массив range-based for не работает
        // (размер компилятору неизвестен), поэтому обычный цикл с индексом.
        for (int i = 0; i < size; ++i)
        {
            arr[i] = randomInt(RANDOM_MIN, RANDOM_MAX);
        }
        std::cout << "Массив заполнен случайными числами.\n";
        return;
    }

    std::cout << "Введите " << size << " целых чисел:\n";
    for (int i = 0; i < size; ++i)
    {
        arr[i] = readInt("  arr[" + std::to_string(i) + "] = ");
    }
}

} // namespace

int findFirstNegative(const int* arr, int size)
{
    // К памяти по нулевому указателю обращаться нельзя - сразу «не найдено»
    if (arr == nullptr)
    {
        return -1;
    }

    for (int i = 0; i < size; ++i)
    {
        if (arr[i] < 0)
        {
            return i;
        }
    }
    return -1;
}

void process(int*& arr, int size)
{
    // 3.1. Ищем первый отрицательный элемент
    const int negativeIndex = findFirstNegative(arr, size);
    if (negativeIndex < 0)
    {
        return; // отрицательных нет - массив и указатель не меняются
    }

    // 3.2. Новый массив из элементов arr[0] ... arr[negativeIndex - 1]
    const int newSize = negativeIndex;
    int* newArr = new int[newSize]{};
    for (int i = 0; i < newSize; ++i)
    {
        newArr[i] = arr[i];
    }

    delete[] arr;  // старая память больше не нужна - освобождаем, иначе утечка
    arr = newArr;  // arr - ссылка, поэтому меняется указатель вызывающей стороны
}

void printDynamicArray(const int* arr, int size)
{
    // Проверка на nullptr защищает от обращения к несуществующей памяти
    if (arr == nullptr)
    {
        std::cout << "Указатель равен nullptr: массива нет, к памяти не обращаемся.\n";
        return;
    }
    if (size <= 0)
    {
        std::cout << "Массив пуст (0 элементов).\n";
        return;
    }

    std::cout << "Индекс   :";
    for (int i = 0; i < size; ++i)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << i;
    }
    std::cout << '\n';

    std::cout << "Значение :";
    for (int i = 0; i < size; ++i)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << arr[i];
    }
    std::cout << '\n';
}

void freeArray(int*& arr)
{
    delete[] arr;   // для nullptr ничего не делает, поэтому повторный вызов безопасен
    arr = nullptr;  // указатель больше не хранит адрес освобождённой памяти
}

void runTask2()
{
    std::cout << "\n=== Задание №2. Динамический массив и new/delete ===\n";

    // Шаг 1. Размер массива становится известен только во время работы программы
    const int size = readIntInRange("\nШаг 1. Размер массива N (от 1 до " +
                                        std::to_string(TASK2_MAX_SIZE) + "): ",
                                    1, TASK2_MAX_SIZE);

    // Шаг 2. Выделяем память. Пустые фигурные скобки {} обнуляют все элементы
    int* arr = new int[size]{};
    std::cout << "\nШаг 2. Выделена память: new int[" << size << "]{} - все элементы равны 0.\n";
    printDynamicArray(arr, size);

    // Новый размер массива после process(). Сама process() вернуть его не может:
    // по условию размер передаётся в неё по значению.
    int newSize = size;

    try
    {
        fillDynamicArray(arr, size);
        std::cout << "\nИсходный массив:\n";
        printDynamicArray(arr, size);

        // Шаг 3. Обработка массива функцией process()
        const int negativeIndex = findFirstNegative(arr, size);
        const void* oldAddress = arr; // адрес до process() - чтобы показать, что указатель изменился

        std::cout << "\nШаг 3. process(arr, size): ";
        if (negativeIndex < 0)
        {
            std::cout << "отрицательных элементов нет - массив и указатель не меняются.\n";
        }
        else
        {
            std::cout << "первый отрицательный элемент arr[" << negativeIndex << "] = "
                      << arr[negativeIndex] << ".\n";
            newSize = negativeIndex;
        }

        process(arr, size);

        if (negativeIndex >= 0)
        {
            std::cout << "Создан новый массив (элементов: " << newSize
                      << "), старая память освобождена (delete[]).\n"
                      << "Указатель arr теперь смотрит на новый массив: был " << oldAddress
                      << ", стал " << static_cast<const void*>(arr) << ".\n";
        }

        // Шаг 4. Результат
        std::cout << "\nШаг 4. Результат:\n";
        printDynamicArray(arr, newSize);
    }
    catch (...)
    {
        // Если ввод прервался (readInt выбросил исключение), память всё равно
        // освобождается - утечки нет. Затем исключение передаётся дальше в main.
        freeArray(arr);
        throw;
    }

    // Шаг 5. Освобождаем память и обнуляем указатель
    freeArray(arr);
    std::cout << "\nШаг 5. Память освобождена (delete[] arr), указатель обнулён (arr = nullptr).\n"
              << "Попытка вывести массив по обнулённому указателю:\n";
    printDynamicArray(arr, newSize);
}
