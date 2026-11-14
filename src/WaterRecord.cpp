#include "WaterRecord.hpp"

#include <iomanip> // std::fixed, std::setprecision
#include <limits>  // std::numeric_limits
#include <string>

// Конструктор: создает запись о воде с указанным объемом (в мл)
WaterRecord::WaterRecord(double amount_)
{
    setAmount(amount_);
}

// Возвращает количество выпитой воды (в мл)
double WaterRecord::getAmount() const { return amount; }

// Устанавливает количество воды (с проверкой на отрицательные значения)
void WaterRecord::setAmount(double v)
{
    if (v >= 0.0)
        amount = v;
}

// Добавляет объем воды к уже выпитому (например, выпил еще стакан)
void WaterRecord::add(double v)
{
    if (v >= 0.0)
        amount += v;
}

// Запрашивает у пользователя количество выпитой воды с консоли
void WaterRecord::inputFromConsole()
{
    auto readDouble = [](const std::string &prompt) -> double
    {
        double v;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> v)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера
                if (v >= 0.0)
                    return v;
                std::cout << "Ошибка: значение не может быть отрицательным.\n";
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: введите число.\n";
            }
        }
    };

    amount = readDouble("Сколько воды выпито за день (мл): ");
}

// Выводит информацию о выпитой воде (и норму, если она передана)
void WaterRecord::print(double norm) const
{
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Вода: " << amount << " мл";
    if (norm > 0.0)
        std::cout << " (норма: " << norm << " мл)";
    std::cout << "\n";
}

// Сохраняет запись о воде в бинарный файл
void WaterRecord::save(std::ofstream &out) const
{
    out.write(reinterpret_cast<const char *>(&amount), sizeof(double));
}

// Загружает запись о воде из бинарного файла
void WaterRecord::load(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&amount), sizeof(double));
}

// Возвращает размер записи о воде в файле (в байтах)
std::size_t WaterRecord::binarySize()
{
    return sizeof(double);
}