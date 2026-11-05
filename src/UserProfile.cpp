#include "UserProfile.hpp"
#include <cstring>
#include <iomanip>
#include <limits>
#include <cmath>

// Геттеры

std::string UserProfile::getNickname() const { return std::string(nickname); }
int UserProfile::getAge() const { return age; }
double UserProfile::getWeight() const { return weight; }
double UserProfile::getHeight() const { return height; }
double UserProfile::getBodyFat() const { return bodyFat; }
double UserProfile::getTargetWeight() const { return targetWeight; }
Goal UserProfile::getGoal() const { return goal; }
Gender UserProfile::getGender() const { return gender; }
double UserProfile::getDailyCalories() const { return dailyCalories; }
double UserProfile::getDailyProteins() const { return dailyProteins; }
double UserProfile::getDailyFats() const { return dailyFats; }
double UserProfile::getDailyCarbs() const { return dailyCarbs; }
double UserProfile::getDailyWater() const { return dailyWater; }
double UserProfile::getDailyFiber() const { return dailyFiber; }
double UserProfile::getBasalMetabolism() const { return basalMetabolism; }
double UserProfile::getSleepHours() const { return sleepHours; }
Status UserProfile::getStatus() const { return status; }
bool UserProfile::getHasNewInfo() const { return hasNewInfo; }
double UserProfile::getAvgActivityCalories() const { return avgActivityCalories; }

// Сеттеры

void UserProfile::setNickname(const std::string &nick)
{
    std::strncpy(nickname, nick.c_str(), 31);
    nickname[31] = '\0';
}

void UserProfile::setAvgActivityCalories(double cal)
{
    if (cal >= 0)
        avgActivityCalories = cal;
}

// Ввод данных при первом запуске

void UserProfile::inputInitialData()
{
    std::cout << "Введите ваш никнейм (ID): ";
    std::string tmpNick;
    std::getline(std::cin >> std::ws, tmpNick);
    setNickname(tmpNick);

    auto readDouble = [](const std::string &prompt) -> double
    {
        double v;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> v)
            {
                if (v <= 0.0)
                {
                    std::cout << "Ошибка: значение должно быть больше нуля.\n";
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return v;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите число.\n";
        }
    };

    std::cout << "Введите пол (1 - мужской, 2 - женский): ";
    int g;
    std::cin >> g;
    gender = (g == 2) ? Gender::FEMALE : Gender::MALE;

    age = (int)readDouble("Возраст (лет): ");
    weight = readDouble("Текущий вес (кг): ");
    height = readDouble("Рост (см): ");
    bodyFat = readDouble("Процент жира (%): ");
    targetWeight = readDouble("Целевой вес (кг): ");
    weeksToGoal = (int)readDouble("Желаемое время достижения цели (недель): ");

    std::cout << "Выберите цель:\n";
    std::cout << "1. Похудение\n2. Сушка\n3. Набор массы\n4. Поддержание веса\n>";
    int goalChoice;
    std::cin >> goalChoice;
    switch (goalChoice)
    {
    case 1:
        goal = Goal::LOSE_WEIGHT;
        break;
    case 2:
        goal = Goal::CUTTING;
        break;
    case 3:
        goal = Goal::GAIN_MASS;
        break;
    default:
        goal = Goal::MAINTAIN;
        break;
    }

    avgActivityCalories = readDouble("Средний расход на активности в день (ккал): ");

    updateNorms();
}

// Вспомогательные методы

void UserProfile::calcBasalMetabolism()
{
    if (gender == Gender::MALE)
    {
        basalMetabolism = 10 * weight + 6.25 * height - 5 * age + 5;
    }
    else
    {
        basalMetabolism = 10 * weight + 6.25 * height - 5 * age - 161;
    }
}

void UserProfile::calcDailyNorms()
{
    calcBasalMetabolism();

    double weightDelta = weight - targetWeight;

    if (weightDelta > 0 && weeksToGoal > 0)
    {
        double weeklyLoss = weightDelta / weeksToGoal;
        double maxSafeLoss = weight * 0.01;
        if (weeklyLoss > maxSafeLoss)
        {
            std::cout << "ВНИМАНИЕ: Вы хотите худеть слишком быстро! Это опасно для здоровья.\n";
        }
    }

    double maintenanceCalories = basalMetabolism + avgActivityCalories;

    if (goal == Goal::LOSE_WEIGHT)
    {
        dailyCalories = maintenanceCalories - 500;
    }
    else if (goal == Goal::CUTTING)
    {
        dailyCalories = maintenanceCalories - 300;
    }
    else if (goal == Goal::GAIN_MASS)
    {
        dailyCalories = maintenanceCalories + 300;
    }
    else
    {
        dailyCalories = maintenanceCalories;
    }

    double pPerc = 0.30, fPerc = 0.30, cPerc = 0.40;
    if (goal == Goal::CUTTING)
    {
        pPerc = 0.40;
        fPerc = 0.25;
        cPerc = 0.35;
    }
    else if (goal == Goal::GAIN_MASS)
    {
        pPerc = 0.25;
        fPerc = 0.25;
        cPerc = 0.50;
    }

    dailyProteins = (dailyCalories * pPerc) / 4.0;
    dailyFats = (dailyCalories * fPerc) / 9.0;
    dailyCarbs = (dailyCalories * cPerc) / 4.0;

    dailyWater = weight * 35.0;
    dailyFiber = 25.0;

    if (age < 18)
        sleepHours = 9.0;
    else if (age < 65)
        sleepHours = 8.0;
    else
        sleepHours = 7.5;

    if (gender == Gender::FEMALE)
        sleepHours += 0.5;
    if (avgActivityCalories > 300.0)
        sleepHours += 0.5;
    else if (avgActivityCalories < 100.0)
        sleepHours -= 0.5;
}

void UserProfile::updateNorms() { calcDailyNorms(); }

bool UserProfile::isWeightLossSafe() const
{
    if (weight > targetWeight && weeksToGoal > 0)
    {
        double weeklyLoss = (weight - targetWeight) / weeksToGoal;
        return weeklyLoss <= (weight * 0.01);
    }
    return true;
}

void UserProfile::updateStatus()
{
    if (status == Status::BEGINNER)
    {
        status = Status::STARTED;
        hasNewInfo = true;
    }
}

void UserProfile::markInfoAsRead() { hasNewInfo = false; }

// Вывод

void UserProfile::print() const
{
    std::cout << "--- Личные данные ---\n";
    std::cout << "Никнейм: " << nickname << "\n";
    std::cout << "Вес: " << weight << " кг, Рост: " << height << " см\n";

    std::cout << "Цель: ";
    switch (goal)
    {
    case Goal::LOSE_WEIGHT:
        std::cout << "Похудение\n";
        break;
    case Goal::CUTTING:
        std::cout << "Сушка\n";
        break;
    case Goal::GAIN_MASS:
        std::cout << "Набор массы\n";
        break;
    default:
        std::cout << "Поддержание\n";
        break;
    }

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "BMR (базовый обмен): " << basalMetabolism << " ккал\n";
    std::cout << "Норма калорий: " << dailyCalories << " ккал\n";
    std::cout << "БЖУ: " << dailyProteins << " / " << dailyFats << " / " << dailyCarbs << " г\n";
    std::cout << "Вода: " << dailyWater << " мл, Клетчатка: " << dailyFiber << " г\n";
    std::cout << "Норма сна: " << sleepHours << " ч\n";
}

// Сброс (кроме никнейма)

void UserProfile::reset()
{
    age = 0;
    weight = 0.0;
    height = 0.0;
    bodyFat = 0.0;
    targetWeight = 0.0;
    goal = Goal::MAINTAIN;
    weeksToGoal = 0;
    gender = Gender::MALE;
    avgActivityCalories = 0.0;
    dailyCalories = 0.0;
    dailyProteins = 0.0;
    dailyFats = 0.0;
    dailyCarbs = 0.0;
    dailyWater = 0.0;
    dailyFiber = 0.0;
    basalMetabolism = 0.0;
    sleepHours = 0.0;
    status = Status::BEGINNER;
    hasNewInfo = false;
}

// Бинарный файл

void UserProfile::save(std::ofstream &out) const
{
    out.write(nickname, 32);
    out.write(reinterpret_cast<const char *>(&age), sizeof(int));
    out.write(reinterpret_cast<const char *>(&weight), sizeof(double));
    out.write(reinterpret_cast<const char *>(&height), sizeof(double));
    out.write(reinterpret_cast<const char *>(&bodyFat), sizeof(double));
    out.write(reinterpret_cast<const char *>(&targetWeight), sizeof(double));
    out.write(reinterpret_cast<const char *>(&goal), sizeof(Goal));
    out.write(reinterpret_cast<const char *>(&gender), sizeof(Gender));
    out.write(reinterpret_cast<const char *>(&weeksToGoal), sizeof(int));
    out.write(reinterpret_cast<const char *>(&avgActivityCalories), sizeof(double));

    out.write(reinterpret_cast<const char *>(&dailyCalories), sizeof(double));
    out.write(reinterpret_cast<const char *>(&dailyProteins), sizeof(double));
    out.write(reinterpret_cast<const char *>(&dailyFats), sizeof(double));
    out.write(reinterpret_cast<const char *>(&dailyCarbs), sizeof(double));
    out.write(reinterpret_cast<const char *>(&dailyWater), sizeof(double));
    out.write(reinterpret_cast<const char *>(&dailyFiber), sizeof(double));
    out.write(reinterpret_cast<const char *>(&basalMetabolism), sizeof(double));
    out.write(reinterpret_cast<const char *>(&sleepHours), sizeof(double));

    out.write(reinterpret_cast<const char *>(&status), sizeof(Status));
    out.write(reinterpret_cast<const char *>(&hasNewInfo), sizeof(bool));
}

void UserProfile::load(std::ifstream &in)
{
    in.read(nickname, 32);
    nickname[31] = '\0';

    in.read(reinterpret_cast<char *>(&age), sizeof(int));
    in.read(reinterpret_cast<char *>(&weight), sizeof(double));
    in.read(reinterpret_cast<char *>(&height), sizeof(double));
    in.read(reinterpret_cast<char *>(&bodyFat), sizeof(double));
    in.read(reinterpret_cast<char *>(&targetWeight), sizeof(double));
    in.read(reinterpret_cast<char *>(&goal), sizeof(Goal));
    in.read(reinterpret_cast<char *>(&gender), sizeof(Gender));
    in.read(reinterpret_cast<char *>(&weeksToGoal), sizeof(int));
    in.read(reinterpret_cast<char *>(&avgActivityCalories), sizeof(double));

    in.read(reinterpret_cast<char *>(&dailyCalories), sizeof(double));
    in.read(reinterpret_cast<char *>(&dailyProteins), sizeof(double));
    in.read(reinterpret_cast<char *>(&dailyFats), sizeof(double));
    in.read(reinterpret_cast<char *>(&dailyCarbs), sizeof(double));
    in.read(reinterpret_cast<char *>(&dailyWater), sizeof(double));
    in.read(reinterpret_cast<char *>(&dailyFiber), sizeof(double));
    in.read(reinterpret_cast<char *>(&basalMetabolism), sizeof(double));
    in.read(reinterpret_cast<char *>(&sleepHours), sizeof(double));

    in.read(reinterpret_cast<char *>(&status), sizeof(Status));
    in.read(reinterpret_cast<char *>(&hasNewInfo), sizeof(bool));
}

std::size_t UserProfile::binarySize()
{
    return 32 + sizeof(int) + sizeof(double) * 9 + sizeof(Goal) + sizeof(Gender) + sizeof(int) + sizeof(Status) + sizeof(bool);
}