#include "DiaryEntry.hpp"
#include "PhysicalActivity.hpp"
#include <iomanip> // std::fixed, std::setprecision

// Конструктор: создает запись дневника на указанную дату
DiaryEntry::DiaryEntry(const Date &date_) : date(date_)
{
}

// Возвращает дату этой записи
const Date &DiaryEntry::getDate() const { return date; }

// Устанавливает новую дату для записи
void DiaryEntry::setDate(const Date &d) { date = d; }

// Возвращает ссылку на прием пищи (завтрак/обед/ужин/перекус) для редактирования
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

// Возвращает константную ссылку на прием пищи (только для чтения)
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

// Возвращает ссылку на запись о воде для редактирования
WaterRecord &DiaryEntry::getWater() { return water; }

// Возвращает константную ссылку на запись о воде (только для чтения)
const WaterRecord &DiaryEntry::getWater() const { return water; }

// Возвращает ссылку на запись о сне для редактирования
SleepRecord &DiaryEntry::getSleep() { return sleep; }

// Возвращает константную ссылку на запись о сне (только для чтения)
const SleepRecord &DiaryEntry::getSleep() const { return sleep; }

// Добавляет физическую активность в дневник за этот день
void DiaryEntry::addActivity(const PhysicalActivity &a)
{
    activities.push_back(a);
}

// Удаляет физическую активность по её названию
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

// Возвращает константную ссылку на список всех активностей за день
const std::vector<PhysicalActivity> &DiaryEntry::getActivities() const
{
    return activities;
}

// Считает и возвращает сумму калорий за все приемы пищи
double DiaryEntry::getTotalCalories() const
{
    return breakfast.getCalories() + lunch.getCalories() +
           dinner.getCalories() + snack.getCalories();
}

// Считает и возвращает сумму белков за все приемы пищи
double DiaryEntry::getTotalProteins() const
{
    return breakfast.getProteins() + lunch.getProteins() +
           dinner.getProteins() + snack.getProteins();
}

// Считает и возвращает сумму жиров за все приемы пищи
double DiaryEntry::getTotalFats() const
{
    return breakfast.getFats() + lunch.getFats() +
           dinner.getFats() + snack.getFats();
}

// Считает и возвращает сумму углеводов за все приемы пищи
double DiaryEntry::getTotalCarbs() const
{
    return breakfast.getCarbs() + lunch.getCarbs() +
           dinner.getCarbs() + snack.getCarbs();
}

// Считает и возвращает сумму клетчатки за все приемы пищи
double DiaryEntry::getTotalFiber() const
{
    return breakfast.getFiber() + lunch.getFiber() +
           dinner.getFiber() + snack.getFiber();
}

// Считает общее количество сожженных калорий за все активности (нужен вес пользователя)
double DiaryEntry::getBurnedKcal(double weightKg) const
{
    double sum = 0.0;
    for (const auto &a : activities)
    {
        sum += a.calcKcal(weightKg);
    }
    return sum;
}

// Проверяет, пуста ли запись (ничего не введено ни в один раздел)
bool DiaryEntry::isEmpty() const
{
    return breakfast.isEmpty() && lunch.isEmpty() &&
           dinner.isEmpty() && snack.isEmpty() &&
           water.getAmount() == 0.0 && sleep.getHours() == 0.0 &&
           activities.empty();
}

// Оператор равенства: сравнивает записи по дате
bool DiaryEntry::operator==(const DiaryEntry &other) const
{
    return date == other.date;
}

// Оператор "меньше": сравнивает записи по дате (используется для сортировки)
bool DiaryEntry::operator<(const DiaryEntry &other) const
{
    return date < other.date;
}

// Выводит полную информацию за день (еду, воду, сон, активности)
void DiaryEntry::print(double waterNorm, double sleepNorm, double weightKg, double bmr) const
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
    sleep.print(sleepNorm, bmr);

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

// Сохраняет запись (дату, еду, воду, сон, активности) в бинарный файл
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

// Загружает запись из бинарного файла
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

    // Защита от поврежденных данных
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