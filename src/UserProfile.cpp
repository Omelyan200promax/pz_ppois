#include "UserProfile.hpp"
#include <cstring>
#include <iomanip>
#include <limits>
#include <cmath>

// Возвращает никнейм пользователя
std::string UserProfile::getNickname() const { return std::string(nickname); }

// Возвращает возраст пользователя
int UserProfile::getAge() const { return age; }

// Возвращает текущий вес пользователя
double UserProfile::getWeight() const { return weight; }

// Возвращает рост пользователя
double UserProfile::getHeight() const { return height; }

// Возвращает процент жира пользователя
double UserProfile::getBodyFat() const { return bodyFat; }

// Возвращает целевой вес пользователя
double UserProfile::getTargetWeight() const { return targetWeight; }

// Возвращает выбранную цель (похудение, набор и т.д.)
Goal UserProfile::getGoal() const { return goal; }

// Возвращает пол пользователя
Gender UserProfile::getGender() const { return gender; }

// Возвращает рассчитанную дневную норму калорий
double UserProfile::getDailyCalories() const { return dailyCalories; }

// Возвращает рассчитанную дневную норму белков
double UserProfile::getDailyProteins() const { return dailyProteins; }

// Возвращает рассчитанную дневную норму жиров
double UserProfile::getDailyFats() const { return dailyFats; }

// Возвращает рассчитанную дневную норму углеводов
double UserProfile::getDailyCarbs() const { return dailyCarbs; }

// Возвращает рассчитанную дневную норму воды
double UserProfile::getDailyWater() const { return dailyWater; }

// Возвращает рассчитанную дневную норму клетчатки
double UserProfile::getDailyFiber() const { return dailyFiber; }

// Возвращает базовый обмен веществ (BMR)
double UserProfile::getBasalMetabolism() const { return basalMetabolism; }

// Возвращает рекомендуемую норму сна
double UserProfile::getSleepHours() const { return sleepHours; }

// Возвращает текущий статус пользователя
Status UserProfile::getStatus() const { return status; }

// Возвращает флаг наличия новой информации (для отображения звездочки)
bool UserProfile::getHasNewInfo() const { return hasNewInfo; }

// Возвращает средний расход калорий на физическую активность в день
double UserProfile::getAvgActivityCalories() const { return avgActivityCalories; }

// Устанавливает никнейм (с защитой от переполнения массива)
void UserProfile::setNickname(const std::string &nick)
{
    std::strncpy(nickname, nick.c_str(), 31);
    nickname[31] = '\0';
}

// Устанавливает средний расход на активности и сразу пересчитывает все нормы
void UserProfile::setAvgActivityCalories(double cal)
{
    if (cal >= 0)
    {
        avgActivityCalories = cal;
        // Если профиль уже инициализирован, сразу пересчитываем нормы
        if (weight > 0.0 && height > 0.0 && age > 0)
        {
            updateNorms();
        }
    }
}

// Запрашивает у пользователя личные данные при первом запуске
void UserProfile::inputInitialData()
{
    std::cout << "========================================\n";
    std::cout << "  ВНИМАНИЕ! БЕЗОПАСНОЕ ПОХУДЕНИЕ\n";
    std::cout << "========================================\n";
    std::cout << "- Безопасный темп похудения: 0.5-1% массы тела в неделю.\n";
    std::cout << "- Для снижения веса на 10 кг нужно минимум 2-3 месяца.\n";
    std::cout << "========================================\n\n";

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

    // Ручной ввод активности убран.
    // Это значение будет установлено из Menu после выбора активностей пользователем.
    avgActivityCalories = 0.0;

    updateNorms();
}

// Рассчитывает базовый обмен веществ (BMR) по формуле Миффлина-Сан Жеора
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

// Рассчитывает все дневные нормы (калории, БЖУ, вода, сон) на основе данных профиля
void UserProfile::calcDailyNorms()
{
    calcBasalMetabolism();

    double weightDelta = weight - targetWeight;

    // Проверка на безопасную скорость похудения
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

    // Корректировка калорий в зависимости от цели
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

    // Проценты БЖУ в зависимости от цели
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

    // Перевод процентов в граммы
    dailyProteins = (dailyCalories * pPerc) / 4.0;
    dailyFats = (dailyCalories * fPerc) / 9.0;
    dailyCarbs = (dailyCalories * cPerc) / 4.0;

    // Нормы воды и клетчатки
    dailyWater = weight * 35.0;
    dailyFiber = 25.0;

    // Расчет нормы сна в зависимости от возраста, пола и активности
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

// Публичный метод для принудительного пересчета норм
void UserProfile::updateNorms() { calcDailyNorms(); }

// Проверяет, является ли выбранный темп похудения безопасным
bool UserProfile::isWeightLossSafe() const
{
    if (weight > targetWeight && weeksToGoal > 0)
    {
        double weeklyLoss = (weight - targetWeight) / weeksToGoal;
        return weeklyLoss <= (weight * 0.01);
    }
    return true;
}

// Обновляет статус пользователя на основе количества дней подряд (стрика)
void UserProfile::updateStatus(int streakDays)
{
    Status newStatus = Status::BEGINNER;

    if (streakDays >= 10)
    {
        newStatus = Status::MACHINE;
    }
    else if (streakDays >= 7)
    {
        newStatus = Status::IRON_WILL;
    }
    else if (streakDays >= 3)
    {
        newStatus = Status::STARTED;
    }

    // Сравниваем статусы через приведение к int (так как enum class)
    if (static_cast<int>(newStatus) > static_cast<int>(status))
    {
        status = newStatus;
        hasNewInfo = true; // Включаем звездочку
    }
}

// Убирает флаг новой информации (звездочку) после просмотра
void UserProfile::markInfoAsRead() { hasNewInfo = false; }

// Выводит подробную информацию о профиле и рассчитанных нормах в консоль
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
    std::cout << "Средний расход на активности: " << avgActivityCalories << " ккал/день\n";
    std::cout << "Норма калорий: " << dailyCalories << " ккал\n";
    std::cout << "БЖУ: " << dailyProteins << " / " << dailyFats << " / " << dailyCarbs << " г\n";
    std::cout << "Вода: " << dailyWater << " мл, Клетчатка: " << dailyFiber << " г\n";
    std::cout << "Норма сна: " << sleepHours << " ч\n";
}

// Сбрасывает все данные профиля, кроме никнейма (пункт "Результат")
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

// Сохраняет профиль пользователя в бинарный файл
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

// Загружает профиль пользователя из бинарного файла
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

// Возвращает размер записи профиля в файле (в байтах)
std::size_t UserProfile::binarySize()
{
    return 32 + sizeof(int) + sizeof(double) * 9 + sizeof(Goal) + sizeof(Gender) + sizeof(int) + sizeof(Status) + sizeof(bool);
}