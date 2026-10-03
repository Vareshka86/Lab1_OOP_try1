/**
 * @file task4.cpp
 * @brief Реализация Задания №4: allocateMatrix(), fillMatrix(), printMatrix(), freeMatrix().
 * @author Vareshka86
 * @date 2026-10-03
 * @version 1.0
 */

#include "task4.h"
#include "input.h"
#include "random_number.h"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/// Ширина колонки с подписью строки: «Студент 10» — 10 символов.
constexpr std::size_t LABEL_WIDTH = 10;

/// Ширина колонки с одной оценкой (вместе с пробелами перед ней).
constexpr int CELL_WIDTH = 4;

/**
 * @brief Считает, сколько символов займёт строка на экране.
 * @details Исходные файлы и консоль работают в кодировке UTF-8: латинская буква
 * или цифра занимает 1 байт, русская буква — 2 байта, знак «№» — 3 байта.
 * Поэтому `text.size()` (число байтов) для русского текста больше числа видимых
 * символов. Каждый символ UTF-8 начинается с байта, который **не** имеет вида
 * 10xxxxxx (так выглядят только байты продолжения), — такие байты и считаются.
 * @param text Строка в кодировке UTF-8.
 * @return Количество видимых символов.
 */
std::size_t displayWidth(const std::string& text)
{
    std::size_t width = 0;
    for (const char ch : text)
    {
        if ((static_cast<unsigned char>(ch) & 0xC0) != 0x80)
        {
            ++width;
        }
    }
    return width;
}

/**
 * @brief Дополняет строку пробелами справа до нужной ширины.
 * @details `std::setw` считает байты, а не символы, и русский текст с ним
 * выравнивается неверно, поэтому ширина считается через displayWidth().
 * @param text  Строка в кодировке UTF-8.
 * @param width Нужная ширина в символах.
 * @return Строка ширины width (или исходная строка, если она уже не короче).
 */
std::string padRight(const std::string& text, std::size_t width)
{
    const std::size_t current = displayWidth(text);
    if (current >= width)
    {
        return text;
    }
    return text + std::string(width - current, ' ');
}

/**
 * @brief Готовит строки таблицы оценок без рамки и заголовка.
 * @details Первая строка — номера оценок, дальше по строке на каждого студента.
 * Строки собираются заранее, чтобы printMatrix() знала их ширину и могла
 * нарисовать рамку нужного размера.
 * @param matrix Матрица оценок (не nullptr).
 * @param rows   Количество строк.
 * @param cols   Количество столбцов.
 * @return Строки таблицы.
 */
std::vector<std::string> makeTableLines(int** matrix, int rows, int cols)
{
    std::vector<std::string> lines;

    std::ostringstream header;
    header << padRight("Оценка №", LABEL_WIDTH);
    for (int j = 0; j < cols; ++j)
    {
        header << std::setw(CELL_WIDTH) << j + 1;
    }
    lines.push_back(header.str());

    for (int i = 0; i < rows; ++i)
    {
        std::ostringstream row;
        row << "Студент " << std::setw(2) << i + 1;
        for (int j = 0; j < cols; ++j)
        {
            // matrix[i] - указатель на строку i, matrix[i][j] - оценка в этой строке
            row << std::setw(CELL_WIDTH) << matrix[i][j];
        }
        lines.push_back(row.str());
    }

    return lines;
}

/**
 * @brief Выводит одну строку внутри рамки: «* текст   *».
 * @param text       Текст строки.
 * @param innerWidth Ширина места для текста внутри рамки.
 */
void printFramedLine(const std::string& text, std::size_t innerWidth)
{
    std::cout << "* " << padRight(text, innerWidth) << " *\n";
}

} // namespace

int** allocateMatrix(int rows, int cols)
{
    // Неверный размер - память не выделяется
    if (rows <= 0 || cols <= 0)
    {
        return nullptr;
    }

    // Шаг 1. Массив указателей на строки. Пустые фигурные скобки {} записывают
    // во все указатели nullptr
    int** matrix = new int*[rows]{};

    try
    {
        // Шаг 2. Каждая строка - отдельный массив чисел; {} обнуляет все оценки
        for (int i = 0; i < rows; ++i)
        {
            matrix[i] = new int[cols]{};
        }
    }
    catch (...)
    {
        // Памяти не хватило: освобождаем то, что уже выделено. Указатели
        // невыделенных строк равны nullptr, а delete[] nullptr ничего не делает.
        freeMatrix(matrix, rows);
        throw;
    }

    return matrix;
}

void fillMatrix(int** matrix, int rows, int cols)
{
    if (matrix == nullptr)
    {
        std::cout << "Матрицы нет (nullptr): заполнять нечего.\n";
        return;
    }

    std::cout << "\nКак заполнить оценки?\n"
              << "  1 - случайными оценками от " << GRADE_MIN << " до " << GRADE_MAX << "\n"
              << "  2 - вручную с клавиатуры\n";
    const int choice = readIntInRange("Ваш выбор: ", 1, 2);

    if (choice == 1)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                matrix[i][j] = randomInt(GRADE_MIN, GRADE_MAX);
            }
        }
        std::cout << "Оценки заполнены случайными числами.\n";
        return;
    }

    std::cout << "Введите оценки (целые числа от " << GRADE_MIN << " до " << GRADE_MAX << "):\n";
    for (int i = 0; i < rows; ++i)
    {
        std::cout << "Студент " << i + 1 << ":\n";
        for (int j = 0; j < cols; ++j)
        {
            matrix[i][j] = readIntInRange("  оценка " + std::to_string(j + 1) + ": ",
                                          GRADE_MIN, GRADE_MAX);
        }
    }
}

// Значения по умолчанию (showBorders = true, title = "Matrix") указаны только
// в объявлении в task4.h; повторять их здесь нельзя - это ошибка компиляции.
void printMatrix(int** matrix, int rows, int cols, bool showBorders, std::string title)
{
    if (matrix == nullptr || rows <= 0 || cols <= 0)
    {
        std::cout << title << ": матрицы нет (nullptr) - выводить нечего.\n";
        return;
    }

    const std::vector<std::string> lines = makeTableLines(matrix, rows, cols);

    if (!showBorders)
    {
        std::cout << title << '\n';
        for (const std::string& line : lines)
        {
            std::cout << line << '\n';
        }
        return;
    }

    // Ширина места внутри рамки - по самой длинной строке: таблицы или заголовка
    std::size_t innerWidth = displayWidth(title);
    for (const std::string& line : lines)
    {
        if (displayWidth(line) > innerWidth)
        {
            innerWidth = displayWidth(line);
        }
    }

    // Рамка из '*': по краям строки "* " и " *", поэтому она на 4 символа шире текста
    const std::string border(innerWidth + 4, '*');
    const std::string separator = "*" + std::string(innerWidth + 2, '-') + "*";

    std::cout << border << '\n';
    printFramedLine(title, innerWidth);
    std::cout << separator << '\n';
    for (const std::string& line : lines)
    {
        printFramedLine(line, innerWidth);
    }
    std::cout << border << '\n';
}

void freeMatrix(int** matrix, int rows)
{
    // Массива нет - освобождать нечего (и читать matrix[i] нельзя)
    if (matrix == nullptr)
    {
        return;
    }

    // Сначала вложенные массивы - строки. Если удалить массив указателей первым,
    // адреса строк пропадут и строки уже не освободить
    for (int i = 0; i < rows; ++i)
    {
        delete[] matrix[i];
    }

    // Потом сам массив указателей
    delete[] matrix;
}

void runTask4()
{
    std::cout << "\n=== Задание №4. Учёт оценок студентов (двумерный массив) ===\n";

    const int rows = readIntInRange("\nКоличество студентов (от 1 до " +
                                        std::to_string(TASK4_MAX_ROWS) + "): ",
                                    1, TASK4_MAX_ROWS);
    const int cols = readIntInRange("Количество оценок у каждого студента (от 1 до " +
                                        std::to_string(TASK4_MAX_COLS) + "): ",
                                    1, TASK4_MAX_COLS);

    int** grades = allocateMatrix(rows, cols);
    std::cout << "\nПамять выделена: allocateMatrix(" << rows << ", " << cols << ") - массив из "
              << rows << " указателей на строки и " << rows << " строк по " << cols
              << " оценок, все оценки равны 0.\n";

    try
    {
        fillMatrix(grades, rows, cols);

        // Три вызова printMatrix() из условия задания
        std::cout << "\n1) Без дополнительных параметров:\n"
                  << "printMatrix(grades, rows, cols);\n";
        printMatrix(grades, rows, cols);

        // Параметр в середине списка пропустить нельзя: чтобы задать заголовок,
        // нужно указать и showBorders (здесь - то же значение, что по умолчанию)
        std::cout << "\n2) Только с заголовком:\n"
                  << "printMatrix(grades, rows, cols, true, \"Оценки студентов\");\n";
        printMatrix(grades, rows, cols, true, "Оценки студентов");

        std::cout << "\n3) Со всеми параметрами:\n"
                  << "printMatrix(grades, rows, cols, false, \"Оценки без рамки\");\n";
        printMatrix(grades, rows, cols, false, "Оценки без рамки");
    }
    catch (...)
    {
        // Если ввод прервался, память всё равно освобождается - утечки нет.
        // Затем исключение передаётся дальше в main.
        freeMatrix(grades, rows);
        throw;
    }

    freeMatrix(grades, rows);
    grades = nullptr; // freeMatrix() получила копию указателя, поэтому обнуляем его здесь
    std::cout << "\nПамять освобождена: freeMatrix(grades, rows) - сначала " << rows
              << " строк, затем массив указателей; grades = nullptr.\n";
}
