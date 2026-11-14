#include "Report.hpp"
#include <iomanip>
#include <cmath> // для std::abs

// Генерирует отчет на основе данных дневника и норм пользователя.
// Проверяет наличие минимум 10 дней записей, считает средние значения (исключая пустые дни),
// выводит вердикты, прогноз веса и персональные рекомендации.
void Report::generate(const Diary &diary, const UserProfile &user)
{
    const auto &entries = diary.getAll();

    // 1. Проверяем, достаточно ли дней
    if (entries.size() < 10)
    {
        std::cout << "Ошибка: для отчёта нужно минимум 10 дней записей.\n";
        std::cout << "У вас пока " << entries.size() << " дней.\n";
        return;
    }

    // 2. Считаем средние значения (только для заполненных дней)
    double totalCalories = 0.0;
    double totalProteins = 0.0;
    double totalFats = 0.0;
    double totalCarbs = 0.0;
    double totalActivity = 0.0;

    int recordedDays = 0; // Дни, где хоть что-то введено
    int emptyDays = 0;    // Дни, где пусто (пропуски)

    for (const auto &entry : entries)
    {
        if (entry.isEmpty())
        {
            emptyDays++;
            continue;
        }

        totalCalories += entry.getTotalCalories();
        totalProteins += entry.getTotalProteins();
        totalFats += entry.getTotalFats();
        totalCarbs += entry.getTotalCarbs();
        totalActivity += entry.getBurnedKcal(user.getWeight());

        recordedDays++;
    }

    if (recordedDays == 0)
    {
        std::cout << "Ошибка: все записи пустые. Нечего анализировать.\n";
        return;
    }

    // Средние значения
    double avgCalories = totalCalories / recordedDays;
    double avgProteins = totalProteins / recordedDays;
    double avgFats = totalFats / recordedDays;
    double avgCarbs = totalCarbs / recordedDays;
    double avgActivity = totalActivity / recordedDays;

    // 3. Вывод шапки отчета
    std::cout << "========================================\n";
    std::cout << "             ВАШ ОТЧЁТ                  \n";
    std::cout << "========================================\n";
    std::cout << "Проанализировано дней: " << recordedDays << "\n";

    if (emptyDays > 0)
    {
        std::cout << "Пропущено (пустых) дней: " << emptyDays << "\n";
        std::cout << "(!) Внимание: данные могут быть неточными, так как вы разленились и пропускали дни.\n";
    }

    // 4. Вывод средних значений
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "----------------------------------------\n";
    std::cout << "Средний калораж:   " << avgCalories << " ккал (норма: " << user.getDailyCalories() << ")\n";
    std::cout << "Средние белки:     " << avgProteins << " г (норма: " << user.getDailyProteins() << ")\n";
    std::cout << "Средние жиры:      " << avgFats << " г (норма: " << user.getDailyFats() << ")\n";
    std::cout << "Средние углеводы:  " << avgCarbs << " г (норма: " << user.getDailyCarbs() << ")\n";
    std::cout << "Средняя активность:" << avgActivity << " ккал (норма: " << user.getOptimalActivityCal() << ")\n";
    std::cout << "----------------------------------------\n";

    // 5. Анализ соответствия (допуски: калории 10%, БЖУ 15%, активность 20%)
    Verdict vCal = checkVerdict(avgCalories, user.getDailyCalories(), 10.0);
    Verdict vProt = checkVerdict(avgProteins, user.getDailyProteins(), 15.0);
    Verdict vFat = checkVerdict(avgFats, user.getDailyFats(), 15.0);
    Verdict vCarb = checkVerdict(avgCarbs, user.getDailyCarbs(), 15.0);
    Verdict vAct = checkVerdict(avgActivity, user.getOptimalActivityCal(), 20.0);

    std::cout << "Вердикт по калориям:  " << getVerdictName(vCal) << "\n";
    std::cout << "Вердикт по белкам:    " << getVerdictName(vProt) << "\n";
    std::cout << "Вердикт по жирам:     " << getVerdictName(vFat) << "\n";
    std::cout << "Вердикт по углеводам: " << getVerdictName(vCarb) << "\n";
    std::cout << "Вердикт по активности:" << getVerdictName(vAct) << "\n";
    std::cout << "----------------------------------------\n";

    // 6. Общий вердикт
    int followsCount = 0;
    int almostCount = 0;
    Verdict allVerdicts[] = {vCal, vProt, vFat, vCarb, vAct};

    for (Verdict v : allVerdicts)
    {
        if (v == Verdict::FOLLOWS)
            followsCount++;
        else if (v == Verdict::ALMOST)
            almostCount++;
    }

    Verdict overall;
    if (followsCount >= 4)
        overall = Verdict::FOLLOWS; // Если 4 из 5 идеально
    else if (followsCount + almostCount >= 4)
        overall = Verdict::ALMOST; // Если 4 из 5 почти/идеально
    else
        overall = Verdict::NOT_FOLLOWS;

    std::cout << "ОБЩИЙ ВЕРДИКТ: " << getVerdictName(overall) << "\n";
    std::cout << "----------------------------------------\n";

    // 7. Подсчет предполагаемого изменения веса за период
    // Баланс = Съедено - (Базовый обмен + Сожжено на тренировках)
    double avgDailyBalance = avgCalories - (user.getBasalMetabolism() + avgActivity);
    double totalWeightChange = (avgDailyBalance * recordedDays) / 7700.0;

    std::cout << "АНАЛИЗ ПРОГРЕССА ЗА ПЕРИОД:\n";
    if (std::abs(totalWeightChange) < 0.1)
    {
        std::cout << "  За " << recordedDays << " дней ваш вес практически не изменился.\n";
    }
    else if (totalWeightChange < 0)
    {
        std::cout << "  За " << recordedDays << " дней вы предположительно СБРОСИЛИ "
                  << std::abs(totalWeightChange) << " кг.\n";
    }
    else
    {
        std::cout << "  За " << recordedDays << " дней вы предположительно НАБРАЛИ "
                  << totalWeightChange << " кг.\n";
    }

    // 8. Прогноз изменения веса на неделю (если продолжать в том же духе)
    double weeklyWeightChange = (avgDailyBalance * 7.0) / 7700.0;
    if (std::abs(weeklyWeightChange) > 0.05)
    {
        if (weeklyWeightChange < 0)
        {
            std::cout << "  Прогноз: при таком питании вы будете терять около "
                      << std::abs(weeklyWeightChange) << " кг в неделю.\n";
        }
        else
        {
            std::cout << "  Прогноз: при таком питании вы будете набирать около "
                      << weeklyWeightChange << " кг в неделю.\n";
        }
    }
    else
    {
        std::cout << "  Прогноз: ваш вес будет оставаться стабильным.\n";
    }
    std::cout << "----------------------------------------\n";

    // 9. Умные рекомендации
    std::cout << "РЕКОМЕНДАЦИИ:\n";
    bool hasRecommendation = false;

    if (vProt == Verdict::NOT_FOLLOWS && avgProteins < user.getDailyProteins())
    {
        std::cout << "  - Вам не хватает белка. Добавьте мясо, рыбу, яйца или творог.\n";
        hasRecommendation = true;
    }
    if (vCarb == Verdict::NOT_FOLLOWS && avgCarbs > user.getDailyCarbs())
    {
        std::cout << "  - У вас перебор углеводов. Замените сладкое на сложные углеводы (овсянка, гречка).\n";
        hasRecommendation = true;
    }
    if (vAct == Verdict::NOT_FOLLOWS && avgActivity < user.getOptimalActivityCal())
    {
        std::cout << "  - Вы слишком мало двигаетесь. Добавьте прогулки или тренировки.\n";
        hasRecommendation = true;
    }
    if (vCal == Verdict::NOT_FOLLOWS && avgCalories > user.getDailyCalories())
    {
        std::cout << "  - Вы переедаете. Постарайтесь урезать порции или убрать перекусы.\n";
        hasRecommendation = true;
    }

    if (!hasRecommendation || overall == Verdict::FOLLOWS)
    {
        std::cout << "  - Отличная работа! Вы придерживаетесь плана, продолжайте в том же духе!\n";
    }
    std::cout << "========================================\n";
}

// Вспомогательный метод: сравнивает фактическое значение с целевым
// и определяет вердикт на основе допустимого процента отклонения.
Report::Verdict Report::checkVerdict(double actual, double target, double tolerancePercent)
{
    if (target <= 0.0)
        return Verdict::NOT_FOLLOWS;

    double diff = std::abs(actual - target);
    double diffPercent = (diff / target) * 100.0;

    if (diffPercent <= tolerancePercent)
        return Verdict::FOLLOWS;
    else if (diffPercent <= tolerancePercent * 2.0)
        return Verdict::ALMOST;
    else
        return Verdict::NOT_FOLLOWS;
}

// Возвращает текстовое название вердикта для вывода в консоль
std::string Report::getVerdictName(Verdict v)
{
    switch (v)
    {
    case Verdict::FOLLOWS:
        return "Придерживается нормы";
    case Verdict::ALMOST:
        return "Почти придерживается";
    case Verdict::NOT_FOLLOWS:
        return "Не придерживается";
    }
    return "Неизвестно";
}