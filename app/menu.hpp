#ifndef MENU_HPP
#define MENU_HPP

#include "UserProfile.hpp"
#include "ProductCatalog.hpp"
#include "ActivityCatalog.hpp"
#include "Diary.hpp"
#include "Calendar.hpp"
#include "Report.hpp"
#include <string>
#include <fstream>

class Menu
{
private:
    UserProfile user;
    ProductCatalog productCatalog;
    ActivityCatalog activityCatalog;
    Diary diary;
    Calendar calendar{diary};
    bool running = true;

    // Вспомогательные методы
    int readInt(const std::string &prompt, int min, int max) const;
    double readDouble(const std::string &prompt, double min, double max) const;
    
    void showInitialSetup();
    void recalculateActivityAverage(); // Пересчет среднего расхода на тренировках
    int calculateStreak();             // Подсчет дней подряд

    // Пункты меню
    void showMainMenu();
    void showPersonalData();
    void showDiary();
    void showProductCatalog();
    void showCalendar();
    void showNewEntry();
    void showReport();
    void showResult();
    
    // Подменю дневника
    void showDiaryEntryMenu(DiaryEntry &entry);
    void addProductToMeal(DiaryEntry &entry);
    public:
    void run(); 
};

#endif // MENU_HPP