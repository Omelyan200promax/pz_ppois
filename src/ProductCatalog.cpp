#include "ProductCatalog.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>

// Конструктор

ProductCatalog::ProductCatalog(const std::string &file)
    : filename(file)
{
    loadFromFile();
}

// Загрузка и сохранение

void ProductCatalog::loadFromFile()
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        // Файла ещё нет, каталог остаётся пустым
        return;
    }

    products.clear();

    // Читаем количество записей
    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));

    // Проверка на корректность: размер файла должен
    // соответствовать count записям
    if (!in)
        return;

    products.resize(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        products[i].load(in);
        if (!in)
        {
            // Файл повреждён, очищаем каталог
            products.clear();
            return;
        }
    }
}

void ProductCatalog::saveToFile() const
{
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "Не удалось открыть файл каталога для записи: "
                  << filename << "\n";
        return;
    }

    // Сначала пишем количество записей
    std::size_t count = products.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Затем сами продукты
    for (const auto &p : products)
    {
        p.save(out);
    }
}

// Добавление, удаление, редактирование

bool ProductCatalog::addProduct(const Product &p)
{
    // Не допускаем дубликатов по имени
    if (findByName(p.getName()) != nullptr)
    {
        return false;
    }
    products.push_back(p);
    return true;
}

bool ProductCatalog::removeProduct(const std::string &name)
{
    for (auto it = products.begin(); it != products.end(); ++it)
    {
        if (it->getName() == name)
        {
            products.erase(it);
            return true;
        }
    }
    return false;
}

bool ProductCatalog::editProduct(const std::string &name,
                                 const Product &newData)
{
    Product *p = findByName(name);
    if (p == nullptr)
        return false;

    *p = newData;
    return true;
}

// Поиск

Product *ProductCatalog::findByName(const std::string &name)
{
    for (auto &p : products)
    {
        if (p.getName() == name)
        {
            return &p;
        }
    }
    return nullptr;
}

// Сортировка

void ProductCatalog::sortAlphabetically()
{
    // Используем operator< из Product
    std::sort(products.begin(), products.end());
}

// Вывод

void ProductCatalog::printAll() const
{
    if (products.empty())
    {
        std::cout << "Каталог пуст.\n";
        return;
    }

    std::cout << "Каталог продуктов (" << products.size() << " шт.):\n";
    for (const auto &p : products)
    {
        p.printShort();
    }
}

// Фильтры

std::vector<Product>
ProductCatalog::filterByCalories(double min, double max) const
{
    std::vector<Product> result;
    for (const auto &p : products)
    {
        if (p.getCalories() >= min && p.getCalories() <= max)
        {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product>
ProductCatalog::filterByProteins(double value, double delta) const
{
    std::vector<Product> result;
    for (const auto &p : products)
    {
        if (p.getProteins() >= value - delta &&
            p.getProteins() <= value + delta)
        {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product>
ProductCatalog::filterByFats(double value, double delta) const
{
    std::vector<Product> result;
    for (const auto &p : products)
    {
        if (p.getFats() >= value - delta &&
            p.getFats() <= value + delta)
        {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product>
ProductCatalog::filterByCarbs(double value, double delta) const
{
    std::vector<Product> result;
    for (const auto &p : products)
    {
        if (p.getCarbs() >= value - delta &&
            p.getCarbs() <= value + delta)
        {
            result.push_back(p);
        }
    }
    return result;
}

// Вспомогательные методы

bool ProductCatalog::isEmpty() const
{
    return products.empty();
}

std::size_t ProductCatalog::size() const
{
    return products.size();
}

const std::vector<Product> &ProductCatalog::getAll() const
{
    return products;
}