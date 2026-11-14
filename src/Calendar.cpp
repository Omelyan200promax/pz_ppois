#include "Calendar.hpp"

// Конструктор
Calendar::Calendar(Diary &d) : diary(d)
{
}

// Вывод всех записей
void Calendar::printAll() const
{
    if (diary.isEmpty())
    {
        std::cout << "Дневник пуст. Записей нет.\n";
        return;
    }

    // Выводим все записи через метод Diary
    diary.printAll();
}

// Поиск по дате
void Calendar::searchByDate(const Date &date) const
{
    // Ищем запись, но так как метод findByDate в Diary не константный,
    // нам придется использовать метод getAll для поиска
    const auto &entries = diary.getAll();
    bool found = false;

    for (const auto &entry : entries)
    {
        if (entry.getDate() == date)
        {
            entry.print(); // Выводим подробную информацию о записи
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Запись на дату " << date.toString() << " не найдена.\n";
    }
}

// Удаление по дате
bool Calendar::deleteByDate(const Date &date)
{
    if (diary.removeEntry(date))
    {
        std::cout << "Запись на " << date.toString() << " успешно удалена.\n";
        return true;
    }

    std::cout << "Ошибка: запись на " << date.toString() << " не найдена.\n";
    return false;
}

// Получить запись для редактирования
DiaryEntry *Calendar::getEntryForEditing(const Date &date)
{
    // Используем метод из Diary, который ищет запись (и создает, если нет)
    return &diary.getOrCreateEntry(date);
}