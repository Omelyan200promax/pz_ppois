#ifndef PHYSICALACTIVITY_HPP
#define PHYSICALACTIVITY_HPP
#include <string>
#include <fstream>
#include <iostream>

// Интенсивность тренировки (enum class, чтобы значения не попадали в глобальную область)
enum class Intensity
{
    Low = 0,
    Medium = 1,
    High = 2
};

// Множитель к MET: 0.8 / 1.0 / 1.2
double intensityFactor(Intensity i);

// Название интенсивности: "низкая" / "средняя" / "высокая"
std::string intensityName(Intensity i);

/*
 Класс PhysicalActivity описывает физическую активность.
 Используется в двух ролях:
 1) запись справочника (каталог активностей): название и MET, время = 0;
 2) запись в дневнике: та же активность, но с заданными временем и интенсивностью.
 MET (metabolic equivalent) - во сколько раз активность тратит больше энергии, чем покой.
 */
class PhysicalActivity
{
private:
    // Фиксированный размер массива нужен для записи в бинарный файл
    static const int NAME_SIZE = 64;

    char name[NAME_SIZE] = {0};                // название активности
    double met = 0.0;                          // MET активности
    double minutes = 0.0;                      // длительность в минутах (для записи в дневнике)
    Intensity intensity = Intensity::Medium;   // интенсивность (для записи в дневнике)

public:
    // Конструкторы
    PhysicalActivity() = default; // нужен для чтения из файла
    PhysicalActivity(const std::string &name_,
                     double met_,
                     double minutes_ = 0.0,
                     Intensity intensity_ = Intensity::Medium);

    // Геттеры
    std::string getName() const;
    double getMet() const;
    double getMinutes() const;
    Intensity getIntensity() const;

    // Сеттеры
    void setName(const std::string &newName);
    void setMet(double v);
    void setMinutes(double v);
    void setIntensity(Intensity i);

    // Расход калорий: ккал = MET * множитель интенсивности * вес (кг) * время (ч)
    // Вариант 1: берёт время и интенсивность из самой записи
    double calcKcal(double weightKg) const;
    // Вариант 2: время и интенсивность задаются явно (для расчётов по каталогу)
    double calcKcal(double weightKg, double minutes_, Intensity intensity_) const;

    // Сравнение (по названию)
    bool operator<(const PhysicalActivity &other) const;
    bool operator==(const PhysicalActivity &other) const;

    // Ввод названия и MET (для добавления в каталог)
    void inputFromConsole();

    // Ввод времени и интенсивности (для записи в дневник)
    void inputWorkoutFromConsole();

    // Подробный вывод
    void print() const;

    // Компактный вывод для списка каталога
    void printShort() const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);

    // Размер записи в файле
    static std::size_t binarySize();
};

#endif // PHYSICALACTIVITY_HPP
