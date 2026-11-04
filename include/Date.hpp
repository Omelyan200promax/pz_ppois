#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <fstream>
#include <iostream>

/*
 Класс Date описывает дату.
 Используется для привязки записей дневника к конкретному дню,
 сортировки в календаре и поиска.
 Хранит день, месяц и год.
 */
class Date
{
private:
    int day = 1;
    int month = 1;
    int year = 2000;

    // Вспомогательный метод для проверки, является ли год високосным
    bool isLeapYear(int y) const;

    // Вспомогательный метод: сколько дней в месяце
    int daysInMonth(int m, int y) const;

public:
    // Конструкторы
    Date();                    // по умолчанию (например, 01.01.2000)
    Date(int d, int m, int y); // с параметрами

    // Геттеры
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Сеттер
    void setDate(int d, int m, int y);

    // Проверка на корректность (например, 31.02 - некорректно)
    bool isValid() const;

    // Форматирование в строку "DD.MM.YYYY"
    std::string toString() const;

    // Парсинг из строки "DD.MM.YYYY"
    static Date fromString(const std::string &str);

    // Операторы сравнения (нужны для сортировки в календаре)
    bool operator==(const Date &other) const;
    bool operator!=(const Date &other) const;
    bool operator<(const Date &other) const;
    bool operator<=(const Date &other) const;
    bool operator>(const Date &other) const;
    bool operator>=(const Date &other) const;

    // Ввод с консоли с валидацией (пользователь не сможет ввести 32 января)
    void inputFromConsole();

    // Вывод в консоль
    void print() const;

    // Работа с бинарным файлом
    void save(std::ofstream &out) const;
    void load(std::ifstream &in);

    static std::size_t binarySize();
};

#endif // DATE_HPP