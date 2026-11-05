#include "TestMenu.hpp"
#include "PhysicalActivity.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>

// Вспомогательные методы

int TestMenu::readInt(const std::string &prompt, int min, int max) const
{
    while (true)
    {
        std::cout << prompt;
        int v = 0;
        if (std::cin >> v)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
            if (v >= min && v <= max)
                return v;
            std::cout << "Ошибка: введите число от " << min << " до " << max << ".\n";
        }
        else
        {
            // Конец ввода (Ctrl+Z / Ctrl+D): выходим, чтобы не зациклиться
            if (std::cin.eof())
                return min;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    }
}

DiaryEntry *TestMenu::findEntry(const Date &d)
{
    for (auto &e : entries)
    {
        if (e.getDate() == d)
            return &e;
    }
    return nullptr;
}

MealType TestMenu::chooseMealType() const
{
    int c = readInt("Приём пищи (1 - завтрак, 2 - обед, 3 - ужин, 4 - перекус): ", 1, 4);
    return static_cast<MealType>(c - 1);
}

bool TestMenu::chooseProduct(Product &result)
{
    int n = static_cast<int>(products.size());

    std::cout << "Продукты, созданные в этой сессии:\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << "  " << i + 1 << ") ";
        products[static_cast<std::size_t>(i)].printShort();
    }
    std::cout << "  " << n + 1 << ") создать новый продукт\n";
    std::cout << "  0) отмена\n";

    int c = readInt("Выбор: ", 0, n + 1);
    if (c == 0)
        return false;

    if (c <= n)
    {
        result = products[static_cast<std::size_t>(c - 1)];
        return true;
    }

    // Новый продукт "на лету"
    Product p("", 0.0, 0.0, 0.0, 0.0);
    p.inputFromConsole();
    if (p.getName().empty())
    {
        std::cout << "Ошибка: у продукта пустое название.\n";
        return false;
    }

    bool exists = false;
    for (const auto &q : products)
    {
        if (q == p)
            exists = true;
    }
    if (!exists)
        products.push_back(p);

    result = p;
    return true;
}

bool TestMenu::chooseActivity(PhysicalActivity &result) const
{
    const std::vector<PhysicalActivity> &list = catalog.getAll();
    if (list.empty())
    {
        std::cout << "Каталог активностей пуст.\n";
        return false;
    }

    for (std::size_t i = 0; i < list.size(); ++i)
    {
        std::cout << "  " << i + 1 << ") ";
        list[i].printShort();
    }
    std::cout << "  0) отмена\n";

    int c = readInt("Выбор: ", 0, static_cast<int>(list.size()));
    if (c == 0)
        return false;

    result = list[static_cast<std::size_t>(c - 1)];
    return true;
}

// Главное меню

void TestMenu::run()
{
    while (true)
    {
        std::cout << "\n===== ТЕСТОВОЕ МЕНЮ =====\n"
                  << "1. Каталог активностей\n"
                  << "2. Дневник: запись за дату\n"
                  << "3. Все записи дневника\n"
                  << "4. Сохранить дневник в файл\n"
                  << "5. Загрузить дневник из файла\n"
                  << "0. Выход\n";

        int c = readInt("Выбор: ", 0, 5);
        if (c == 0)
            return;

        switch (c)
        {
        case 1:
            activityMenu();
            break;
        case 2:
            diaryMenu();
            break;
        case 3:
            showAllEntries();
            break;
        case 4:
            saveDiary();
            break;
        case 5:
            loadDiary();
            break;
        }
    }
}

// Каталог активностей

void TestMenu::activityMenu()
{
    while (true)
    {
        std::cout << "\n--- Каталог активностей ---\n"
                  << "1. Показать все\n"
                  << "2. Добавить\n"
                  << "3. Удалить по названию\n"
                  << "4. Сортировать по алфавиту\n"
                  << "5. Сохранить каталог в файл\n"
                  << "6. Посчитать расход калорий\n"
                  << "0. Назад\n";

        int c = readInt("Выбор: ", 0, 6);
        if (c == 0)
            return;

        if (c == 1)
        {
            catalog.printAll();
        }
        else if (c == 2)
        {
            PhysicalActivity a;
            a.inputFromConsole();
            if (a.getName().empty())
                std::cout << "Ошибка: пустое название.\n";
            else if (catalog.addActivity(a))
                std::cout << "Добавлено.\n";
            else
                std::cout << "Такая активность уже есть.\n";
        }
        else if (c == 3)
        {
            std::string name;
            std::cout << "Название для удаления: ";
            std::getline(std::cin >> std::ws, name);
            std::cout << (catalog.removeActivity(name) ? "Удалено.\n" : "Не найдено.\n");
        }
        else if (c == 4)
        {
            catalog.sortAlphabetically();
            catalog.printAll();
        }
        else if (c == 5)
        {
            catalog.saveToFile();
            std::cout << "Сохранено.\n";
        }
        else if (c == 6)
        {
            calcActivityKcal();
        }
    }
}

void TestMenu::calcActivityKcal()
{
    PhysicalActivity a;
    if (!chooseActivity(a))
        return;

    a.inputWorkoutFromConsole();
    a.print();
    std::cout << "Вес для расчёта: " << weightKg << " кг\n";
    std::cout << "Расход: " << a.calcKcal(weightKg) << " ккал\n";
}

// Дневник

void TestMenu::diaryMenu()
{
    Date d(1, 1, 2000);
    d.inputFromConsole();

    DiaryEntry *entry = findEntry(d);
    if (entry == nullptr)
    {
        entries.push_back(DiaryEntry(d));
        entry = &entries.back();
        std::cout << "Создана новая запись за " << d.toString() << ".\n";
    }
    else
    {
        std::cout << "Открыта существующая запись за " << d.toString() << ".\n";
    }

    entryMenu(*entry);
}

void TestMenu::entryMenu(DiaryEntry &entry)
{
    while (true)
    {
        std::cout << "\n--- Запись за " << entry.getDate().toString() << " ---\n"
                  << "1. Добавить продукт в приём пищи\n"
                  << "2. Удалить продукт из приёма пищи\n"
                  << "3. Ввести количество воды\n"
                  << "4. Добавить воды к уже выпитому\n"
                  << "5. Ввести сон\n"
                  << "6. Добавить физическую активность\n"
                  << "7. Удалить физическую активность\n"
                  << "8. Показать запись\n"
                  << "0. Назад\n";

        int c = readInt("Выбор: ", 0, 8);
        if (c == 0)
            return;

        if (c == 1)
        {
            MealType t = chooseMealType();
            Product p("", 0.0, 0.0, 0.0, 0.0);
            if (chooseProduct(p))
                entry.getMeal(t).addProductFromConsole(p);
        }
        else if (c == 2)
        {
            MealType t = chooseMealType();
            entry.getMeal(t).print();
            std::string name;
            std::cout << "Название продукта для удаления: ";
            std::getline(std::cin >> std::ws, name);
            std::cout << (entry.getMeal(t).removeProduct(name) ? "Удалено.\n" : "Не найдено.\n");
        }
        else if (c == 3)
        {
            entry.getWater().inputFromConsole();
        }
        else if (c == 4)
        {
            WaterRecord extra;
            extra.inputFromConsole();
            entry.getWater().add(extra.getAmount());
            entry.getWater().print(waterNorm);
        }
        else if (c == 5)
        {
            entry.getSleep().inputFromConsole();
        }
        else if (c == 6)
        {
            PhysicalActivity a;
            if (chooseActivity(a))
            {
                a.inputWorkoutFromConsole();
                entry.addActivity(a);
            }
        }
        else if (c == 7)
        {
            std::string name;
            std::cout << "Название активности для удаления: ";
            std::getline(std::cin >> std::ws, name);
            std::cout << (entry.removeActivity(name) ? "Удалено.\n" : "Не найдено.\n");
        }
        else if (c == 8)
        {
            entry.print(waterNorm, sleepNorm, weightKg);
        }
    }
}

void TestMenu::showAllEntries() const
{
    if (entries.empty())
    {
        std::cout << "Пока нет записей. Начни отслеживать свои калории!\n";
        return;
    }

    // Сортируем копию по дате (проверка DiaryEntry::operator<)
    std::vector<DiaryEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end());

    for (const auto &e : sorted)
    {
        e.print(waterNorm, sleepNorm, weightKg);
    }
}

// Файл дневника

void TestMenu::saveDiary() const
{
    std::ofstream out(diaryFile, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cout << "Не удалось открыть файл для записи: " << diaryFile << "\n";
        return;
    }

    std::size_t count = entries.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));
    for (const auto &e : entries)
    {
        e.save(out);
    }
    std::cout << "Сохранено записей: " << count << "\n";
}

void TestMenu::loadDiary()
{
    std::ifstream in(diaryFile, std::ios::binary);
    if (!in.is_open())
    {
        std::cout << "Файл " << diaryFile << " не найден.\n";
        return;
    }

    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));
    if (!in || count > 100000)
    {
        std::cout << "Файл повреждён.\n";
        return;
    }

    std::vector<DiaryEntry> loaded;
    for (std::size_t i = 0; i < count; ++i)
    {
        DiaryEntry e;
        e.load(in);
        if (!in)
        {
            std::cout << "Файл повреждён, загрузка отменена.\n";
            return;
        }
        loaded.push_back(e);
    }

    entries = loaded;
    std::cout << "Загружено записей: " << count << "\n";
}