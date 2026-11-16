/*#include "TestDevMenu.hpp"*/
#include "menu.hpp"
#include <windows.h> // Подключаем библиотеку для работы с консолью Windows

int main()
{
    // Принудительно включаем UTF-8 для русского языка в консоли
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Menu menu;
    menu.run();
    return 0;
    /*TestDevMenu test;
    test.run();
    return 0;*/
}