#include "TestMenu.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

// Точка входа для проверки своих классов (не заменяет app/main.cpp проекта)
int main()
{
#ifdef _WIN32
    // Чтобы русский текст нормально отображался в консоли Windows
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    TestMenu menu;
    menu.run();
    return 0;
}
