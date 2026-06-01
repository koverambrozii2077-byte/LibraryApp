#include "Menu.h"
#include "FileIO.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include "Utf8Helper.h"

Menu::Menu(const std::string& dataFile) : dataFile(dataFile) {}

void Menu::run() {
    std::string err;
    std::vector<Book> loaded;
    if (!FileIO::load(dataFile, loaded, err)) {
        std::cout << "[!] Ошибка загрузки файла: " << err << "\n";
    } else {
        library.setAll(loaded);
        std::cout << "[i] Загружено книг: " << loaded.size() << "\n";
    }

    bool running = true;
    while (running) {
        showMain();
        int choice = Utf8::readInt("Ваш выбор: ");
        std::cout << "\n";
        switch (choice) {
            case 1: showAll(); break;
            case 2: addBook(); break;
            case 3: editBook(); break;
            case 4: removeBook(); break;
            case 5: searchMenu(); break;
            case 6: sortMenu(); break;
            case 7: analyticsMenu(); break;
            case 8: saveToFile(); break;
            case 0: running = false; break;
            default: std::cout << "[!] Неизвестный пункт меню.\n";
        }
    }

    if (modified) {
        std::cout << "\n[!] Есть несохранённые изменения.\n";
        std::string ans = Utf8::readLine("Сохранить перед выходом? (y/n): ");
        if (ans == "y" || ans == "Y" || ans == "д" || ans == "Д") {
            saveToFile();
        }
    }
    std::cout << "До свидания!\n";
}

void Menu::showMain() {
    std::cout << "\n=========================================\n";
    std::cout << "|       БИБЛИОТЕЧНЫЙ КАТАЛОГ\n";
    std::cout << "|========================================\n";
    std::cout << "| 1. Добавить книгу\n";
    std::cout << "| 2. Редактировать книгу\n";
    std::cout << "| 3. Удалить книгу\n";
    std::cout << "| 4. Поиск и фильтрация\n";
    std::cout << "| 5. Сортировка\n";
    std::cout << "| 6. Показать все книги\n";
    std::cout << "| 7. Аналитика\n";
    std::cout << "| 8. Сохранить в файл\n";
    std::cout << "| 0. Выход\n";
    std::cout << "|========================================\n";
    std::cout << "|Выберите действие: ";
}

void Menu::showAll() {
    const auto& all = library.getAll();
    if (all.empty()) {
        std::cout << "Каталог пуст.\n";
        return;
    }
    Book::printHeader();
    for (const auto& b : all) b.print();
    std::cout << "Всего позиций: " << all.size() << "\n";
}

void Menu::addBook() {
    std::cout << "--- Добавление книги ---\n";
    int id = library.nextId();
    std::cout << "Автоматически присвоен ID: " << id << "\n";
    std::string title = Utf8::readLine("Название: ");
    std::string author = Utf8::readLine("Автор: ");
    std::string genre = Utf8::readLine("Жанр: ");
    int year = Utf8::readInt("Год издания: ");
    int copies = Utf8::readInt("Количество экземпляров: ");
    double price = Utf8::readDouble("Цена: ");

    Book b(id, title, author, genre, year, copies, price);
    std::string err;
    if (!library.addBook(b, err)) {
        std::cout << "[!] " << err << "\n";
        return;
    }
    modified = true;
    std::cout << "[+] Книга успешно добавлена.\n";
}

void Menu::editBook() {
    int id = Utf8::readInt("Введите ID книги для редактирования: ");
    const Book* existing = library.findById(id);
    if (!existing) {
        std::cout << "[!] Книга с таким ID не найдена.\n";
        return;
    }

    std::cout << "Текущие данные:\n";
    Book::printHeader();
    existing->print();
    std::cout << "\nОставьте поле пустым, чтобы не менять значение.\n";

    Book newData = *existing;
    std::string s;
    
    s = Utf8::readLine("Название [" + existing->title + "]: ");
    if (!s.empty()) newData.title = s;
    
    s = Utf8::readLine("Автор [" + existing->author + "]: ");
    if (!s.empty()) newData.author = s;
    
    s = Utf8::readLine("Жанр [" + existing->genre + "]: ");
    if (!s.empty()) newData.genre = s;
    
    s = Utf8::readLine("Год издания [" + std::to_string(existing->year) + "]: ");
    if (!s.empty()) {
        try { newData.year = std::stoi(s); }
        catch (...) { std::cout << "[!] Некорректное значение, оставляем старое.\n"; }
    }
    
    s = Utf8::readLine("Количество [" + std::to_string(existing->copies) + "]: ");
    if (!s.empty()) {
        try { newData.copies = std::stoi(s); }
        catch (...) { std::cout << "[!] Некорректное значение, оставляем старое.\n"; }
    }
    
    s = Utf8::readLine("Цена [" + std::to_string(existing->price) + "]: ");
    if (!s.empty()) {
        try { newData.price = std::stod(s); }
        catch (...) { std::cout << "[!] Некорректное значение, оставляем старое.\n"; }
    }

    std::string err;
    if (!library.editBook(id, newData, err)) {
        std::cout << "[!] " << err << "\n";
        return;
    }
    modified = true;
    std::cout << "[+] Книга обновлена.\n";
}

void Menu::removeBook() {
    std::cout << "Удалить по:\n1. ID\n2. Названию\n";
    int mode = Utf8::readInt("Ваш выбор: ");
    std::string err;
    if (mode == 1) {
        int id = Utf8::readInt("ID: ");
        if (library.removeById(id, err)) {
            modified = true;
            std::cout << "[+] Книга удалена.\n";
        } else {
            std::cout << "[!] " << err << "\n";
        }
    } else if (mode == 2) {
        std::string title = Utf8::readLine("Название: ");
        if (library.removeByTitle(title, err)) {
            modified = true;
            std::cout << "[+] Книга удалена.\n";
        } else {
            std::cout << "[!] " << err << "\n";
        }
    } else {
        std::cout << "[!] Неверный выбор.\n";
    }
}

void Menu::searchMenu() {
    std::cout << "--- Поиск ---\n";
    std::cout << " 1. По названию\n";
    std::cout << " 2. По автору\n";
    std::cout << " 3. По жанру\n";
    std::cout << " 4. По диапазону года издания\n";
    std::cout << " 5. По диапазону цен\n";
    int c = Utf8::readInt("Ваш выбор: ");
    std::vector<Book> result;

    if (c == 1) {
        std::string q = Utf8::readLine("Введите часть названия: ");
        result = library.searchByTitle(q);
    } else if (c == 2) {
        std::string q = Utf8::readLine("Введите автора: ");
        result = library.searchByAuthor(q);
    } else if (c == 3) {
        std::string q = Utf8::readLine("Введите жанр: ");
        result = library.searchByGenre(q);
    } else if (c == 4) {
        int mn = Utf8::readInt("Год от: ");
        int mx = Utf8::readInt("Год до: ");
        result = library.searchByYearRange(mn, mx);
    } else if (c == 5) {
        double mn = Utf8::readDouble("Цена от: ");
        double mx = Utf8::readDouble("Цена до: ");
        result = library.searchByPriceRange(mn, mx);
    } else {
        std::cout << "[!] Неверный выбор.\n";
        return;
    }

    if (result.empty()) {
        std::cout << "Ничего не найдено.\n";
        return;
    }
    Book::printHeader();
    for (const auto& b : result) b.print();
    std::cout << "Найдено: " << result.size() << "\n";
}

void Menu::sortMenu() {
    std::cout << "--- Сортировка ---\n";
    std::cout << " 1. По году издания (возр.)\n";
    std::cout << " 2. По году издания (убыв.)\n";
    std::cout << " 3. По количеству (возр.)\n";
    std::cout << " 4. По количеству (убыв.)\n";
    std::cout << " 5. По названию (А-Я)\n";
    std::cout << " 6. По автору (А-Я)\n";
    int c = Utf8::readInt("Ваш выбор: ");
    switch (c) {
        case 1: library.sortByYear(true); break;
        case 2: library.sortByYear(false); break;
        case 3: library.sortByCopies(true); break;
        case 4: library.sortByCopies(false); break;
        case 5: library.sortByTitle(true); break;
        case 6: library.sortByAuthor(true); break;
        default: std::cout << "[!] Неверный выбор.\n"; return;
    }
    modified = true;
    std::cout << "[+] Отсортировано. Текущий список:\n";
    showAll();
}

void Menu::analyticsMenu() {
    std::cout << "--- Аналитика ---\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Общая стоимость книжного фонда: " << library.totalValue() << "\n";
    
    auto low = library.lowStockItems(2);
    std::cout << "Книги, требующие пополнения (остаток <= 2 шт.) (" << low.size() << "):\n";
    if (!low.empty()) {
        Book::printHeader();
        for (const auto& b : low) b.print();
    }
    
    auto top = library.topExpensive(3);
    std::cout << "\nТоп-3 самых дорогих книг:\n";
    if (!top.empty()) {
        Book::printHeader();
        for (const auto& b : top) b.print();
    }
}

void Menu::saveToFile() {
    std::string err;
    if (FileIO::save(dataFile, library.getAll(), err)) {
        modified = false;
        std::cout << "[+] Данные сохранены в " << dataFile << "\n";
    } else {
        std::cout << "[!] Ошибка сохранения: " << err << "\n";
    }
}
