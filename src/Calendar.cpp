#include "Calendar.hpp"

// Конструктор: сохраняет ссылку на дневник для дальнейшей работы
Calendar::Calendar(Diary &d) : diary(d)
{
}

// Выводит список всех записей дневника, если они есть
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

// Ищет запись по конкретной дате и выводит её на экран
void Calendar::searchByDate(const Date &date) const
{
    // Ищем запись через getAll, так как сам метод поиска не константный
    const auto &entries = diary.getAll();
    bool found = false;

    for (const auto &entry : entries)
    {
        if (entry.getDate() == date)
        {
            entry.print(); // Выводим подробную информацию о найденной записи
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "Запись на дату " << date.toString() << " не найдена.\n";
    }
}

// Удаляет запись из дневника по указанной дате
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

// Возвращает указатель на запись для редактирования (создает новую, если такой нет)
DiaryEntry *Calendar::getEntryForEditing(const Date &date)
{
    // Используем метод из Diary, который ищет запись (и создает, если нет)
    return &diary.getOrCreateEntry(date);
}