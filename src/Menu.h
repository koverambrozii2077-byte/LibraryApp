#pragma once
#include "Library.h"
#include <string>

class Menu {
public:
    explicit Menu(const std::string& dataFile = "data/catalog.txt");
    void run();
private:
    Library library;
    std::string dataFile;
    bool modified = false;

    void showMain();
    void showAll();
    void addBook();
    void editBook();
    void removeBook();
    void searchMenu();
    void sortMenu();
    void analyticsMenu();
    void saveToFile();
};