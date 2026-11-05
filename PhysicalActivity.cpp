
#include "PhysicalActivity.h"
#include <iostream>
#include <cstring>   // std::strncpy, std::strcmp
#include <iomanip>   // std::setw, std::fixed, std::setprecision
#include <limits>    // std::numeric_limits
#include <algorithm> // std::transform

// Свободные функции

double intensityFactor(Intensity i)
{
    switch (i)
    {
    case Intensity::Low:
        return 0.8;
    case Intensity::Medium:
        return 1.0;
    case Intensity::High:
        return 1.2;
    }
    return 1.0;
}

std::string intensityName(Intensity i)
{
    switch (i)
    {
    case Intensity::Low:
        return "низкая";
    case Intensity::Medium:
        return "средняя";
    case Intensity::High:
        return "высокая";
    }
    return "?";
}

// Конструкторы

PhysicalActivity::PhysicalActivity(const std::string &name_, double met_, double minutes_, Intensity intensity_) : intensity(intensity_)
{
    setName(name_);
    setMet(met_);
    setMinutes(minutes_);
}

// Геттеры

std::string PhysicalActivity::getName() const { return std::string(name); }
double PhysicalActivity::getMet() const { return met; }
double PhysicalActivity::getMinutes() const { return minutes; }
Intensity PhysicalActivity::getIntensity() const { return intensity; }

// Сеттеры

void PhysicalActivity::setName(const std::string &newName)
{
    // Обрезаем до NAME_SIZE - 1 символа, чтобы остался '\0'
    std::strncpy(name, newName.c_str(), NAME_SIZE - 1);
    name[NAME_SIZE - 1] = '\0';
}

void PhysicalActivity::setMet(double v)
{
    if (v >= 0.0)
        met = v;
}

void PhysicalActivity::setMinutes(double v)
{
    if (v >= 0.0)
        minutes = v;
}

void PhysicalActivity::setIntensity(Intensity i)
{
    intensity = i;
}

// Расход калорий

double PhysicalActivity::calcKcal(double weightKg) const
{
    return calcKcal(weightKg, minutes, intensity);
}

double PhysicalActivity::calcKcal(double weightKg, double minutes_, Intensity intensity_) const
{
    if (weightKg < 0.0 || minutes_ < 0.0)
        return 0.0;
    return met * intensityFactor(intensity_) * weightKg * (minutes_ / 60.0);
}

// Сравнение

// Алфавитная сортировка без учёта регистра
bool PhysicalActivity::operator<(const PhysicalActivity &other) const
{
    std::string a(name);
    std::string b(other.name);

    std::transform(a.begin(), a.end(), a.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    std::transform(b.begin(), b.end(), b.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    return a < b;
}

bool PhysicalActivity::operator==(const PhysicalActivity &other) const
{
    return std::strcmp(name, other.name) == 0;
}

// Ввод с консоли

void PhysicalActivity::inputFromConsole()
{
    std::string tmpName;
    std::cout << "Введите название активности: ";
    std::getline(std::cin >> std::ws, tmpName);
    setName(tmpName);

    auto readDouble = [](const std::string &prompt) -> double
    {
        double v;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> v)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
                if (v >= 0.0)
                    return v;
                std::cout << "Ошибка: значение не может быть отрицательным.\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число.\n";
            }
        }
    };

    met = readDouble("MET активности (например, 9.8 для бега): ");
}

void PhysicalActivity::inputWorkoutFromConsole()
{
    auto readDouble = [](const std::string &prompt) -> double
    {
        double v;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> v)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (v >= 0.0)
                    return v;
                std::cout << "Ошибка: значение не может быть отрицательным.\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число.\n";
            }
        }
    };

    minutes = readDouble("Длительность (мин): ");

    int level = 0;
    while (level < 1 || level > 3)
    {
        level = static_cast<int>(readDouble("Интенсивность (1 - низкая, 2 - средняя, 3 - высокая): "));
        if (level < 1 || level > 3)
            std::cout << "Ошибка: введите 1, 2 или 3.\n";
    }
    intensity = static_cast<Intensity>(level - 1);
}

// Вывод в консоль

void PhysicalActivity::print() const
{
    std::cout << "-----------------------------\n";
    std::cout << "Активность: " << name << "\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  MET:           " << met << "\n";
    std::cout << "  Время:         " << minutes << " мин\n";
    std::cout << "  Интенсивность: " << intensityName(intensity) << "\n";
    std::cout << "-----------------------------\n";
}

void PhysicalActivity::printShort() const
{
    // setw считает байты, а кириллица в UTF-8 занимает 2 байта на букву,
    // поэтому длину в символах считаем сами и добавляем пробелы вручную
    std::size_t len = 0;
    for (const char *p = name; *p != '\0'; ++p)
    {
        if ((static_cast<unsigned char>(*p) & 0xC0) != 0x80)
            ++len;
    }

    std::cout << name << std::string(len < 30 ? 30 - len : 1, ' ')
              << std::fixed << std::setprecision(1)
              << "MET " << std::setw(5) << met << "\n";
}

// Бинарный файл

void PhysicalActivity::save(std::ofstream &out) const
{
    out.write(name, NAME_SIZE);
    out.write(reinterpret_cast<const char *>(&met), sizeof(double));
    out.write(reinterpret_cast<const char *>(&minutes), sizeof(double));

    int level = static_cast<int>(intensity);
    out.write(reinterpret_cast<const char *>(&level), sizeof(int));
}

void PhysicalActivity::load(std::ifstream &in)
{
    in.read(name, NAME_SIZE);
    name[NAME_SIZE - 1] = '\0'; // страховка от повреждённых данных

    in.read(reinterpret_cast<char *>(&met), sizeof(double));
    in.read(reinterpret_cast<char *>(&minutes), sizeof(double));

    int level = 1;
    in.read(reinterpret_cast<char *>(&level), sizeof(int));
    if (level < 0 || level > 2)
        level = 1; // повреждённое значение, ставим среднюю
    intensity = static_cast<Intensity>(level);
}

std::size_t PhysicalActivity::binarySize()
{
    return NAME_SIZE + sizeof(double) * 2 + sizeof(int);
}
