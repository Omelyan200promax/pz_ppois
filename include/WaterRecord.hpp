#ifndef WATERRECORD_HPP
#define WATERRECORD_HPP
#include <fstream>
#include <iostream>

/*
 Класс WaterRecord хранит, сколько воды выпито за день (в миллилитрах).
 Входит в запись дневника. Норму воды считает профиль пользователя,
 здесь она передаётся параметром только для вывода
 */
class WaterRecord
{
private:
    double amount = 0.0; // выпито воды, мл

public:
    // Конструкторы
    WaterRecord() = default;
    explicit WaterRecord(double amount_);

    // Геттер
    double getAmount() const;

    // Сеттер
    void setAmount(double v);

    // Добавить к уже выпитому (например, выпил ещё стакан)
    void add(double v);

    // Ввод с консоли
    void inputFromConsole();

    // Вывод. Если norm > 0, рядом показывается норма.
    void print(double norm = 0.0) const;

    // Бинарный файл
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);

    static std::size_t binarySize();
};

#endif // WATERRECORD_HPP
