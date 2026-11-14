#include "Report.hpp"
#include <iomanip>

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

        // Расход калорий на активность берем из веса пользователя
        totalActivity += entry.getBurnedKcal(user.getWeight());

        recordedDays++;
    }

    // Если вдруг все записи пустые
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

    // 3. Вывод отчета
    std::cout << "========================================\n";
    std::cout << "             ВАШ ОТЧЁТ                  \n";
    std::cout << "========================================\n";
    std::cout << "Проанализировано дней: " << recordedDays << "\n";
    if (emptyDays > 0)
    {
        std::cout << "Пропущено (пустых) дней: " << emptyDays << "\n";
        std::cout << "(!) Внимание: данные могут быть неточными из-за пропусков.\n";
    }

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "----------------------------------------\n";
    std::cout << "Средний калораж:   " << avgCalories << " ккал (норма: " << user.getDailyCalories() << ")\n";
    std::cout << "Средние белки:     " << avgProteins << " г (норма: " << user.getDailyProteins() << ")\n";
    std::cout << "Средние жиры:      " << avgFats << " г (норма: " << user.getDailyFats() << ")\n";
    std::cout << "Средние углеводы:  " << avgCarbs << " г (норма: " << user.getDailyCarbs() << ")\n";
    std::cout << "Средняя активность:" << avgActivity << " ккал\n";
    std::cout << "----------------------------------------\n";

    // 4. Анализ соответствия
    // Сравниваем средний калораж с нормой (допуск 10% - "придерживается", 20% - "почти")
    Verdict verdict = checkVerdict(avgCalories, user.getDailyCalories(), 10.0);

    std::cout << "Вердикт по калориям: " << getVerdictName(verdict) << "\n";
    std::cout << "========================================\n";
}

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