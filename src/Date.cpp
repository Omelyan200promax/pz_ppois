#include "Date.hpp"

#include <sstream>
#include <iomanip>
#include <limits>
#include <ctime>

// Конструктор

Date::Date(int d, int m, int y) : day(d), month(m), year(y)
{
    // Если передали мусор, сбрасываем на дефолт
    if (!isValid())
    {
        day = 1;
        month = 1;
        year = 2000;
    }
}

// Геттеры

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

// Сеттер

void Date::setDate(int d, int m, int y)
{
    Date temp(d, m, y);
    if (temp.isValid())
    {
        day = d;
        month = m;
        year = y;
    }
}

// Вспомогательные методы

bool Date::isLeapYear(int y) const
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) const
{
    if (m == 4 || m == 6 || m == 9 || m == 11)
        return 30;
    if (m == 2)
        return isLeapYear(y) ? 29 : 28;
    return 31;
}

bool Date::isValid() const
{
    if (year < 1900 || year > 2100)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > daysInMonth(month, year))
        return false;
    return true;
}

// Форматирование и парсинг

std::string Date::toString() const
{
    std::ostringstream oss;
    // Форматируем так, чтобы было 01.05.2024, а не 1.5.2024
    oss << std::setfill('0') << std::setw(2) << day << "."
        << std::setfill('0') << std::setw(2) << month << "."
        << year;
    return oss.str();
}

Date Date::fromString(const std::string &str)
{
    Date d;
    std::istringstream iss(str);
    char dot1, dot2;
    int d_val, m_val, y_val;

    // Пытаемся прочитать формат DD.MM.YYYY
    if (iss >> d_val >> dot1 >> m_val >> dot2 >> y_val)
    {
        if (dot1 == '.' && dot2 == '.')
        {
            d.setDate(d_val, m_val, y_val);
        }
    }
    return d;
}

// Операторы сравнения

bool Date::operator==(const Date &other) const
{
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator!=(const Date &other) const
{
    return !(*this == other);
}

bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

bool Date::operator<=(const Date &other) const
{
    return *this < other || *this == other;
}

bool Date::operator>(const Date &other) const
{
    return !(*this <= other);
}

bool Date::operator>=(const Date &other) const
{
    return !(*this < other);
}

// Ввод с консоли

void Date::inputFromConsole()
{
    while (true)
    {
        std::cout << "Введите дату в формате ДД.ММ.ГГГГ (или 'now' для текущей): ";
        std::string input;
        std::getline(std::cin >> std::ws, input);

        if (input == "now")
        {
            std::time_t t = std::time(nullptr);
            std::tm *now = std::localtime(&t);
            day = now->tm_mday;
            month = now->tm_mon + 1;    // tm_mon от 0 до 11
            year = now->tm_year + 1900; // tm_year от 1900
            return;
        }

        Date temp = fromString(input);
        if (temp.isValid())
        {
            *this = temp;
            return;
        }
        std::cout << "Ошибка: некорректная дата. Попробуйте снова.\n";
    }
}

// Вывод в консоль

void Date::print() const
{
    std::cout << toString();
}

// Бинарный файл

void Date::save(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&day), sizeof(int));
    out.write(reinterpret_cast<const char *>(&month), sizeof(int));
    out.write(reinterpret_cast<const char *>(&year), sizeof(int));
}

void Date::load(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&day), sizeof(int));
    in.read(reinterpret_cast<char *>(&month), sizeof(int));
    in.read(reinterpret_cast<char *>(&year), sizeof(int));
}

std::size_t Date::binarySize()
{
    return sizeof(int) * 3; // 12 байт
}