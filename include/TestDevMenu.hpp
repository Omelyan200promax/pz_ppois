#ifndef TESTDEVMENU_HPP
#define TESTDEVMENU_HPP

#include "UserProfile.hpp"
#include "ProductCatalog.hpp"
#include "ActivityCatalog.hpp"
#include "Diary.hpp"
#include "Calendar.hpp"
#include "Report.hpp"
#include <iostream>
#include <limits>

/*
 Класс TestDevMenu - временное меню для проверки классов разработчика 1:
 UserProfile, ProductCatalog, Diary, Calendar, Report.
 Позволяет проверить формулы, поиск, сохранение и связку всех частей.
 */
class TestDevMenu
{
private:
    UserProfile user;
    ProductCatalog productCatalog;
    ActivityCatalog activityCatalog;
    Diary diary;
    Calendar calendar{diary};

    // Безопасный ввод целого числа
    int readInt(const std::string &prompt, int min, int max) const;

    // Тесты
    void testUserProfile();
    void testProductCatalog();
    void testDiaryAndCalendar();
    void testReport();

public:
    void run();
};

#endif // TESTDEVMENU_HPP