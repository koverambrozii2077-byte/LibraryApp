#pragma once
#include <string>
#include <iostream>

class Book {
public:
    int id;
    std::string title;
    std::string author;
    std::string genre;
    int year;
    int copies;
    double price;

    Book() : id(0), title(""), author(""), genre(""), year(0), copies(0), price(0.0) {}
    Book(int id, const std::string& title, const std::string& author, const std::string& genre,
         int year, int copies, double price);

    // Валидация полей
    bool isValid(std::string& error) const;
    
    // Преобразование в строку для сохранения в файл
    std::string toFileString() const;
    
    // Парсинг из строки файла
    static bool fromFileString(const std::string& line, Book& out, std::string& error);
    
    // Вывод в консоль
    void print() const;
    static void printHeader();
};