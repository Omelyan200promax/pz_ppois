#ifndef SLEEPRECORD_HPP
#define SLEEPRECORD_HPP

#include <fstream>
#include <iostream>

/*
 Класс SleepRecord хранит, сколько часов человек спал за день.
 Входит в запись дневника. Рекомендуемую норму сна считает профиль
 пользователя, здесь она передаётся параметром только для вывода.
 */
class SleepRecord
{
private:
    double hours = 0.0; // продолжительность сна, часы (0..24)

public:
    // Конструкторы
    SleepRecord() = default;
    explicit SleepRecord(double hours_);

    // Геттер
    double getHours() const;

    // Сеттер (принимает значения от 0 до 24)
    void setHours(double v);

    // Ввод с консоли
    void inputFromConsole();

    // Вывод. Если norm > 0, рядом показывается норма.
    void print(double norm = 0.0) const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);

    static std::size_t binarySize();
};

#endif // SLEEPRECORD_HPP
