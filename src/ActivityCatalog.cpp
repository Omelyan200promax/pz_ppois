#include "ActivityCatalog.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "PhysicalActivity.hpp"

// Конструктор: загружает данные из файла или задает базовый набор активностей
ActivityCatalog::ActivityCatalog(const std::string &file)
    : filename(file)
{
    // Если файла нет или он повреждён, загружаем базовый набор
    if (!loadFromFile())
    {
        fillDefaults();
    }
}

// Заполняет каталог базовым набором активностей по умолчанию
void ActivityCatalog::fillDefaults()
{
    activities.clear();

    // Низкая активность (MET 2.0 - 3.9)
    activities.push_back(PhysicalActivity("Ходьба", 3.0));
    activities.push_back(PhysicalActivity("Йога", 2.5));
    activities.push_back(PhysicalActivity("Пилатес", 3.0));
    activities.push_back(PhysicalActivity("Растяжка", 2.3));

    // Средняя активность (MET 4.0 - 7.9)
    activities.push_back(PhysicalActivity("Плавание", 6.0));
    activities.push_back(PhysicalActivity("Велосипед", 6.0));
    activities.push_back(PhysicalActivity("Танцы", 5.5));
    activities.push_back(PhysicalActivity("Силовая тренировка", 5.0));
    activities.push_back(PhysicalActivity("Домашняя уборка", 4.5));

    // Высокая активность (MET 8.0+)
    activities.push_back(PhysicalActivity("Бег", 9.8));
    activities.push_back(PhysicalActivity("Скакалка", 10.0));
    activities.push_back(PhysicalActivity("Бокс / Кикбоксинг", 9.0));
}

// Загружает каталог активностей из бинарного файла
bool ActivityCatalog::loadFromFile()
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        // Файла ещё нет, возвращаем false для загрузки дефолтных данных
        return false;
    }

    // Читаем количество записей
    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));

    // Защита от поврежденных данных (если count огромный, выделится много памяти)
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

// Сохраняет текущий каталог активностей в бинарный файл
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

// Добавляет новую активность в каталог (с проверкой на дубликаты)
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

// Удаляет активность из каталога по её названию
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

// Редактирует существующую активность (заменяет данные по названию)
bool ActivityCatalog::editActivity(const std::string &name,
                                   const PhysicalActivity &newData)
{
    PhysicalActivity *a = findByName(name);
    if (a == nullptr)
        return false;

    *a = newData;
    return true;
}

// Ищет активность по названию и возвращает указатель на неё (или nullptr)
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

// Сортирует активности в каталоге по алфавиту
void ActivityCatalog::sortAlphabetically()
{
    // Используем operator< из PhysicalActivity
    std::sort(activities.begin(), activities.end());
}

// Выводит список всех активностей в консоль
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

// Проверяет, пуст ли каталог активностей
bool ActivityCatalog::isEmpty() const
{
    return activities.empty();
}

// Возвращает текущее количество активностей в каталоге
std::size_t ActivityCatalog::size() const
{
    return activities.size();
}

// Возвращает константную ссылку на вектор всех активностей
const std::vector<PhysicalActivity> &ActivityCatalog::getAll() const
{
    return activities;
}