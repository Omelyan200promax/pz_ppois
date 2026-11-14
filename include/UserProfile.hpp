#ifndef USERPROFILE_HPP
#define USERPROFILE_HPP

#include "Date.hpp"
#include <string>
#include <fstream>
#include <iostream>

// Цель пользователя (определяет дефицит/профицит калорий и БЖУ)
enum class Goal
{
    LOSE_WEIGHT, // Похудение
    CUTTING,     // Сушка
    GAIN_MASS,   // Набор массы
    MAINTAIN     // Поддержание веса
};

// Статус пользователя (зависит от количества дней подряд в дневнике)
enum class Status
{
    BEGINNER,  // Новичок
    STARTED,   // Начало положено
    IRON_WILL, // Стальная сила воли
    MACHINE    // Машина
};

// Пол (необходим для точного расчета базового метаболизма)
enum class Gender
{
    MALE,  // Мужской
    FEMALE // Женский
};

class UserProfile
{
private:
    // --- Личные данные пользователя ---
    char nickname[32] = {0}; // Никнейм (используется как уникальный ID)
    int age = 0;
    double weight = 0.0;
    double height = 0.0;
    double bodyFat = 0.0;
    double targetWeight = 0.0;
    int weeksToGoal = 0; // Желаемый срок достижения цели (в неделях)
    Goal goal = Goal::MAINTAIN;
    Gender gender = Gender::MALE;

    // --- Данные из дневника (обновляются извне) ---
    double avgActivityCalories = 0.0; // Средний расход на активности в день

    // --- Рассчитанные нормы (заполняются автоматически) ---
    double dailyCalories = 0.0;
    double dailyProteins = 0.0;
    double dailyFats = 0.0;
    double dailyCarbs = 0.0;
    double dailyWater = 0.0;
    double dailyFiber = 0.0;
    double basalMetabolism = 0.0; // BMR (базовый обмен веществ)
    double sleepHours = 0.0;

    // --- Статус и уведомления ---
    Status status = Status::BEGINNER;
    bool hasNewInfo = false; // Флаг для отображения звездочки (есть новый статус)

    // --- Вспомогательные методы расчета ---
    void calcBasalMetabolism(); // Расчет BMR по формуле Миффлина-Сан Жеора
    void calcDailyNorms();      // Пересчет всех норм (калории, БЖУ, вода, сон)

public:
    UserProfile() = default;

    // --- Геттеры (нужны для Diary, Report и Menu) ---
    std::string getNickname() const;
    int getAge() const;
    double getWeight() const;
    double getHeight() const;
    double getBodyFat() const;
    double getTargetWeight() const;
    Goal getGoal() const;
    Gender getGender() const;
    double getDailyCalories() const;
    double getDailyProteins() const;
    double getDailyFats() const;
    double getDailyCarbs() const;
    double getDailyWater() const;
    double getDailyFiber() const;
    double getBasalMetabolism() const;
    double getSleepHours() const;
    Status getStatus() const;
    bool getHasNewInfo() const;
    double getAvgActivityCalories() const;

    // --- Сеттеры ---
    void setNickname(const std::string &nick);
    // Установка среднего расхода калорий. Автоматически пересчитывает нормы.
    void setAvgActivityCalories(double cal);

    // --- Ввод и обновление ---
    void inputInitialData(); // Ввод всех данных при первом запуске
    void updateNorms();      // Принудительный пересчет норм

    // --- Проверки и статусы ---
    bool isWeightLossSafe() const;     // Проверка на безопасный темп похудения
    void updateStatus(int streakDays); // Обновление статуса на основе стрика (дней подряд)
    void markInfoAsRead();             // Убрать звездочку нового статуса

    // --- Вывод и сброс ---
    void print() const; // Красивый вывод личных данных и норм
    void reset();       // Сброс всех данных, кроме никнейма (пункт "Результат")

    // --- Бинарный файл ---
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);
    static std::size_t binarySize(); // Размер записи в файле (в байтах)
};

#endif // USERPROFILE_HPP