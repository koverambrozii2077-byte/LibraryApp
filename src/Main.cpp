#include "Menu.h"
#include "Utf8Helper.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Корректное отображение кириллицы в Windows
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(hIn, &mode)) {
        mode |= ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;
        mode &= ~ENABLE_QUICK_EDIT_MODE;
        SetConsoleMode(hIn, mode);
    }
#endif

    try {
        Menu app("data/catalog.txt");
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "[КРИТИЧЕСКАЯ ОШИБКА] " << e.what() << "\n";
        return 1;
    }
    return 0;
}