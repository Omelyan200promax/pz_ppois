#include "TestDevMenu.hpp"
#include <iomanip>

// Безопасный ввод целого числа от min до max
int TestDevMenu::readInt(const std::string &prompt, int min, int max) const
{
    while (true)
    {
        std::cout << prompt;
        int v = 0;
        if (std::cin >> v)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
            if (v >= min && v <= max)
                return v;
            std::cout << "Ошибка: введите число от " << min << " до " << max << ".\n";
        }
        else
        {
            if (std::cin.eof())
                return min;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    }
}

// Главный цикл
void TestDevMenu::run()
{
    while (true)
    {
        std::cout << "\n====== МЕНЮ РАЗРАБОТЧИКА 1 (ТЕСТЫ) ======\n"
                  << "1. Тест UserProfile (ввод и расчет норм)\n"
                  << "2. Тест ProductCatalog (добавление, поиск, фильтры)\n"
                  << "3. Тест Diary + Calendar (создание записи, связка с профилем)\n"
                  << "4. Тест Report (генерация отчета за 10 дней)\n"
                  << "0. Выход\n";

        int c = readInt("Выбор: ", 0, 4);
        if (c == 0)
            return;

        switch (c)
        {
        case 1:
            testUserProfile();
            break;
        case 2:
            testProductCatalog();
            break;
        case 3:
            testDiaryAndCalendar();
            break;
        case 4:
            testReport();
            break;
        }
    }
}

// Тест 1: Профиль
void TestDevMenu::testUserProfile()
{
    std::cout << "\n--- Тест UserProfile ---\n";
    user.reset(); // Сбрасываем профиль перед новым тестом

    std::cout << "Введи данные для профиля (или используй тестовые, если уже вводил)\n";
    user.inputInitialData();
    std::cout << "\nРезультат расчета:\n";
    user.print();

    // Тест статусов
    std::cout << "\nТекущий статус: " << static_cast<int>(user.getStatus()) << "\n";
    std::cout << "Симулируем 7 дней подряд...\n";
    user.updateStatus(7);
    std::cout << "Новый статус: " << static_cast<int>(user.getStatus()) << "\n";
    std::cout << "Флаг новой инфы (звездочка): " << (user.getHasNewInfo() ? "Да" : "Нет") << "\n";
}

// Тест 2: Каталог продуктов
void TestDevMenu::testProductCatalog()
{
    std::cout << "\n--- Тест ProductCatalog ---\n";

    // Создаем тестовые продукты
    Product p1("Яблоко", 52, 0.3, 0.2, 14, 2.4);
    Product p2("Куриная грудка", 165, 31, 3.6, 0, 0);
    Product p3("Гречка", 343, 12.6, 3.3, 62, 10);
    Product p4("Йогурт натуральный", 60, 5, 3, 4, 0);

    productCatalog.addProduct(p1);
    productCatalog.addProduct(p2);
    productCatalog.addProduct(p3);
    productCatalog.addProduct(p4);

    std::cout << "Добавлено 4 продукта.\n";
    productCatalog.printAll();

    std::cout << "\nТест поиска по подстроке 'Кури':\n";
    auto res = productCatalog.searchByName("Кури");
    for (const auto &p : res)
        p.printShort();

    std::cout << "\nТест фильтра по калориям (50 - 200):\n";
    auto resCal = productCatalog.filterByCalories(50, 200);
    for (const auto &p : resCal)
        p.printShort();

    std::cout << "\nТест фильтра по белкам (30 г +- 5):\n";
    auto resProt = productCatalog.filterByProteins(30.0);
    for (const auto &p : resProt)
        p.printShort();

    std::cout << "\nТест фильтра по жирам (3.5 г +- 3):\n";
    auto resFats = productCatalog.filterByFats(3.5);
    for (const auto &p : resFats)
        p.printShort();

    std::cout << "\nТест фильтра по углеводам (10 г +- 7):\n";
    auto resCarbs = productCatalog.filterByCarbs(10.0);
    for (const auto &p : resCarbs)
        p.printShort();

    productCatalog.saveToFile();
    std::cout << "Каталог сохранен в файл.\n";
}

// Тест 3: Дневник и Календарь
void TestDevMenu::testDiaryAndCalendar()
{
    std::cout << "\n--- Тест Diary + Calendar ---\n";

    // 1. Спрашиваем дату
    Date d;
    d.inputFromConsole();

    // 2. Берем запись (Calendar сам вызовет Diary::getOrCreateEntry)
    DiaryEntry *entry = calendar.getEntryForEditing(d);
    std::cout << "Запись на " << d.toString() << " готова к редактированию.\n";

    // Создаем продукты
    Product apple("Яблоко", 52, 0.3, 0.2, 14, 2.4);
    Product chicken("Куриная грудка", 165, 31, 3.6, 0, 0);
    Product buckwheat("Гречка", 343, 12.6, 3.3, 62, 10);
    Product yogurt("Йогурт натуральный", 60, 5, 3, 4, 0);

    // 3. Добавляем еду на весь день
    std::cout << "Добавляем продукты в разные приемы пищи...\n";
    // Завтрак: яблоко и йогурт
    entry->getMeal(MealType::Breakfast).addProduct(apple, 150);
    entry->getMeal(MealType::Breakfast).addProduct(yogurt, 200);

    // Обед: курица и гречка
    entry->getMeal(MealType::Lunch).addProduct(chicken, 200);
    entry->getMeal(MealType::Lunch).addProduct(buckwheat, 150);

    // Ужин: курица
    entry->getMeal(MealType::Dinner).addProduct(chicken, 150);

    // Перекус: яблоко
    entry->getMeal(MealType::Snack).addProduct(apple, 100);

    // 4. Добавляем активность
    std::cout << "Добавим активность 'Бег' (30 мин, средняя).\n";
    PhysicalActivity run("Бег", 9.8, 30, Intensity::Medium);
    entry->addActivity(run);

    // 5. Вводим воду и сон
    std::cout << "Введем воду и сон (тестовые значения: 2000 мл, 8 ч).\n";
    entry->getWater().setAmount(2000);
    entry->getSleep().setHours(8);

    // 6. Сохраняем дневник
    diary.saveToFile();
    std::cout << "Дневник сохранен в файл.\n";

    // 7. Выводим запись. Передаем нормы и BMR из профиля!
    std::cout << "\nВЫВОД ЗАПИСИ (с учетом норм из профиля):\n";
    entry->print(user.getDailyWater(), user.getSleepHours(), user.getWeight(), user.getBasalMetabolism());

    // 8. Проверяем Календарь
    std::cout << "\nВывод всех записей через Calendar:\n";
    calendar.printAll();
}

// Тест 4: Отчет
void TestDevMenu::testReport()
{
    std::cout << "\n--- Тест Report ---\n";
    std::cout << "Генерируем 10 дней с разной едой и активностью...\n";

    diary.clear();                  // <--- ДОБАВИТЬ ЭТО
    user.setAvgActivityCalories(0); // <--- И ЭТО (сбрасываем старые тренировки)

    // Продукты для теста
    Product apple("Яблоко", 52, 0.3, 0.2, 14, 2.4);
    Product chicken("Куриная грудка", 165, 31, 3.6, 0, 0);
    Product buckwheat("Гречка", 343, 12.6, 3.3, 62, 10);
    Product yogurt("Йогурт натуральный", 60, 5, 3, 4, 0);

    // Создаем 10 записей с разными датами
    for (int i = 1; i <= 10; ++i)
    {
        Date d(i, 11, 2026); // 01.11 - 10.11
        DiaryEntry *entry = calendar.getEntryForEditing(d);

        if (i % 2 == 1)
        {
            // Нечетные дни: нормальное питание (ближе к норме)
            entry->getMeal(MealType::Breakfast).addProduct(yogurt, 200);
            entry->getMeal(MealType::Breakfast).addProduct(apple, 100);
            entry->getMeal(MealType::Lunch).addProduct(chicken, 200);
            entry->getMeal(MealType::Lunch).addProduct(buckwheat, 150);
            entry->getMeal(MealType::Dinner).addProduct(chicken, 150);
        }
        else
        {
            // Четные дни: перебор по калориям (много еды)
            entry->getMeal(MealType::Breakfast).addProduct(buckwheat, 200);
            entry->getMeal(MealType::Lunch).addProduct(chicken, 250);
            entry->getMeal(MealType::Lunch).addProduct(buckwheat, 250);
            entry->getMeal(MealType::Dinner).addProduct(chicken, 200);
            entry->getMeal(MealType::Snack).addProduct(apple, 150);
        }

        // Активность: в четные дни бегаем, в нечетные - ходим
        if (i % 2 == 0)
        {
            PhysicalActivity run("Бег", 9.8, 40, Intensity::High);
            entry->addActivity(run);
        }
        else
        {
            PhysicalActivity walk("Ходьба", 3.5, 60, Intensity::Low);
            entry->addActivity(walk);
        }
    }
    diary.saveToFile();

    // Вызываем отчет
    std::cout << "Генерация отчета:\n";
    Report::generate(diary, user);
}