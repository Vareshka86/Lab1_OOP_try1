/**
 * @file task1.cpp
 * @brief Реализация Задания №1: функции работы со статическим массивом.
 * @author Vareshka86
 * @date 2026-10-05
 * @version 1.0.1
 */

#include "task1.h"
#include "input.h"
#include "random_number.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/// Ширина колонки при выводе одного элемента массива (без пробела-разделителя).
constexpr int COLUMN_WIDTH = 6;

/**
 * @brief Спрашивает у пользователя, как заполнить массив.
 * @return FillMode::Random или FillMode::Keyboard.
 */
FillMode askFillMode()
{
    std::cout << "\nКак заполнить массив?\n"
              << "  1 - случайными числами от " << RANDOM_MIN << " до " << RANDOM_MAX << "\n"
              << "  2 - вручную с клавиатуры\n";

    const int choice = readIntInRange("Ваш выбор: ", 1, 2);
    return (choice == 1) ? FillMode::Random : FillMode::Keyboard;
}

/**
 * @brief Выводит меню действий Задания №1.
 */
void printTask1Menu()
{
    std::cout << "\nДействия с массивом:\n"
              << "  1 - вывести массив            (printArray)\n"
              << "  2 - поменять местами элементы (swapElements)\n"
              << "  3 - умножить элементы на 2    (multiplyByTwo)\n"
              << "  4 - заполнить массив заново   (fillArray)\n"
              << "  0 - вернуться в главное меню\n";
}

} // namespace

void fillArray(IntArray& arr, FillMode mode)
{
    if (mode == FillMode::Random)
    {
        // int& - ссылка на элемент: присваивание записывает число прямо в массив.
        // Случайные числа даёт общий модуль random_number (randomInt).
        for (int& element : arr)
        {
            element = randomInt(RANDOM_MIN, RANDOM_MAX);
        }
        std::cout << "Массив заполнен случайными числами.\n";
        return;
    }

    // Ручной ввод: здесь нужен номер элемента для подсказки,
    // поэтому используется обычный цикл со счётчиком.
    std::cout << "Введите " << ARRAY_SIZE << " целых чисел:\n";
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        arr[i] = readInt("  arr[" + std::to_string(i) + "] = ");
    }
}

void printArray(const IntArray& arr)
{
    // Строка с индексами - чтобы пользователю было удобно выбирать
    // элементы для swapElements().
    // Перед каждым числом выводится пробел: даже если число длиннее
    // колонки (например, 2000000000), оно не «слипнется» с соседним.
    std::cout << "Индекс   :";
    for (std::size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << i;
    }
    std::cout << '\n';

    // Строка со значениями: range-based for + auto + константная ссылка.
    // Тип value выводится компилятором как const int&.
    std::cout << "Значение :";
    for (const auto& value : arr)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << value;
    }
    std::cout << '\n';
}

void swapValues(int& a, int& b)
{
    // Никаких * и & при работе: a и b - это сами переменные вызывающей стороны
    const int temp = a;
    a = b;
    b = temp;
}

void swapElements(IntArray& arr, int firstIndex, int secondIndex)
{
    // arr - ссылка (IntArray&): это сам массив numbers из runTask1(), не копия.

    // Проверка границ: индекс должен быть в диапазоне [0; ARRAY_SIZE - 1].
    // ARRAY_SIZE приводится к int, чтобы не сравнивать знаковое с беззнаковым.
    const int size = static_cast<int>(ARRAY_SIZE);
    if (firstIndex < 0 || firstIndex >= size || secondIndex < 0 || secondIndex >= size)
    {
        std::cout << "Ошибка: индексы должны быть от 0 до " << size - 1
                  << ". Массив не изменён.\n";
        return;
    }

    // Обмен элемента с самим собой ничего не меняет - сообщаем об этом
    if (firstIndex == secondIndex)
    {
        std::cout << "Индексы совпадают - менять местами нечего. Массив не изменён.\n";
        return;
    }

    // Элементы массива передаются в swapValues по ссылке,
    // поэтому меняются местами прямо внутри arr.
    swapValues(arr[firstIndex], arr[secondIndex]);
    std::cout << "Элементы arr[" << firstIndex << "] и arr[" << secondIndex
              << "] поменялись местами.\n";
}

void multiplyByTwo(IntArray& arr)
{
    // Безопасные границы: если x больше MAX/2 (или меньше MIN/2),
    // то 2 * x не помещается в int.
    constexpr int upperLimit = std::numeric_limits<int>::max() / 2;
    constexpr int lowerLimit = std::numeric_limits<int>::min() / 2;

    // Сначала только проверяем (const auto& - читаем без копирования)...
    for (const auto& x : arr)
    {
        if (x > upperLimit || x < lowerLimit)
        {
            std::cout << "Ошибка: элемент " << x << " при умножении на 2 "
                      << "выйдет за пределы int. Массив не изменён.\n";
            return;
        }
    }

    // ...и только потом изменяем. int& x - неконстантная ссылка на элемент,
    // поэтому x *= 2 умножает сам элемент массива, а не его копию.
    for (int& x : arr)
    {
        x *= 2;
    }
    std::cout << "Все элементы умножены на 2.\n";
}

void runTask1()
{
    // Статический массив: размер ARRAY_SIZE известен на этапе компиляции
    // (в отличие от динамического массива new int[N] из Задания №2).
    // Пустые фигурные скобки {} обнуляют все элементы.
    int numbers[ARRAY_SIZE]{}; // тип этого массива и есть IntArray

    std::cout << "\n=== Задание №1. Статический массив и ссылки ===\n";

    fillArray(numbers, askFillMode());
    printArray(numbers);

    const int maxIndex = static_cast<int>(ARRAY_SIZE) - 1;
    const std::string indexRange = "(0-" + std::to_string(maxIndex) + "): ";

    while (true)
    {
        printTask1Menu();
        const int choice = readIntInRange("Ваш выбор: ", 0, 4);

        switch (choice)
        {
        case 1:
            printArray(numbers);
            break;

        case 2:
        {
            const int first = readIntInRange("Индекс первого элемента " + indexRange, 0, maxIndex);
            const int second = readIntInRange("Индекс второго элемента " + indexRange, 0, maxIndex);
            swapElements(numbers, first, second);
            printArray(numbers);
            break;
        }

        case 3:
            multiplyByTwo(numbers);
            printArray(numbers);
            break;

        case 4:
            fillArray(numbers, askFillMode());
            printArray(numbers);
            break;

        default: // 0 - выход в главное меню
            return;
        }
    }
}
