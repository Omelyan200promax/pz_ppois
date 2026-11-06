#ifndef TESTMENU_HPP
#define TESTMENU_HPP

#include "ActivityCatalog.hpp"
#include "DiaryEntry.hpp"
#include "Product.hpp"
#include <string>
#include <vector>

/*
 Класс TestMenu - временное консольное меню для проверки классов:
 PhysicalActivity, ActivityCatalog, Meal, WaterRecord, SleepRecord, DiaryEntry.
 Это не итоговое меню проекта. Данные профиля пользователя здесь заменены
 константами (вес, нормы воды и сна), а каталога продуктов нет:
 продукты создаются на лету и запоминаются только на время работы программы
 */
class TestMenu
{
private:
    ActivityCatalog catalog{"activities.bin"};
    std::vector<DiaryEntry> entries; // записи дневника за разные даты
    std::vector<Product> products;   // продукты, созданные в этой сессии
    std::string diaryFile = "diary_test.bin";

    // Заглушки вместо профиля пользователя
    double weightKg = 70.0;
    double waterNorm = 2000.0;
    double sleepNorm = 8.0;

    // Безопасный ввод целого числа от min до max (0 используется как "назад")
    int readInt(const std::string &prompt, int min, int max) const;

    // Подменю
    void activityMenu();
    void diaryMenu();
    void entryMenu(DiaryEntry &entry);

    // Действия
    void showAllEntries() const;
    void saveDiary() const;
    void loadDiary();
    void calcActivityKcal();
    MealType chooseMealType() const;
    bool chooseProduct(Product &result);
    bool chooseActivity(PhysicalActivity &result) const;
    DiaryEntry *findEntry(const Date &d);

public:
    // Главный цикл меню
    void run();
};

#endif // TESTMENU_HPP
