#include "Date.hpp"

#include <sstream>
#include <iomanip>
#include <limits>
#include <ctime>

// Конструктор с параметрами: создает дату и проверяет её на корректность
Date::Date(int d, int m, int y) : day(d), month(m), year(y)
{
    // Если передали некорректную дату (например, 31.02), сбрасываем на дефолт
    if (!isValid())
    {
        day = 1;
        month = 1;
        year = 2000;
    }
}

// Возвращает день
int Date::getDay() const { return day; }

// Возвращает месяц
int Date::getMonth() const { return month; }

// Возвращает год
int Date::getYear() const { return year; }

// Устанавливает новую дату с проверкой на корректность
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

// Проверяет, является ли год високосным
bool Date::isLeapYear(int y) const
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Возвращает количество дней в указанном месяце (с учетом високосного года)
int Date::daysInMonth(int m, int y) const
{
    if (m == 4 || m == 6 || m == 9 || m == 11)
        return 30;
    if (m == 2)
        return isLeapYear(y) ? 29 : 28;
    return 31;
}

// Проверяет, существует ли такая дата в календаре
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

// Преобразует дату в строку формата "ДД.ММ.ГГГГ"
std::string Date::toString() const
{
    std::ostringstream oss;
    // Форматируем так, чтобы было 01.05.2024, а не 1.5.2024
    oss << std::setfill('0') << std::setw(2) << day << "."
        << std::setfill('0') << std::setw(2) << month << "."
        << year;
    return oss.str();
}

// Создает объект даты из строки формата "ДД.ММ.ГГГГ"
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

// Оператор равенства: сравнивает день, месяц и год
bool Date::operator==(const Date &other) const
{
    return day == other.day && month == other.month && year == other.year;
}

// Оператор неравенства
bool Date::operator!=(const Date &other) const
{
    return !(*this == other);
}

// Оператор "меньше" (используется для сортировки записей по дате)
bool Date::operator<(const Date &other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

// Оператор "меньше или равно"
bool Date::operator<=(const Date &other) const
{
    return *this < other || *this == other;
}

// Оператор "больше"
bool Date::operator>(const Date &other) const
{
    return !(*this <= other);
}

// Оператор "больше или равно"
bool Date::operator>=(const Date &other) const
{
    return !(*this < other);
}

// Запрашивает дату у пользователя с консоли (с поддержкой команды 'now')
void Date::inputFromConsole()
{
    while (true)
    {
        std::cout << "Введите дату в формате ДД.ММ.ГГГГ (или 'now' для текущей): ";
        std::string input;
        std::getline(std::cin >> std::ws, input);

        // Если введено 'now', берём текущую системную дату
        if (input == "now")
        {
            std::time_t t = std::time(nullptr);
            std::tm *now = std::localtime(&t);
            day = now->tm_mday;
            month = now->tm_mon + 1;
            year = now->tm_year + 1900;
            return;
        }

        // Пытаемся распарсить строку напрямую
        std::istringstream iss(input);
        char dot1, dot2;
        int d_val = 0, m_val = 0, y_val = 0;

        if (iss >> d_val >> dot1 >> m_val >> dot2 >> y_val && dot1 == '.' && dot2 == '.')
        {
            Date temp(d_val, m_val, y_val);
            if (temp.isValid())
            {
                *this = temp;
                return;
            }
        }

        // Если не получилось или дата некорректна
        std::cout << "Ошибка: некорректная дата. Попробуйте снова.\n";
    }
}

// Выводит дату в консоль
void Date::print() const
{
    std::cout << toString();
}

// Сохраняет дату в бинарный файл (3 числа типа int)
void Date::save(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&day), sizeof(int));
    out.write(reinterpret_cast<const char *>(&month), sizeof(int));
    out.write(reinterpret_cast<const char *>(&year), sizeof(int));
}

// Загружает дату из бинарного файла
void Date::load(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&day), sizeof(int));
    in.read(reinterpret_cast<char *>(&month), sizeof(int));
    in.read(reinterpret_cast<char *>(&year), sizeof(int));
}

// Возвращает размер записи даты в файле (в байтах)
std::size_t Date::binarySize()
{
    return sizeof(int) * 3; // 12 байт
}