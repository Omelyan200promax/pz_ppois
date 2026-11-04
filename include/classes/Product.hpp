#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <fstream>
#include <iostream>

/*
 Класс Product описывает продукт питания или блюдо.
 Все пищевые показатели хранятся на 100 грамм продукта.
 Класс умеет сохраняться и загружаться в бинарный файл,
 а также пересчитывать БЖУ и калории на произвольную граммовку.
 */
class Product
{
private:
    // Фиксированный размер массива нужен для корректной
    // записи и чтения в бинарный файл, потому что std::string
    // нельзя писать в файл напрямую.
    static const int NAME_SIZE = 64;

    char name[NAME_SIZE]; // название продукта, до 63 символов плюс '\0'
    double calories;      // ккал на 100 г
    double proteins;      // белки на 100 г
    double fats;          // жиры на 100 г
    double carbs;         // углеводы на 100 г
    double fiber;         // клетчатка на 100 г

public:
    // Конструкторы
    Product(); // пустой продукт
    Product(const std::string &name,
            double calories,
            double proteins,
            double fats,
            double carbs,
            double fiber = 0.0);

    // Конструктор копирования и оператор присваивания
    // не нужны, компилятор сгенерирует корректные, так как поля простые.
    Product(const Product &) = default;
    Product &operator=(const Product &) = default;
    ~Product() = default;

    // Геттеры
    std::string getName() const;
    double getCalories() const; // на 100 г
    double getProteins() const;
    double getFats() const;
    double getCarbs() const;
    double getFiber() const;

    // Сеттеры
    void setName(const std::string &newName);
    void setCalories(double v);
    void setProteins(double v);
    void setFats(double v);
    void setCarbs(double v);
    void setFiber(double v);

    // Установить сразу все нутриенты
    void setNutrients(double calories,
                      double proteins,
                      double fats,
                      double carbs,
                      double fiber);

    // Пересчёт на граммовку.
    // grams это сколько грамм продукта съедено.
    // Возвращают значение с учётом граммовки.
    double calcCalories(double grams) const;
    double calcProteins(double grams) const;
    double calcFats(double grams) const;
    double calcCarbs(double grams) const;
    double calcFiber(double grams) const;

    // Сравнение.
    // Нужно для сортировки каталога по алфавиту.
    bool operator<(const Product &other) const;
    bool operator==(const Product &other) const; // сравнение по имени

    // Ввод и вывод.
    // Ввод с консоли нового продукта,
    // используется при добавлении в каталог.
    void inputFromConsole();

    // Красивый вывод в консоль, значения на 100 г.
    void print() const;

    // Компактный вывод для списка каталога.
    void printShort() const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);

    // Размер записи в файле, нужен для проверки целостности.
    static std::size_t binarySize();
};

#endif // PRODUCT_H