#include "Product.hpp"
#include <cstring>   // std::strncpy, std::strcmp
#include <iomanip>   // std::setw, std::fixed, std::setprecision
#include <limits>    // std::numeric_limits
#include <algorithm> // std::transform, std::tolower

// Конструктор: создает продукт с заданными названием и пищевой ценностью на 100 г
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

// Возвращает название продукта
std::string Product::getName() const
{
    return std::string(name);
}

// Возвращает калорийность на 100 г
double Product::getCalories() const { return calories; }

// Возвращает количество белков на 100 г
double Product::getProteins() const { return proteins; }

// Возвращает количество жиров на 100 г
double Product::getFats() const { return fats; }

// Возвращает количество углеводов на 100 г
double Product::getCarbs() const { return carbs; }

// Возвращает количество клетчатки на 100 г
double Product::getFiber() const { return fiber; }

// Устанавливает название продукта (с защитой от переполнения массива)
void Product::setName(const std::string &newName)
{
    // Обрезаем до NAME_SIZE - 1 символа, чтобы остался '\0'
    std::strncpy(name, newName.c_str(), NAME_SIZE - 1);
    name[NAME_SIZE - 1] = '\0';
}

// Устанавливает калорийность (с проверкой на отрицательные значения)
void Product::setCalories(double v)
{
    if (v >= 0.0)
        calories = v;
}

// Устанавливает количество белков (с проверкой на отрицательные значения)
void Product::setProteins(double v)
{
    if (v >= 0.0)
        proteins = v;
}

// Устанавливает количество жиров (с проверкой на отрицательные значения)
void Product::setFats(double v)
{
    if (v >= 0.0)
        fats = v;
}

// Устанавливает количество углеводов (с проверкой на отрицательные значения)
void Product::setCarbs(double v)
{
    if (v >= 0.0)
        carbs = v;
}

// Устанавливает количество клетчатки (с проверкой на отрицательные значения)
void Product::setFiber(double v)
{
    if (v >= 0.0)
        fiber = v;
}

// Устанавливает сразу все нутриенты (вызывает соответствующие сеттеры)
void Product::setNutrients(double calories_, double proteins_, double fats_, double carbs_, double fiber_)
{
    setCalories(calories_);
    setProteins(proteins_);
    setFats(fats_);
    setCarbs(carbs_);
    setFiber(fiber_);
}

// Пересчитывает калорийность на указанную граммовку
double Product::calcCalories(double grams) const
{
    return calories * grams / 100.0;
}

// Пересчитывает количество белков на указанную граммовку
double Product::calcProteins(double grams) const
{
    return proteins * grams / 100.0;
}

// Пересчитывает количество жиров на указанную граммовку
double Product::calcFats(double grams) const
{
    return fats * grams / 100.0;
}

// Пересчитывает количество углеводов на указанную граммовку
double Product::calcCarbs(double grams) const
{
    return carbs * grams / 100.0;
}

// Пересчитывает количество клетчатки на указанную граммовку
double Product::calcFiber(double grams) const
{
    return fiber * grams / 100.0;
}

// Оператор "меньше": алфавитная сортировка без учёта регистра
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

// Оператор равенства: сравнивает продукты по названию
bool Product::operator==(const Product &other) const
{
    return std::strcmp(name, other.name) == 0;
}

// Запрашивает у пользователя данные о продукте (название и БЖУ) для создания нового
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

// Выводит подробную информацию о продукте (все значения на 100 г)
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

// Выводит компактную информацию о продукте (одна строка для таблицы каталога)
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

// Сохраняет продукт в бинарный файл (записывает все поля в фиксированном порядке)
void Product::save(std::ofstream &out) const
{
    out.write(name, NAME_SIZE);
    out.write(reinterpret_cast<const char *>(&calories), sizeof(double));
    out.write(reinterpret_cast<const char *>(&proteins), sizeof(double));
    out.write(reinterpret_cast<const char *>(&fats), sizeof(double));
    out.write(reinterpret_cast<const char *>(&carbs), sizeof(double));
    out.write(reinterpret_cast<const char *>(&fiber), sizeof(double));
}

// Загружает продукт из бинарного файла
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

// Возвращает размер записи продукта в файле (в байтах)
std::size_t Product::binarySize()
{
    return NAME_SIZE + sizeof(double) * 5;
}