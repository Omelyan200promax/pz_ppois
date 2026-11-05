#include "DiaryEntry.hpp"
#include "PhysicalActivity.h"
#include <iomanip> // std::fixed, std::setprecision

// Конструкторы

DiaryEntry::DiaryEntry(const Date &date_) : date(date_)
{
}

// Дата

const Date &DiaryEntry::getDate() const { return date; }

void DiaryEntry::setDate(const Date &d) { date = d; }

// Доступ к частям записи

Meal &DiaryEntry::getMeal(MealType t)
{
    switch (t)
    {
    case MealType::Breakfast:
        return breakfast;
    case MealType::Lunch:
        return lunch;
    case MealType::Dinner:
        return dinner;
    case MealType::Snack:
        return snack;
    }
    return breakfast;
}

const Meal &DiaryEntry::getMeal(MealType t) const
{
    switch (t)
    {
    case MealType::Breakfast:
        return breakfast;
    case MealType::Lunch:
        return lunch;
    case MealType::Dinner:
        return dinner;
    case MealType::Snack:
        return snack;
    }
    return breakfast;
}

WaterRecord &DiaryEntry::getWater() { return water; }
const WaterRecord &DiaryEntry::getWater() const { return water; }

SleepRecord &DiaryEntry::getSleep() { return sleep; }
const SleepRecord &DiaryEntry::getSleep() const { return sleep; }

// Физические активности

void DiaryEntry::addActivity(const PhysicalActivity &a)
{
    activities.push_back(a);
}

bool DiaryEntry::removeActivity(const std::string &name)
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

const std::vector<PhysicalActivity> &DiaryEntry::getActivities() const
{
    return activities;
}

// Итоги за день

double DiaryEntry::getTotalCalories() const
{
    return breakfast.getCalories() + lunch.getCalories() +
           dinner.getCalories() + snack.getCalories();
}

double DiaryEntry::getTotalProteins() const
{
    return breakfast.getProteins() + lunch.getProteins() +
           dinner.getProteins() + snack.getProteins();
}

double DiaryEntry::getTotalFats() const
{
    return breakfast.getFats() + lunch.getFats() +
           dinner.getFats() + snack.getFats();
}

double DiaryEntry::getTotalCarbs() const
{
    return breakfast.getCarbs() + lunch.getCarbs() +
           dinner.getCarbs() + snack.getCarbs();
}

double DiaryEntry::getTotalFiber() const
{
    return breakfast.getFiber() + lunch.getFiber() +
           dinner.getFiber() + snack.getFiber();
}

double DiaryEntry::getBurnedKcal(double weightKg) const
{
    double sum = 0.0;
    for (const auto &a : activities)
    {
        sum += a.calcKcal(weightKg);
    }
    return sum;
}

bool DiaryEntry::isEmpty() const
{
    return breakfast.isEmpty() && lunch.isEmpty() &&
           dinner.isEmpty() && snack.isEmpty() &&
           water.getAmount() == 0.0 && sleep.getHours() == 0.0 &&
           activities.empty();
}

// Сравнение по дате

bool DiaryEntry::operator==(const DiaryEntry &other) const
{
    return date == other.date;
}

bool DiaryEntry::operator<(const DiaryEntry &other) const
{
    return date < other.date;
}

// Вывод в консоль

void DiaryEntry::print(double waterNorm, double sleepNorm, double weightKg) const
{
    std::cout << "==============================\n";
    std::cout << "Запись за " << date.toString() << "\n";
    std::cout << "==============================\n";

    breakfast.print();
    lunch.print();
    dinner.print();
    snack.print();

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "------------------------------\n";
    std::cout << "Всего за день: " << getTotalCalories() << " ккал | "
              << "Б " << getTotalProteins() << "  "
              << "Ж " << getTotalFats() << "  "
              << "У " << getTotalCarbs() << "  "
              << "Кл " << getTotalFiber() << "\n";

    water.print(waterNorm);
    sleep.print(sleepNorm);

    std::cout << "Физическая активность:\n";
    if (activities.empty())
    {
        std::cout << "  (нет)\n";
    }
    for (const auto &a : activities)
    {
        std::cout << "  " << a.getName() << ", " << a.getMinutes() << " мин, "
                  << "интенсивность: " << intensityName(a.getIntensity());
        if (weightKg > 0.0)
            std::cout << " - " << a.calcKcal(weightKg) << " ккал";
        std::cout << "\n";
    }
    if (weightKg > 0.0 && !activities.empty())
        std::cout << "  Сожжено всего: " << getBurnedKcal(weightKg) << " ккал\n";

    std::cout << "==============================\n";
}

// Бинарный файл

void DiaryEntry::save(std::ofstream &out) const
{
    date.save(out);
    breakfast.save(out);
    lunch.save(out);
    dinner.save(out);
    snack.save(out);
    water.save(out);
    sleep.save(out);

    // Количество активностей, затем сами активности
    std::size_t count = activities.size();
    out.write(reinterpret_cast<const char *>(&count), sizeof(count));
    for (const auto &a : activities)
    {
        a.save(out);
    }
}

void DiaryEntry::load(std::ifstream &in)
{
    date.load(in);
    breakfast.load(in);
    lunch.load(in);
    dinner.load(in);
    snack.load(in);
    water.load(in);
    sleep.load(in);

    activities.clear();
    std::size_t count = 0;
    in.read(reinterpret_cast<char *>(&count), sizeof(count));
    if (!in || count > 100000)
        return;

    for (std::size_t i = 0; i < count; ++i)
    {
        PhysicalActivity a;
        a.load(in);
        if (!in)
        {
            // Файл повреждён, очищаем список активностей
            activities.clear();
            return;
        }
        activities.push_back(a);
    }
}
