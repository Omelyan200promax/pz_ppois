#ifndef DIARYENTRY_HPP
#define DIARYENTRY_HPP

#include "Date.hpp"
#include "Meal.hpp"
#include "WaterRecord.hpp"
#include "SleepRecord.hpp"
#include "PhysicalActivity.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

/*
 Класс DiaryEntry - запись дневника за один день.
 Объединяет дату, четыре приёма пищи (завтрак, обед, ужин, перекус),
 воду, сон и список выполненных физических активностей.
 Новая запись создаётся пустой, дальше её заполняют через геттеры.
 Нормы воды, сна и вес пользователя класс не хранит: они приходят
 параметрами из профиля пользователя (там, где они нужны).
 */
class DiaryEntry
{
private:
    Date date{1, 1, 2000}; // как у Date по умолчанию, но через конструктор с параметрами
    Meal breakfast{MealType::Breakfast};
    Meal lunch{MealType::Lunch};
    Meal dinner{MealType::Dinner};
    Meal snack{MealType::Snack};
    WaterRecord water;
    SleepRecord sleep;
    std::vector<PhysicalActivity> activities; // активности за день

public:
    // Конструкторы
    DiaryEntry() = default;
    explicit DiaryEntry(const Date &date_);

    // Дата
    const Date &getDate() const;
    void setDate(const Date &d);

    // Доступ к приёмам пищи, воде и сну.
    // Версии без const возвращают ссылку, через неё запись можно редактировать.
    Meal &getMeal(MealType t);
    const Meal &getMeal(MealType t) const;
    WaterRecord &getWater();
    const WaterRecord &getWater() const;
    SleepRecord &getSleep();
    const SleepRecord &getSleep() const;

    // Физические активности
    void addActivity(const PhysicalActivity &a);
    bool removeActivity(const std::string &name);
    const std::vector<PhysicalActivity> &getActivities() const;

    // Итоги за день по питанию
    double getTotalCalories() const;
    double getTotalProteins() const;
    double getTotalFats() const;
    double getTotalCarbs() const;
    double getTotalFiber() const;

    // Сожжено калорий за день (нужен вес пользователя в кг)
    double getBurnedKcal(double weightKg) const;

    // Запись пустая, если ничего не введено
    bool isEmpty() const;

    // Сравнение по дате (нужно для сортировки и поиска в календаре)
    bool operator==(const DiaryEntry &other) const;
    bool operator<(const DiaryEntry &other) const;

    // Вывод всей записи.
    // Нормы и вес необязательны: если значение > 0, оно показывается.
    void print(double waterNorm = 0.0, double sleepNorm = 0.0, double weightKg = 0.0) const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);
};

#endif // DIARYENTRY_HPP
