#include "menu.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <fstream>

// Безопасный ввод целых чисел
int Menu::readInt(const std::string &prompt, int min, int max) const
{
    while (true)
    {
        std::cout << prompt;
        int v = 0;
        if (std::cin >> v)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (v >= min && v <= max) return v;
            std::cout << "Ошибка: введите число от " << min << " до " << max << ".\n";
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    }
}

// Безопасный ввод дробных чисел
double Menu::readDouble(const std::string &prompt, double min, double max) const
{
    while (true)
    {
        std::cout << prompt;
        double v = 0.0;
        if (std::cin >> v)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (v >= min && v <= max) return v;
            std::cout << "Ошибка: введите число от " << min << " до " << max << ".\n";
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    }
}

// Первоначальная настройка при запуске
void Menu::showInitialSetup()
{
    std::cout << "Добро пожаловать в трекер питания и активности!\n";
    // Ввод профиля (там уже есть предупреждение о безопасном похудении)
    user.inputInitialData();

    // Выбор активностей из каталога
    std::cout << "\nВыберите ваши активности. (Введите 1, чтобы выбрать, 0 - пропустить)\n";
    double totalWeeklyKcal = 0.0;
    
    for (const auto &act : activityCatalog.getAll())
    {
        int choice = readInt("Занимаетесь ли вы \"" + act.getName() + "\"? (1 - да, 0 - нет): ", 0, 1);
        if (choice == 1)
        {
            int days = readInt("  Сколько раз в неделю? (1-7): ", 1, 7);
            double mins = readDouble("  Сколько минут за одну тренировку? ", 1, 300);
            
            PhysicalActivity tempAct = act;
            tempAct.setMinutes(mins);
            totalWeeklyKcal += tempAct.calcKcal(user.getWeight()) * days;
        }
    }

    // Средний расход в день
    user.setAvgActivityCalories(totalWeeklyKcal / 7.0);

    // Запрос текущей даты
    std::cout << "\nСоздадим первую запись в дневнике.\n";
    Date currentDate;
    currentDate.inputFromConsole();
    diary.getOrCreateEntry(currentDate);
    
    std::cout << "Настройка завершена!\n";
}

// Главное меню
void Menu::run()
{
    showInitialSetup();
    
    while (running)
    {
        showMainMenu();
    }
}

void Menu::showMainMenu()
{
    std::cout << "\n====== ГЛАВНОЕ МЕНЮ ======\n";
    std::cout << "1. Личные данные";
    if (user.getHasNewInfo()) std::cout << " [*]"; // Звездочка статуса
    std::cout << "\n2. Дневник (питание и активность)\n";
    std::cout << "3. Каталог продуктов\n";
    std::cout << "4. Календарь\n";
    std::cout << "5. Новая запись\n";
    std::cout << "6. Отчет\n";
    std::cout << "7. Результат (достижение цели)\n";
    std::cout << "0. Выход\n";

    int choice = readInt("Выбор: ", 0, 7);
    switch (choice)
    {
        case 1: showPersonalData(); break;
        case 2: showDiary(); break;
        case 3: showProductCatalog(); break;
        case 4: showCalendar(); break;
        case 5: showNewEntry(); break;
        case 6: showReport(); break;
        case 7: showResult(); break;
        case 0: 
        running = false;
        std::ofstream uf("user.bin", std::ios::binary);
        user.save(uf);
        productCatalog.saveToFile();
        activityCatalog.saveToFile();
        diary.saveToFile();
        std::cout << "Данные сохранены. До свидания!\n";
        break;
    }
}

// 1. Личные данные
void Menu::showPersonalData()
{
    user.print();
    user.markInfoAsRead(); // Убираем звездочку
    
    // Обновляем статус на основе стриков
    int streak = calculateStreak();
    user.updateStatus(streak);
    std::cout << "Текущий стрик (дней подряд): " << streak << "\n";
}

// 2. Дневник
void Menu::showDiary()
{
    Date d;
    d.inputFromConsole();
    DiaryEntry &entry = diary.getOrCreateEntry(d);
    
    showDiaryEntryMenu(entry);
    
    diary.saveToFile();
    recalculateActivityAverage(); // Пересчитываем норму калорий после тренировок!
}

void Menu::showDiaryEntryMenu(DiaryEntry &entry)
{
    while (true)
    {
        std::cout << "\n--- Запись за " << entry.getDate().toString() << " ---\n";
        std::cout << "1. Добавить продукт в прием пищи\n";
        std::cout << "2. Ввести воду\n";
        std::cout << "3. Ввести сон\n";
        std::cout << "4. Добавить физическую активность\n";
        std::cout << "5. Показать запись\n";
        std::cout << "0. Назад\n";

        int choice = readInt("Выбор: ", 0, 5);
        switch (choice)
        {
        case 0: return;
        case 1:
            addProductToMeal(entry);
        break;
        case 2:
            entry.getWater().inputFromConsole();
        break;
        case 3:
            entry.getSleep().inputFromConsole();
        break;
        case 4:
        {
            activityCatalog.printAll();
            std::string actName;
            std::cout << "Введите название активности из списка: ";
            std::getline(std::cin >> std::ws, actName);
            PhysicalActivity* act = activityCatalog.findByName(actName);
            if (act) {
                act->inputWorkoutFromConsole();
                entry.addActivity(*act);
                std::cout << "Активность добавлена!\n";
            } else {
                std::cout << "Активность не найдена.\n";
            }
        }
        break;
         case 5:
            entry.print(user.getDailyWater(), user.getSleepHours(), user.getWeight(), user.getBasalMetabolism());
         break;
        default:
        std::cout <<"Введите корректное значение!\n";
            break;
        }
    }
}

// Добавление продукта
void Menu::addProductToMeal(DiaryEntry &entry)
{
    int meal = readInt("Прием пищи (1-Завтрак, 2-Обед, 3-Ужин, 4-Перекус): ", 1, 4);
    MealType type = static_cast<MealType>(meal - 1);

    std::string query;
    std::cout << "Поиск продукта (введите часть названия): ";
    std::getline(std::cin >> std::ws, query);

    auto results = productCatalog.searchByName(query);
    if (results.empty()) {
        std::cout << "Ничего не найдено. Хотите создать новый продукт? (1 - да, 0 - нет): ";
        if (readInt("", 0, 1) == 1) {
            Product newP;
            newP.inputFromConsole();
            productCatalog.addProduct(newP);
            productCatalog.saveToFile();
            entry.getMeal(type).addProductFromConsole(newP);
        }
    } else {
        std::cout << "Найдено:\n";
        for (size_t i = 0; i < results.size(); ++i) {
            std::cout << i + 1 << ") ";
            results[i].printShort();
        }
        int choice = readInt("Выберите номер продукта (0 - отмена): ", 0, results.size());
        if (choice > 0) {
            entry.getMeal(type).addProductFromConsole(results[choice - 1]);
        }
    }
}

// 3. Каталог продуктов
void Menu::showProductCatalog()
{
    std::cout << "\n--- Каталог продуктов ---\n";
    productCatalog.printAll();
    
    // Упрощенный вариант: добавление нового продукта
    int choice = readInt("Добавить новый продукт? (1 - да, 0 - назад): ", 0, 1);
    if (choice == 1) {
        Product p;
        p.inputFromConsole();
        if (productCatalog.addProduct(p)) {
            std::cout << "Продукт добавлен!\n";
            productCatalog.saveToFile();
        } else {
            std::cout << "Такой продукт уже есть.\n";
        }
    }
}

// 4. Календарь
void Menu::showCalendar()
{
    std::cout << "\n--- Календарь ---\n";
    calendar.printAll();
    // Можно добавить поиск и удаление по дате
    int choice = readInt("Удалить запись по дате? (1 - да, 0 - назад): ", 0, 1);
    if (choice == 1) {
        Date d; d.inputFromConsole();
        if (calendar.deleteByDate(d)) diary.saveToFile();
    }
}

// 5. Новая запись
void Menu::showNewEntry()
{
    Date d;
    d.inputFromConsole();
    diary.getOrCreateEntry(d);
    diary.saveToFile();
    std::cout << "Запись на " << d.toString() << " создана!\n";
}

// 6. Отчет
void Menu::showReport()
{
    Report::generate(diary, user);
}

// 7. Результат
void Menu::showResult()
{
    std::cout << "Вы достигли своей цели? (1 - Да, 0 - Нет): ";
    if (readInt("", 0, 1) == 1) {
        std::cout << "Поздравляем! Вы огромный молодец!\n";
        std::cout << "Данные сбрасываются для новой цели (никнейм сохранен).\n";
        user.reset();
        diary.clear();
        
        // Очищаем файлы
        diary.saveToFile();
        // (user.save нужно делать, если есть отдельный файл)
        running = false; // Выход или перезапуск
    }
}

// Пересчет среднего расхода на тренировках (берем последние 7 дней)
void Menu::recalculateActivityAverage()
{
    const auto &entries = diary.getAll();
    if (entries.empty()) return;

    double totalBurned = 0.0;
    int daysCount = 0;
    
    // Берем последние до 7 записей
    int startIdx = std::max(0, (int)entries.size() - 7);
    for (int i = startIdx; i < (int)entries.size(); ++i) {
        totalBurned += entries[i].getBurnedKcal(user.getWeight());
        daysCount++;
    }

    if (daysCount > 0) {
        user.setAvgActivityCalories(totalBurned / daysCount);
    }
}

// Подсчет дней подряд (стрик)
int Menu::calculateStreak()
{
    const auto &entries = diary.getAll();
    if (entries.empty()) return 0;

    // Сортируем копию по дате (от новых к старым)
    std::vector<DiaryEntry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const DiaryEntry& a, const DiaryEntry& b) {
        return a.getDate() > b.getDate();
    });

    int streak = 1;
    for (size_t i = 0; i < sorted.size() - 1; ++i) {
        const Date& d1 = sorted[i].getDate();
        const Date& d2 = sorted[i+1].getDate();
        
        // Упрощенная проверка на соседние дни (без учета переходов через месяц)
        // Для идеального подсчета нужен метод разницы в днях, но пока так:
        if (d1.getYear() == d2.getYear() && d1.getMonth() == d2.getMonth() && d1.getDay() - d2.getDay() == 1) {
            streak++;
        } else {
            break; // Стрик прервался
        }
    }
    return streak;
}