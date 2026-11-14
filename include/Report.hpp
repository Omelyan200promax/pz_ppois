#ifndef REPORT_HPP
#define REPORT_HPP

#include "Diary.hpp"
#include "UserProfile.hpp"
#include <iostream>

/*
 Класс Report формирует аналитический отчёт на основе данных из дневника
 и сравнивает их с нормами из профиля пользователя.
 */
class Report
{
public:
    // Тип вердикта
    enum class Verdict
    {
        FOLLOWS,    // Придерживается
        ALMOST,     // Почти придерживается
        NOT_FOLLOWS // Не придерживается
    };

    // Статический метод, который генерирует отчет
    static void generate(const Diary &diary, const UserProfile &user);

private:
    // Вспомогательный метод для определения вердикта
    static Verdict checkVerdict(double actual, double target, double tolerancePercent);

    // Получить название вердикта
    static std::string getVerdictName(Verdict v);
};

#endif // REPORT_HPP