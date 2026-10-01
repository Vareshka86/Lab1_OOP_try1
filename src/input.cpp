/**
 * @file input.cpp
 * @brief Реализация функций безопасного ввода чисел (см. input.h).
 * @author Vareshka86
 * @date 2026-10-01
 * @version 0.1
 */

#include "input.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

int readInt(const std::string& prompt)
{
    std::string line; // вся строка, введённая пользователем

    while (true)
    {
        std::cout << prompt;

        // Если строку прочитать не удалось - поток закончился (EOF).
        // Повторять запрос бессмысленно, поэтому сообщаем об ошибке исключением.
        if (!std::getline(std::cin, line))
        {
            throw std::runtime_error("ввод данных прерван (достигнут конец потока)");
        }

        std::istringstream stream{line}; // разбираем строку как отдельный поток
        int value{};                     // инициализация в фигурных скобках: value == 0
        char extra{};                    // сюда попадёт первый «лишний» символ, если он есть

        // Корректно, если число прочиталось И после него в строке ничего нет.
        // Переполнение int (например, 99999999999) тоже считается ошибкой:
        // в этом случае оператор >> переводит поток в состояние fail.
        if ((stream >> value) && !(stream >> extra))
        {
            return value;
        }

        std::cout << "Ошибка: нужно ввести одно целое число. Попробуйте ещё раз.\n";
    }
}

int readIntInRange(const std::string& prompt, int minValue, int maxValue)
{
    while (true)
    {
        const int value = readInt(prompt);

        if (value >= minValue && value <= maxValue)
        {
            return value;
        }

        std::cout << "Ошибка: число должно быть в диапазоне от " << minValue
                  << " до " << maxValue << ". Попробуйте ещё раз.\n";
    }
}
