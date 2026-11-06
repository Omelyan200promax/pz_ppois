#include "ActivityCatalog.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "PhysicalActivity.hpp"
// Конструктор

ActivityCatalog::ActivityCatalog(const std::string &file)
    : filename(file)
{
    // Нет файла или он повреждён - берём базовый набор
    if (!loadFromFile())
    {
        fillDefaults();
    }
}

// Заполнение по умолчанию

void ActivityCatalog::fillDefaults()
{
    activities.clear();
    activities.push_back(PhysicalActivity("Ходьба", 3.5));
    activities.push_back(PhysicalActivity("Бег", 9.8));
    activities.push_back(PhysicalActivity("Плавание", 6.0));
    activities.push_back(PhysicalActivity("Велосипед", 7.5));
    activities.push_back(PhysicalActivity("Йога", 2.5));
    activities.push_back(PhysicalActivity("Силовая тренировка", 5.0));
}

// Загрузка и сохранение

bool ActivityCatalog::loadFromFile()
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        // Файла ещё нет
        return false;
    }

    // Читаем количество записей
    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));
    if (!in || count > 100000)
        return false;

    std::vector<PhysicalActivity> loaded(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        loaded[i].load(in);
        if (!in)
        {
            // Файл повреждён, старые данные не трогаем
            return false;
        }
    }

    activities = loaded;
    return true;
}

void ActivityCatalog::saveToFile() const
{
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "Не удалось открыть файл каталога активностей для записи: "
                  << filename << "\n";
        return;
    }

    // Сначала пишем количество записей
    std::size_t count = activities.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Затем сами активности
    for (const auto &a : activities)
    {
        a.save(out);
    }
}

// Добавление, удаление, редактирование

bool ActivityCatalog::addActivity(const PhysicalActivity &a)
{
    // Не допускаем дубликатов по названию
    if (findByName(a.getName()) != nullptr)
    {
        return false;
    }
    activities.push_back(a);
    return true;
}

bool ActivityCatalog::removeActivity(const std::string &name)
{
    for (auto it = activities.begin(); it != activities.end(); ++it)
    {
        if (it->getName() == name)
        {
            activities.erase(it);
            return true;
        }
    }
    return false;
}

bool ActivityCatalog::editActivity(const std::string &name,
                                   const PhysicalActivity &newData)
{
    PhysicalActivity *a = findByName(name);
    if (a == nullptr)
        return false;

    *a = newData;
    return true;
}

// Поиск

PhysicalActivity *ActivityCatalog::findByName(const std::string &name)
{
    for (auto &a : activities)
    {
        if (a.getName() == name)
        {
            return &a;
        }
    }
    return nullptr;
}

// Сортировка

void ActivityCatalog::sortAlphabetically()
{
    // Используем operator< из PhysicalActivity
    std::sort(activities.begin(), activities.end());
}

// Вывод

void ActivityCatalog::printAll() const
{
    if (activities.empty())
    {
        std::cout << "Каталог активностей пуст.\n";
        return;
    }

    std::cout << "Каталог активностей (" << activities.size() << " шт.):\n";
    for (const auto &a : activities)
    {
        a.printShort();
    }
}

// Вспомогательные методы

bool ActivityCatalog::isEmpty() const
{
    return activities.empty();
}

std::size_t ActivityCatalog::size() const
{
    return activities.size();
}

const std::vector<PhysicalActivity> &ActivityCatalog::getAll() const
{
    return activities;
}