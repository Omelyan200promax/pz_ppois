#include "Product.hpp"
#include <cstring>   // std::strncpy, std::strcmp
#include <iomanip>   // std::setw, std::fixed, std::setprecision
#include <limits>    // std::numeric_limits
#include <algorithm> // std::transform, std::tolower

// Конструкторы

Product::Product(const std::string &name_,
                 double calories_,
                 double proteins_,
                 double fats_,
                 double carbs_,
                 double fiber_)
{
    setName(name_);
    setNutrients(calories_, proteins_, fats_, carbs_, fiber_);
}

// Геттеры

std::string Product::getName() const
{
    return std::string(name);
}

double Product::getCalories() const { return calories; }
double Product::getProteins() const { return proteins; }
double Product::getFats() const { return fats; }
double Product::getCarbs() const { return carbs; }
double Product::getFiber() const { return fiber; }

// Сеттеры

void Product::setName(const std::string &newName)
{
    // Обрезаем до NAME_SIZE - 1 символа    , чтобы остался '\0'
    std::strncpy(name, newName.c_str(), NAME_SIZE - 1);
    name[NAME_SIZE - 1] = '\0';
}

void Product::setCalories(double v)
{
    if (v >= 0.0)
        calories = v;
}

void Product::setProteins(double v)
{
    if (v >= 0.0)
        proteins = v;
}

void Product::setFats(double v)
{
    if (v >= 0.0)
        fats = v;
}

void Product::setCarbs(double v)
{
    if (v >= 0.0)
        carbs = v;
}

void Product::setFiber(double v)
{
    if (v >= 0.0)
        fiber = v;
}

void Product::setNutrients(double calories_, double proteins_, double fats_, double carbs_, double fiber_)
{
    setCalories(calories_);
    setProteins(proteins_);
    setFats(fats_);
    setCarbs(carbs_);
    setFiber(fiber_);
}

// Пересчёт на граммовку

double Product::calcCalories(double grams) const
{
    return calories * grams / 100.0;
}

double Product::calcProteins(double grams) const
{
    return proteins * grams / 100.0;
}

double Product::calcFats(double grams) const
{
    return fats * grams / 100.0;
}

double Product::calcCarbs(double grams) const
{
    return carbs * grams / 100.0;
}

double Product::calcFiber(double grams) const
{
    return fiber * grams / 100.0;
}

// Сравнение

// Алфавитная сортировка без учёта регистра.
// Например, apple и Apple считаются равными по первому символу.
bool Product::operator<(const Product &other) const
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

bool Product::operator==(const Product &other) const
{
    return std::strcmp(name, other.name) == 0;
}

// Ввод с консоли

void Product::inputFromConsole()
{
    std::string tmpName;
    std::cout << "Введите название продукта: ";
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
                if (v < 0.0)
                {
                    std::cout << "Ошибка: значение не может быть отрицательным.\n";
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
                return v;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    };

    calories = readDouble("Калорийность на 100 г (ккал): ");
    proteins = readDouble("Белки на 100 г (г):         ");
    fats = readDouble("Жиры на 100 г (г):          ");
    carbs = readDouble("Углеводы на 100 г (г):      ");
    fiber = readDouble("Клетчатка на 100 г (г):     ");
}

// Вывод в консоль

void Product::print() const
{
    std::cout << "-----------------------------\n";
    std::cout << "Продукт: " << name << "\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Калории   (на 100 г): " << calories << " ккал\n";
    std::cout << "  Белки     (на 100 г): " << proteins << " г\n";
    std::cout << "  Жиры      (на 100 г): " << fats << " г\n";
    std::cout << "  Углеводы  (на 100 г): " << carbs << " г\n";
    std::cout << "  Клетчатка (на 100 г): " << fiber << " г\n";
    std::cout << "-----------------------------\n";
}

void Product::printShort() const
{
    std::cout << std::left << std::setw(30) << name
              << std::right << std::fixed << std::setprecision(1)
              << std::setw(8) << calories << " ккал | "
              << "Б " << std::setw(5) << proteins << "  "
              << "Ж " << std::setw(5) << fats << "  "
              << "У " << std::setw(5) << carbs << "  "
              << "Кл " << std::setw(5) << fiber << "\n";
}

// Бинарный файл

void Product::save(std::ofstream &out) const
{
    // Записываем поля в фиксированном порядке.
    out.write(name, NAME_SIZE);
    out.write(reinterpret_cast<const char *>(&calories), sizeof(double));
    out.write(reinterpret_cast<const char *>(&proteins), sizeof(double));
    out.write(reinterpret_cast<const char *>(&fats), sizeof(double));
    out.write(reinterpret_cast<const char *>(&carbs), sizeof(double));
    out.write(reinterpret_cast<const char *>(&fiber), sizeof(double));
}

void Product::load(std::ifstream &in)
{
    in.read(name, NAME_SIZE);
    name[NAME_SIZE - 1] = '\0'; // страховка от повреждённых данных

    in.read(reinterpret_cast<char *>(&calories), sizeof(double));
    in.read(reinterpret_cast<char *>(&proteins), sizeof(double));
    in.read(reinterpret_cast<char *>(&fats), sizeof(double));
    in.read(reinterpret_cast<char *>(&carbs), sizeof(double));
    in.read(reinterpret_cast<char *>(&fiber), sizeof(double));
}

std::size_t Product::binarySize()
{
    return NAME_SIZE + sizeof(double) * 5;
}