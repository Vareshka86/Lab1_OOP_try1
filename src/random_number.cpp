/**
 * @file random_number.cpp
 * @brief Реализация генератора случайных чисел (см. random_number.h).
 * @author Vareshka86
 * @date 2026-10-02
 * @version 0.2
 */

#include "random_number.h"

#include <random>

/// Вспомогательные функции, видимые только внутри этого файла.
namespace
{

/**
 * @brief Возвращает генератор случайных чисел, общий для всей программы.
 * @details Генератор хранится в **статической локальной** переменной:
 * он создаётся один раз при первом вызове и живёт до конца программы.
 * Поэтому из функции можно безопасно вернуть ссылку на него
 * (возвращать ссылку на обычную локальную переменную нельзя —
 * она уничтожается при выходе из функции).
 * @return Ссылка на генератор std::mt19937.
 */
std::mt19937& randomEngine()
{
    // std::random_device даёт случайное «зерно», чтобы при каждом
    // запуске программы получались разные числа.
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

} // namespace

int randomInt(int minValue, int maxValue)
{
    // Равномерное распределение целых чисел на отрезке [minValue; maxValue]
    std::uniform_int_distribution<int> distribution{minValue, maxValue};
    return distribution(randomEngine());
}
