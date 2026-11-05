#ifndef USERPROFILE_HPP
#define USERPROFILE_HPP

#include "Date.hpp"
#include <string>
#include <fstream>
#include <iostream>

enum class Goal
{
    LOSE_WEIGHT,
    CUTTING,
    GAIN_MASS,
    MAINTAIN
};
enum class Status
{
    BEGINNER,
    STARTED,
    IRON_WILL,
    MACHINE
};
enum class Gender
{
    MALE,
    FEMALE
};

class UserProfile
{
private:
    char nickname[32] = {0};
    int age = 0;
    double weight = 0.0;
    double height = 0.0;
    double bodyFat = 0.0;
    double targetWeight = 0.0;
    Goal goal = Goal::MAINTAIN;
    int weeksToGoal = 0;
    Gender gender = Gender::MALE;

    double avgActivityCalories = 0.0;

    double dailyCalories = 0.0;
    double dailyProteins = 0.0;
    double dailyFats = 0.0;
    double dailyCarbs = 0.0;
    double dailyWater = 0.0;
    double dailyFiber = 0.0;
    double basalMetabolism = 0.0;
    double sleepHours = 0.0;

    Status status = Status::BEGINNER;
    bool hasNewInfo = false;

    void calcBasalMetabolism();
    void calcDailyNorms();

public:
    UserProfile() = default;

    // Геттеры (для Diary и Report)
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

    // Сеттеры
    void setNickname(const std::string &nick);
    void setAvgActivityCalories(double cal);

    // Основные методы
    void inputInitialData();
    void updateNorms();
    bool isWeightLossSafe() const;
    void updateStatus();
    void markInfoAsRead();
    void print() const;
    void reset();

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);
    static std::size_t binarySize();
};

#endif // USERPROFILE_HPP