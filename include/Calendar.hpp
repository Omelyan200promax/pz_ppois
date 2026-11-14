#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include "Diary.hpp"
#include "Date.hpp"
#include "DiaryEntry.hpp"
#include <iostream>

/*
 Класс Calendar - это архив записей дневника.
 Он не хранит данные сам, а работает как "вид" на класс Diary.
 Позволяет просматривать записи по датам, искать их, удалять
 и получать конкретную запись для редактирования.
 */
class Calendar
{
private:
    Diary &diary; // ссылка на дневник, откуда берутся все данные

public:
    explicit Calendar(Diary &d);

    // Вывод всех записей, отсортированных по дате
    void printAll() const;

    // Поиск и вывод записи на конкретную дату
    void searchByDate(const Date &date) const;

    // Удаление записи по дате
    bool deleteByDate(const Date &date);

    // Получить запись для редактирования (возвращает указатель или nullptr)
    DiaryEntry *getEntryForEditing(const Date &date);
};

#endif // CALENDAR_HPP