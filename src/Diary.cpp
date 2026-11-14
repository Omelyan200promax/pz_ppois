#include "Diary.hpp"
#include <iostream>
#include <algorithm>

// Конструктор: создает дневник и автоматически загружает данные из файла
Diary::Diary(const std::string &file) : filename(file)
{
    loadFromFile();
}

// Загружает все записи из бинарного файла
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

// Сохраняет все записи дневника в бинарный файл
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

// Ищет запись по дате. Если находит — возвращает ссылку на неё.
// Если нет — создает новую запись с этой датой и возвращает ссылку на неё.
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

// Удаляет запись дневника по указанной дате
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

// Сортирует все записи дневника по дате (от старых к новым)
void Diary::sortByDate()
{
    // Используем operator< из DiaryEntry (сравнивает по дате)
    std::sort(entries.begin(), entries.end());
}

// Выводит краткий список всех дат, за которые есть записи
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

// Проверяет, пуст ли дневник (нет ни одной записи)
bool Diary::isEmpty() const
{
    return entries.empty();
}

// Возвращает текущее количество записей в дневнике
std::size_t Diary::size() const
{
    return entries.size();
}

// Возвращает константную ссылку на вектор всех записей (для чтения)
const std::vector<DiaryEntry> &Diary::getAll() const
{
    return entries;
}

// Полностью очищает все записи дневника
void Diary::clear()
{
    entries.clear();
}