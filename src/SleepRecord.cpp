#include "SleepRecord.hpp"
#include <iomanip> // std::fixed, std::setprecision
#include <limits>  // std::numeric_limits
#include <string>

// Конструктор: создает запись о сне с указанным количеством часов
SleepRecord::SleepRecord(double hours_)
{
    setHours(hours_);
}

// Возвращает количество часов сна
double SleepRecord::getHours() const { return hours; }

// Устанавливает количество часов сна (с проверкой, что от 0 до 24)
void SleepRecord::setHours(double v)
{
    if (v >= 0.0 && v <= 24.0)
        hours = v;
}

// Рассчитывает калории, потраченные во сне (на основе базового метаболизма BMR)
double SleepRecord::calcBurnedKcal(double basalMetabolism) const
{
    if (basalMetabolism <= 0.0 || hours <= 0.0)
        return 0.0;
    // Организм тратит примерно BMR/24 калорий в час даже во сне
    return (basalMetabolism / 24.0) * hours;
}

// Запрашивает у пользователя количество часов сна с консоли
void SleepRecord::inputFromConsole()
{
    auto readDouble = [](const std::string &prompt) -> double
    {
        double v;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> v)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
                if (v >= 0.0 && v <= 24.0)
                    return v;
                std::cout << "Ошибка: сон должен быть от 0 до 24 часов.\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число.\n";
            }
        }
    };

    hours = readDouble("Сколько часов вы спали: ");
}

// Выводит информацию о сне (время сна, норму и потраченные калории)
void SleepRecord::print(double norm, double bmr) const
{
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Сон: " << hours << " ч";
    if (norm > 0.0)
        std::cout << " (норма: " << norm << " ч)";
    if (bmr > 0.0)
        std::cout << " | Потрачено во сне: " << calcBurnedKcal(bmr) << " ккал";
    std::cout << "\n";
}

// Сохраняет запись о сне в бинарный файл
void SleepRecord::save(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&hours), sizeof(double));
}

// Загружает запись о сне из бинарного файла
void SleepRecord::load(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&hours), sizeof(double));
}

// Возвращает размер записи о сне в файле (в байтах)
std::size_t SleepRecord::binarySize()
{
    return sizeof(double);
}