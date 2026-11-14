#include "Meal.hpp"

#include <iomanip> // std::setw, std::fixed, std::setprecision
#include <limits>  // std::numeric_limits

// Возвращает текстовое название приема пищи (например, "Завтрак")
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

// Конструктор: создает прием пищи заданного типа (по умолчанию - завтрак)
Meal::Meal(MealType type_) : type(type_)
{
}

// Возвращает тип приема пищи
MealType Meal::getType() const { return type; }

// Устанавливает тип приема пищи
void Meal::setType(MealType t) { type = t; }

// Добавляет продукт с указанной граммовкой. Если продукт уже есть, суммирует граммы
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
        // Если продукта нет, добавляем его в конец
        products.push_back(p);
        grams.push_back(g);
        return true;
    }
    return false;
}

// Запрашивает у пользователя граммовку продукта и добавляет его в прием пищи
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

// Удаляет продукт из приема пищи по его названию
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

// Очищает прием пищи (удаляет все продукты и граммовки)
void Meal::clear()
{
    products.clear();
    grams.clear();
}

// Проверяет, пуст ли прием пищи
bool Meal::isEmpty() const
{
    return products.empty();
}

// Возвращает количество продуктов в приеме пищи
std::size_t Meal::size() const
{
    return products.size();
}

// Возвращает константную ссылку на список продуктов (для чтения)
const std::vector<Product> &Meal::getProducts() const
{
    return products;
}

// Возвращает константную ссылку на список граммовок (для чтения)
const std::vector<double> &Meal::getGrams() const
{
    return grams;
}

// Считает и возвращает общее количество калорий в приеме пищи
double Meal::getCalories() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcCalories(grams[i]);
    return sum;
}

// Считает и возвращает общее количество белков в приеме пищи
double Meal::getProteins() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcProteins(grams[i]);
    return sum;
}

// Считает и возвращает общее количество жиров в приеме пищи
double Meal::getFats() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcFats(grams[i]);
    return sum;
}

// Считает и возвращает общее количество углеводов в приеме пищи
double Meal::getCarbs() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcCarbs(grams[i]);
    return sum;
}

// Считает и возвращает общее количество клетчатки в приеме пищи
double Meal::getFiber() const
{
    double sum = 0.0;
    for (std::size_t i = 0; i < products.size(); ++i)
        sum += products[i].calcFiber(grams[i]);
    return sum;
}

// Выводит информацию о приеме пищи (список продуктов и итоговые БЖУ) в консоль
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

// Сохраняет прием пищи (тип, продукты и граммовки) в бинарный файл
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

// Загружает прием пищи из бинарного файла (с защитой от поврежденных данных)
void Meal::load(std::ifstream &in)
{
    products.clear();
    grams.clear();

    int t = 0;
    in.read(reinterpret_cast<char *>(&t), sizeof(int));

    // Проверка на корректность типа приема пищи
    if (t < 0 || t > 3)
        t = 0; // повреждённое значение, ставим завтрак
    type = static_cast<MealType>(t);

    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));

    // Защита от слишком большого количества продуктов
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