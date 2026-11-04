#ifndef PRODUCTCATALOG_H
#define PRODUCTCATALOG_H

#include "Product.hpp"
#include <vector>
#include <string>

/*
 Класс ProductCatalog хранит список продуктов и умеет
 загружать и сохранять его в бинарный файл, а также
 выполнять поиск, сортировку и фильтрацию.
 */
class ProductCatalog
{
private:
    std::vector<Product> products;
    std::string filename;

public:
    // Конструктор принимает имя файла, по умолчанию catalog.bin
    explicit ProductCatalog(const std::string &file = "catalog.bin");

    // Загрузка и сохранение бинарного файла
    void loadFromFile();
    void saveToFile() const;

    // Добавление, удаление, редактирование
    bool addProduct(const Product &p);
    bool removeProduct(const std::string &name);
    bool editProduct(const std::string &name, const Product &newData);

    // Поиск по имени, возвращает указатель или nullptr
    Product *findByName(const std::string &name);

    // Сортировка по алфавиту
    void sortAlphabetically();

    // Вывод всего каталога
    void printAll() const;

    // Фильтры по БЖУ и калориям
    std::vector<Product> filterByCalories(double min, double max) const;
    std::vector<Product> filterByProteins(double value, double delta = 5.0) const;
    std::vector<Product> filterByFats(double value, double delta = 3.0) const;
    std::vector<Product> filterByCarbs(double value, double delta = 7.0) const;

    // Проверка на пустоту и размер
    bool isEmpty() const;
    std::size_t size() const;

    // Геттер для чтения без копирования
    const std::vector<Product> &getAll() const;
};

#endif // PRODUCTCATALOG_H