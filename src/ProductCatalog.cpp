#include "ProductCatalog.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>

// Конструктор: создает каталог и автоматически загружает данные из файла
ProductCatalog::ProductCatalog(const std::string &file)
    : filename(file)
{
    loadFromFile();
}

// Загружает каталог продуктов из бинарного файла
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

    // Проверка на корректность: размер файла должен соответствовать count записям
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

// Сохраняет каталог продуктов в бинарный файл
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

// Добавляет новый продукт в каталог (с проверкой на дубликаты по имени)
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

// Удаляет продукт из каталога по его названию
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

// Редактирует существующий продукт (заменяет данные по имени)
bool ProductCatalog::editProduct(const std::string &name,
                                 const Product &newData)
{
    Product *p = findByName(name);
    if (p == nullptr)
        return false;

    *p = newData;
    return true;
}

// Ищет продукт по точному названию и возвращает указатель на него (или nullptr)
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

// Ищет продукты по части названия (без учета регистра) и возвращает их список
std::vector<Product> ProductCatalog::searchByName(const std::string &query) const
{
    std::vector<Product> result;
    std::string lowerQuery = query;

    // Приводим запрос к нижнему регистру
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });

    for (const auto &p : products)
    {
        std::string pName = p.getName();
        // Приводим название продукта к нижнему регистру
        std::transform(pName.begin(), pName.end(), pName.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        // Если в названии есть искомая подстрока
        if (pName.find(lowerQuery) != std::string::npos)
        {
            result.push_back(p);
        }
    }
    return result;
}

// Сортирует продукты в каталоге по алфавиту
void ProductCatalog::sortAlphabetically()
{
    // Используем operator< из Product
    std::sort(products.begin(), products.end());
}

// Выводит список всех продуктов в консоль (компактный формат)
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

// Фильтрует продукты по диапазону калорийности на 100 г
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

// Фильтрует продукты по количеству белков (в диапазоне +- delta)
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

// Фильтрует продукты по количеству жиров (в диапазоне +- delta)
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

// Фильтрует продукты по количеству углеводов (в диапазоне +- delta)
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

// Проверяет, пуст ли каталог продуктов
bool ProductCatalog::isEmpty() const
{
    return products.empty();
}

// Возвращает текущее количество продуктов в каталоге
std::size_t ProductCatalog::size() const
{
    return products.size();
}

// Возвращает константную ссылку на вектор всех продуктов (для чтения)
const std::vector<Product> &ProductCatalog::getAll() const
{
    return products;
}