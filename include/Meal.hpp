#ifndef MEAL_HPP
#define MEAL_HPP

#include "Product.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// Тип приёма пищи
enum class MealType
{
    Breakfast = 0,
    Lunch = 1,
    Dinner = 2,
    Snack = 3
};

// Название приёма пищи: "Завтрак" / "Обед" / "Ужин" / "Перекус"
std::string mealTypeName(MealType t);

/*
 Класс Meal описывает один приём пищи (завтрак, обед, ужин или перекус).
 Хранит список съеденных продуктов и граммовку каждого.
 Продукт копируется в приём пищи вместе со своими значениями на 100 г,
 поэтому последующее изменение каталога не меняет старые записи дневника.
 Итоговые калории и БЖУ считаются через методы пересчёта Product
 */
class Meal
{
private:
    MealType type = MealType::Breakfast;
    std::vector<Product> products; // съеденные продукты
    std::vector<double> grams;     // граммовка, grams[i] относится к products[i]

public:
    // Конструкторы
    Meal() = default;
    explicit Meal(MealType type_);

    // Геттер и сеттер типа
    MealType getType() const;
    void setType(MealType t);

    // Добавить продукт с граммовкой.
    // Если такой продукт уже есть (по названию), граммы суммируются.
    bool addProduct(const Product &p, double g);

    // Добавить продукт, спросив граммовку у пользователя
    bool addProductFromConsole(const Product &p);

    // Удалить продукт по названию
    bool removeProduct(const std::string &name);

    // Очистить приём пищи
    void clear();

    // Проверка на пустоту и количество позиций
    bool isEmpty() const;
    std::size_t size() const;

    // Чтение без копирования
    const std::vector<Product> &getProducts() const;
    const std::vector<double> &getGrams() const;

    // Итоги по приёму пищи (с учётом граммовки)
    double getCalories() const;
    double getProteins() const;
    double getFats() const;
    double getCarbs() const;
    double getFiber() const;

    // Вывод в консоль
    void print() const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);
};

#endif // MEAL_HPP
