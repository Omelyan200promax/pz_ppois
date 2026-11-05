#ifndef ACTIVITYCATALOG_HPP
#define ACTIVITYCATALOG_HPP

#include "PhysicalActivity.hpp"
#include <vector>
#include <string>

/*
 Класс ActivityCatalog хранит справочник физических активностей
 (бег, плавание и т.д.) и умеет загружать и сохранять его в бинарный файл,
 а также искать, добавлять, редактировать, удалять и сортировать записи.
 Если файла каталога ещё нет, заполняется базовым набором активностей.
 */
class ActivityCatalog
{
private:
    std::vector<PhysicalActivity> activities;
    std::string filename = "activities.bin";

    // Заполнение базовым набором активностей
    void fillDefaults();

public:
    // Конструктор принимает имя файла, по умолчанию activities.bin
    explicit ActivityCatalog(const std::string &file = "activities.bin");

    // Загрузка и сохранение бинарного файла.
    // loadFromFile возвращает false, если файла нет или он повреждён.
    bool loadFromFile();
    void saveToFile() const;

    // Добавление, удаление, редактирование
    bool addActivity(const PhysicalActivity &a);
    bool removeActivity(const std::string &name);
    bool editActivity(const std::string &name, const PhysicalActivity &newData);

    // Поиск по названию, возвращает указатель или nullptr
    PhysicalActivity *findByName(const std::string &name);

    // Сортировка по алфавиту
    void sortAlphabetically();

    // Вывод всего каталога
    void printAll() const;

    // Проверка на пустоту и размер
    bool isEmpty() const;
    std::size_t size() const;

    // Геттер для чтения без копирования
    const std::vector<PhysicalActivity> &getAll() const;
};

#endif // ACTIVITYCATALOG_HPP
