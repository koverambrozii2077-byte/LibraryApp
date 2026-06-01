#include "Book.h"
#include <sstream>
#include <iomanip>
#include <vector>

Book::Book(int id, const std::string& title, const std::string& author, const std::string& genre,
           int year, int copies, double price)
    : id(id), title(title), author(author), genre(genre), year(year), copies(copies), price(price) {}

bool Book::isValid(std::string& error) const {
    if (title.empty()) { error = "Название не может быть пустым"; return false; }
    if (author.empty()) { error = "Автор не может быть пустым"; return false; }
    if (genre.empty()) { error = "Жанр не может быть пустым"; return false; }
    if (year <= 0) { error = "Год издания должен быть > 0"; return false; }
    if (copies < 0) { error = "Количество экземпляров не может быть отрицательным"; return false; }
    if (price < 0) { error = "Цена не может быть отрицательной"; return false; }
    return true;
}

std::string Book::toFileString() const {
    std::ostringstream oss;
    oss << id << "," << title << "," << author << "," << genre << ","
        << year << "," << copies << "," << std::fixed << std::setprecision(2) << price;
    return oss.str();
}

bool Book::fromFileString(const std::string& line, Book& out, std::string& error) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    if (tokens.size() != 7) {
        error = "Неверный формат строки (ожидается 7 полей)";
        return false;
    }
    try {
        out.id = std::stoi(tokens[0]);
        out.title = tokens[1];
        out.author = tokens[2];
        out.genre = tokens[3];
        out.year = std::stoi(tokens[4]);
        out.copies = std::stoi(tokens[5]);
        out.price = std::stod(tokens[6]);
    } catch (const std::exception& e) {
        error = std::string("Ошибка парсинга: ") + e.what();
        return false;
    }
    return out.isValid(error);
}

void Book::printHeader() {
    std::cout << std::left
        << std::setw(5)  << "ID"
        << std::setw(25) << "Название"
        << std::setw(20) << "Автор"
        << std::setw(15) << "Жанр"
        << std::setw(6)  << "Год"
        << std::setw(6)  << "Кол-во"
        << std::setw(10) << "Цена"
        << "\n";
    std::cout << std::string(87, '-') << "\n";
}

void Book::print() const {
    std::cout << std::left
        << std::setw(5)  << id
        << std::setw(25) << title
        << std::setw(20) << author
        << std::setw(15) << genre
        << std::setw(6)  << year
        << std::setw(6)  << copies
        << std::setw(10) << std::fixed << std::setprecision(2) << price
        << "\n";
}