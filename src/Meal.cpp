#include "Meal.hpp"

#include <iomanip> // std::setw, std::fixed, std::setprecision
#include <limits>  // std::numeric_limits

// Свободные функции.

std::string mealTypeName(MealType t)
{
    switch (t)
    {
    case MealType::Breakfast:
        return "Завтрак";
    case MealType::Lunch:
        return "Обед";
    case MealType::Dinner:
        return "Ужин";
    case MealType::Snack:
        return "Перекус";
    }
    return "?";
}

// Конструкторы

Meal::Meal(MealType type_) : type(type_)
{
}

// Геттер и сеттер

MealType Meal::getType() const { return type; }

void Meal::setType(MealType t) { type = t; }

// Работа со списком продуктов

bool Meal::addProduct(const Product &p, double g)
{
    if (g >= 0.0)
    {
        // Такой продукт уже есть, просто увеличиваем граммы
        for (std::size_t i = 0; i < products.size(); ++i)
        {
            if (products[i].getName() == p.getName())
            {
                grams[i] += g;
                return true;
            }
        }
        products.push_back(p);
        grams.push_back(g);
        return true;
    }
    return false;
}

bool Meal::addProductFromConsole(const Product &p)
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

    double g = readDouble("Сколько грамм \"" + p.getName() + "\" съедено: ");
    return addProduct(p, g);
}

bool Meal::removeProduct(const std::string &name)
{
    for (std::size_t i = 0; i < products.size(); ++i)
    {
        if (products[i].getName() == name)
        {
            products.erase(products.begin() + static_cast<std::ptrdiff_t>(i));
            grams.erase(grams.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }
    return false;
}

void Meal::clear()
{
    products.clear();
    grams.clear();
}

bool Meal::isEmpty() const
{
    return products.empty();
}

std::size_t Meal::size() const
{
    return products.size();
}

const std::vector<Product> &Meal::getProducts() const
{
    return products;
}

const std::vector<double> &Meal::getGrams() const
{
    return grams;
}

// Итоги

double Meal::getCalories() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcCalories(grams[i]);
    return sum;
}

double Meal::getProteins() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcProteins(grams[i]);
    return sum;
}

double Meal::getFats() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcFats(grams[i]);
    return sum;
}

double Meal::getCarbs() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcCarbs(grams[i]);
    return sum;
}

double Meal::getFiber() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcFiber(grams[i]);
    return sum;
}

// Вывод в консоль

void Meal::print() const
{
    std::cout << std::fixed << std::setprecision(1);
    std::cout << mealTypeName(type) << ":\n";

    if (products.empty())
    {
        std::cout << "  (пусто)\n";
        return;
    }

    for (std::size_t i = 0; i < products.size(); ++i)
    {
        std::cout << "  " << products[i].getName() << ", " << grams[i] << " г - "
                  << products[i].calcCalories(grams[i]) << " ккал\n";
    }

    std::cout << "  Итого: " << getCalories() << " ккал | "
              << "Б " << getProteins() << "  "
              << "Ж " << getFats() << "  "
              << "У " << getCarbs() << "  "
              << "Кл " << getFiber() << "\n";
}

// Бинарный файл

void Meal::save(std::ofstream &out) const
{
    int t = static_cast<int>(type);
    out.write(reinterpret_cast<const char *>(&t), sizeof(int));

    // Количество позиций
    std::size_t count = products.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Для каждой позиции: продукт и граммовка
    for (std::size_t i = 0; i < count; ++i)
    {
        products[i].save(out);
        out.write(reinterpret_cast<const char *>(&grams[i]), sizeof(double));
    }
}

void Meal::load(std::ifstream &in)
{
    products.clear();
    grams.clear();

    int t = 0;
    in.read(reinterpret_cast<char *>(&t), sizeof(int));
    if (t < 0 || t > 3)
        t = 0; // повреждённое значение, ставим завтрак
    type = static_cast<MealType>(t);

    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));
    if (!in || count > 100000)
        return;

    for (std::size_t i = 0; i < count; ++i)
    {
        Product p("", 0.0, 0.0, 0.0, 0.0); // конструктор по умолчанию у Product не определён
        double g = 0.0;
        p.load(in);
        in.read(reinterpret_cast<char *>(&g), sizeof(double));
        if (!in)
        {
            // Файл повреждён, очищаем приём пищи
            products.clear();
            grams.clear();
            return;
        }
        products.push_back(p);
        grams.push_back(g);
    }
}
