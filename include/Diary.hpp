#ifndef DIARY_HPP
#define DIARY_HPP

#include "DiaryEntry.hpp"
#include "Date.hpp"
#include <vector>
#include <string>
#include <fstream>

class Diary
{
private:
    std::vector<DiaryEntry> entries;
    std::string filename = "diary.bin";

public:
    explicit Diary(const std::string &file = "diary.bin");

    // Загрузка и сохранение
    void loadFromFile();
    void saveToFile() const;

    // Основной метод: найти запись по дате или создать новую.
    // Возвращает ссылку, чтобы меню могло сразу её редактировать.
    DiaryEntry &getOrCreateEntry(const Date &date);

    // Удаление записи по дате
    bool removeEntry(const Date &date);

    // Сортировка по дате
    void sortByDate();

    // Вывод всех записей
    void printAll() const;

    // Проверка на пустоту и размер
    bool isEmpty() const;
    std::size_t size() const;

    // Геттер для чтения (без копирования)
    const std::vector<DiaryEntry> &getAll() const;
};

#endif // DIARY_HPP