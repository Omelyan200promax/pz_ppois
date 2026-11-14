#include "Diary.hpp"
#include <iostream>
#include <algorithm>

// Конструктор

Diary::Diary(const std::string &file) : filename(file)
{
    loadFromFile();
}

// Загрузка и сохранение

void Diary::loadFromFile()
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        // Файла ещё нет, дневник остаётся пустым
        return;
    }

    entries.clear();

    // Читаем количество записей
    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));

    if (!in)
        return;

    entries.resize(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        entries[i].load(in);
        if (!in)
        {
            // Файл повреждён, очищаем дневник
            entries.clear();
            return;
        }
    }
}

void Diary::saveToFile() const
{
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "Не удалось открыть файл дневника для записи: "
                  << filename << "\n";
        return;
    }

    // Сначала пишем количество записей
    std::size_t count = entries.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));

    // Затем сами записи
    for (const auto &e : entries)
    {
        e.save(out);
    }
}

// Поиск или создание записи

DiaryEntry &Diary::getOrCreateEntry(const Date &date)
{
    // Ищем запись с такой датой
    for (auto &e : entries)
    {
        if (e.getDate() == date)
        {
            return e;
        }
    }

    // Не нашли — создаём новую и добавляем
    entries.emplace_back(date);
    return entries.back();
}

// Удаление

bool Diary::removeEntry(const Date &date)
{
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        if (it->getDate() == date)
        {
            entries.erase(it);
            return true;
        }
    }
    return false;
}

// Сортировка

void Diary::sortByDate()
{
    // Используем operator< из DiaryEntry (сравнивает по дате)
    std::sort(entries.begin(), entries.end());
}

// Вывод

void Diary::printAll() const
{
    if (entries.empty())
    {
        std::cout << "Дневник пуст.\n";
        return;
    }

    std::cout << "Записи дневника (" << entries.size() << " шт.):\n";
    for (const auto &e : entries)
    {
        std::cout << "  " << e.getDate().toString();
        if (e.isEmpty())
        {
            std::cout << " [пусто]";
        }
        std::cout << "\n";
    }
}

// Вспомогательные методы

bool Diary::isEmpty() const
{
    return entries.empty();
}

std::size_t Diary::size() const
{
    return entries.size();
}

const std::vector<DiaryEntry> &Diary::getAll() const
{
    return entries;
}