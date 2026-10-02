/**
 * @file task3.cpp
 * @brief Реализация Задания №3: структура SafeArray, getElement(), printSafe(), reSizeArray().
 * @author Vareshka86
 * @date 2026-10-02
 * @version 0.4
 */

#include "task3.h"
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
 * @brief Заполняет массив случайными числами или с клавиатуры.
 * @details Каждый элемент записывается вызовом getElement() слева от знака
 * равенства, поэтому заполнение заодно показывает работу функции доступа.
 * @param arr Ссылка на заполняемый массив.
 */
void fillSafeArray(SafeArray& arr)
{
    std::cout << "\nКак заполнить массив?\n"
              << "  1 - случайными числами от " << RANDOM_MIN << " до " << RANDOM_MAX << "\n"
              << "  2 - вручную с клавиатуры\n";
    const int choice = readIntInRange("Ваш выбор: ", 1, 2);

    if (choice == 1)
    {
        for (int i = 0; i < arr.size; ++i)
        {
            getElement(arr, i) = randomInt(RANDOM_MIN, RANDOM_MAX);
        }
        std::cout << "Массив заполнен случайными числами.\n";
        return;
    }

    std::cout << "Введите " << arr.size << " целых чисел:\n";
    for (int i = 0; i < arr.size; ++i)
    {
        getElement(arr, i) = readInt("  arr[" + std::to_string(i) + "] = ");
    }
}

/**
 * @brief Выводит меню действий Задания №3.
 */
void printTask3Menu()
{
    std::cout << "\nДействия с массивом:\n"
              << "  1 - вывести массив            (printSafe)\n"
              << "  2 - прочитать элемент         (getElement справа от =)\n"
              << "  3 - записать элемент          (getElement слева от =)\n"
              << "  4 - изменить размер массива   (reSizeArray)\n"
              << "  0 - вернуться в главное меню\n";
}

} // namespace

SafeArray createArray(int size)
{
    // Неверный размер - пустая структура, память не выделяется
    if (size <= 0)
    {
        return SafeArray{nullptr, 0};
    }

    // Пустые фигурные скобки {} обнуляют все элементы
    SafeArray arr{new int[size]{}, size};
    return arr; // возврат по значению: копируются указатель и размер, а не элементы
}

int& getElement(SafeArray& arr, int index)
{
    // Статическая локальная переменная-заглушка: создаётся один раз и живёт
    // до конца программы, поэтому ссылку на неё возвращать безопасно.
    static int dummy = 0;

    if (arr.data == nullptr || index < 0 || index >= arr.size)
    {
        std::cout << "Ошибка: индекс " << index << " вне границ массива";
        if (arr.size > 0)
        {
            std::cout << " (допустимо от 0 до " << arr.size - 1 << ")";
        }
        std::cout << ". Используется заглушка, массив не меняется.\n";

        dummy = 0;    // обнуляем, чтобы чтение по неверному индексу всегда давало 0
        return dummy; // запись в заглушку не затрагивает основной массив
    }

    return arr.data[index]; // ссылка на сам элемент массива
}

void printSafe(const SafeArray& arr)
{
    // const SafeArray& запрещает менять поля структуры (data и size), но data -
    // это int*, и через него сами числа поменять было бы можно. Поэтому элементы
    // читаются через указатель на константу.
    const int* values = arr.data;

    if (values == nullptr)
    {
        std::cout << "Массива нет (data == nullptr): выводить нечего.\n";
        return;
    }
    if (arr.size <= 0)
    {
        std::cout << "Массив пуст (0 элементов).\n";
        return;
    }

    std::cout << "Индекс   :";
    for (int i = 0; i < arr.size; ++i)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << i;
    }
    std::cout << '\n';

    std::cout << "Значение :";
    for (int i = 0; i < arr.size; ++i)
    {
        std::cout << ' ' << std::setw(COLUMN_WIDTH) << values[i];
    }
    std::cout << '\n';
}

void reSizeArray(SafeArray& arr, int N, int M)
{
    // Проверки: массив должен существовать, N - совпадать с его размером, M - быть больше 0
    if (arr.data == nullptr)
    {
        std::cout << "Ошибка: массива нет - менять размер нечему.\n";
        return;
    }
    if (N != arr.size)
    {
        std::cout << "Ошибка: N = " << N << " не совпадает с размером массива " << arr.size
                  << ". Массив не изменён.\n";
        return;
    }
    if (M <= 0)
    {
        std::cout << "Ошибка: новый размер M должен быть больше 0. Массив не изменён.\n";
        return;
    }
    if (M == N)
    {
        std::cout << "Новый размер равен текущему (" << N << ") - массив не изменился.\n";
        return;
    }

    // Новый массив: пустые фигурные скобки {} обнуляют все элементы, поэтому
    // при увеличении размера новые элементы сразу равны 0 (пункт 6.2 условия)
    int* newData = new int[M]{};

    // Копируем элементы, которые есть и в старом, и в новом массиве
    const int common = (M < N) ? M : N;
    for (int i = 0; i < common; ++i)
    {
        newData[i] = arr.data[i];
    }

    if (M < N)
    {
        // Пункт 6.1 условия: элементы с индексами M ... N - 1 удаляются - выводим их
        std::cout << "Размер уменьшен с " << N << " до " << M << ". Удалённые элементы:\n";
        std::cout << "Индекс   :";
        for (int i = M; i < N; ++i)
        {
            std::cout << ' ' << std::setw(COLUMN_WIDTH) << i;
        }
        std::cout << '\n' << "Значение :";
        for (int i = M; i < N; ++i)
        {
            std::cout << ' ' << std::setw(COLUMN_WIDTH) << arr.data[i];
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << "Размер увеличен с " << N << " до " << M << ". Новые элементы с индексами "
                  << N << "-" << M - 1 << " равны 0.\n";
    }

    delete[] arr.data;   // старый массив больше не нужен - освобождаем, иначе утечка
    arr.data = newData;  // структура теперь описывает новый массив
    arr.size = M;
}

void freeSafeArray(SafeArray& arr)
{
    delete[] arr.data;  // для nullptr ничего не делает, поэтому повторный вызов безопасен
    arr.data = nullptr; // в структуре не остаётся адреса освобождённой памяти
    arr.size = 0;
}

void runTask3()
{
    std::cout << "\n=== Задание №3. Безопасный массив SafeArray ===\n";

    const int size = readIntInRange("\nРазмер массива (от " + std::to_string(TASK3_MIN_SIZE) +
                                        " до " + std::to_string(TASK3_MAX_SIZE) + "): ",
                                    TASK3_MIN_SIZE, TASK3_MAX_SIZE);

    // createArray() возвращает структуру по значению
    SafeArray myArr = createArray(size);
    std::cout << "Массив создан: createArray(" << size << ") - все элементы равны 0.\n";
    printSafe(myArr);

    try
    {
        fillSafeArray(myArr);
        std::cout << "\nМассив после заполнения:\n";
        printSafe(myArr);

        // Пункт 4 условия: вызов getElement() слева от знака равенства
        std::cout << "\ngetElement(myArr, 2) = 999;\n";
        getElement(myArr, 2) = 999;
        printSafe(myArr);

        // Индекс за границей массива: запись уходит в заглушку, массив не меняется
        std::cout << "\ngetElement(myArr, " << myArr.size << ") = 555;   // индекс за границей массива\n";
        getElement(myArr, myArr.size) = 555;
        printSafe(myArr);

        while (true)
        {
            printTask3Menu();
            const int choice = readIntInRange("Ваш выбор: ", 0, 4);
            if (choice == 0)
            {
                break;
            }

            switch (choice)
            {
            case 1:
                printSafe(myArr);
                break;

            case 2:
            {
                // Вызов справа от знака равенства: значение элемента копируется в value
                const int index = readInt("Индекс элемента: ");
                const int value = getElement(myArr, index);
                std::cout << "getElement(myArr, " << index << ") = " << value << '\n';
                break;
            }

            case 3:
            {
                // Вызов слева от знака равенства: запись прямо в элемент массива
                const int index = readInt("Индекс элемента: ");
                const int value = readInt("Новое значение: ");
                getElement(myArr, index) = value;
                printSafe(myArr);
                break;
            }

            default: // 4
            {
                // N - текущий размер массива, M - новый, как в условии задания
                const int M = readIntInRange("Новый размер M (от 1 до " +
                                                 std::to_string(TASK3_MAX_SIZE) + "): ",
                                             1, TASK3_MAX_SIZE);
                reSizeArray(myArr, myArr.size, M);
                std::cout << "Массив после изменения размера:\n";
                printSafe(myArr);
                break;
            }
            }
        }
    }
    catch (...)
    {
        // Если ввод прервался, память всё равно освобождается - утечки нет.
        // Затем исключение передаётся дальше в main.
        freeSafeArray(myArr);
        throw;
    }

    freeSafeArray(myArr);
    std::cout << "\nПамять освобождена (freeSafeArray): data = nullptr, size = 0.\n";
    printSafe(myArr);
}
