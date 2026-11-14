
#include "PhysicalActivity.hpp"
#include <iostream>
#include <cstring>   // std::strncpy, std::strcmp
#include <iomanip>   // std::setw, std::fixed, std::setprecision
#include <limits>    // std::numeric_limits
#include <algorithm> // std::transform

// Возвращает множитель для MET в зависимости от интенсивности (низкая - 0.8, средняя - 1.0, высокая - 1.2)
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

// Возвращает текстовое название интенсивности для вывода в консоль
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

// Конструктор: создает активность с заданными параметрами
PhysicalActivity::PhysicalActivity(const std::string &name_, double met_, double minutes_, Intensity intensity_) : intensity(intensity_)
{
    setName(name_);
    setMet(met_);
    setMinutes(minutes_);
}

// Возвращает название активности
std::string PhysicalActivity::getName() const { return std::string(name); }

// Возвращает MET активности
double PhysicalActivity::getMet() const { return met; }

// Возвращает длительность активности в минутах
double PhysicalActivity::getMinutes() const { return minutes; }

// Возвращает интенсивность активности
Intensity PhysicalActivity::getIntensity() const { return intensity; }

// Устанавливает название активности (обрезает до безопасной длины)
void PhysicalActivity::setName(const std::string &newName)
{
    // Обрезаем до NAME_SIZE - 1 символа, чтобы остался '\0'
    std::strncpy(name, newName.c_str(), NAME_SIZE - 1);
    name[NAME_SIZE - 1] = '\0';
}

// Устанавливает MET активности (с проверкой на отрицательные значения)
void PhysicalActivity::setMet(double v)
{
    if (v >= 0.0)
        met = v;
}

// Устанавливает длительность активности в минутах (с проверкой на отрицательные значения)
void PhysicalActivity::setMinutes(double v)
{
    if (v >= 0.0)
        minutes = v;
}

// Устанавливает интенсивность активности
void PhysicalActivity::setIntensity(Intensity i)
{
    intensity = i;
}

// Рассчитывает сожженные калории, используя время и интенсивность, сохраненные в объекте
double PhysicalActivity::calcKcal(double weightKg) const
{
    return calcKcal(weightKg, minutes, intensity);
}

// Рассчитывает сожженные калории по формуле: MET * множитель * вес * время (в часах)
double PhysicalActivity::calcKcal(double weightKg, double minutes_, Intensity intensity_) const
{
    if (weightKg < 0.0 || minutes_ < 0.0)
        return 0.0;
    return met * intensityFactor(intensity_) * weightKg * (minutes_ / 60.0);
}

// Оператор "меньше": алфавитная сортировка без учёта регистра
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

// Оператор равенства: сравнивает активности по названию
bool PhysicalActivity::operator==(const PhysicalActivity &other) const
{
    return std::strcmp(name, other.name) == 0;
}

// Запрашивает у пользователя название и MET для добавления активности в каталог
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

// Запрашивает у пользователя время и интенсивность для записи тренировки в дневник
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

// Выводит подробную информацию об активности (название, MET, время, интенсивность)
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

// Выводит компактную информацию об активности (для списка каталога)
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

// Сохраняет активность в бинарный файл
void PhysicalActivity::save(std::ofstream &out) const
{
    out.write(name, NAME_SIZE);
    out.write(reinterpret_cast<const char *>(&met), sizeof(double));
    out.write(reinterpret_cast<const char *>(&minutes), sizeof(double));

    int level = static_cast<int>(intensity);
    out.write(reinterpret_cast<const char *>(&level), sizeof(int));
}

// Загружает активность из бинарного файла (с защитой от поврежденных данных)
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

// Возвращает размер записи активности в файле (в байтах)
std::size_t PhysicalActivity::binarySize()
{
    return NAME_SIZE + sizeof(double) * 2 + sizeof(int);
}