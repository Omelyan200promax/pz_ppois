#include "SleepRecord.hpp"
#include <iomanip> // std::fixed, std::setprecision
#include <limits>  // std::numeric_limits
#include <string>

// Конструкторы.

SleepRecord::SleepRecord(double hours_)
{
    setHours(hours_);
}

// Геттер

double SleepRecord::getHours() const { return hours; }

// Сеттер

void SleepRecord::setHours(double v)
{
    if (v >= 0.0 && v <= 24.0)
        hours = v;
}

// Ввод с консоли

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

// Вывод в консоль

void SleepRecord::print(double norm) const
{
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Сон: " << hours << " ч";
    if (norm > 0.0)
        std::cout << " (норма: " << norm << " ч)";
    std::cout << "\n";
}

// Бинарный файл

void SleepRecord::save(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&hours), sizeof(double));
}

void SleepRecord::load(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&hours), sizeof(double));
}

std::size_t SleepRecord::binarySize()
{
    return sizeof(double);
}
