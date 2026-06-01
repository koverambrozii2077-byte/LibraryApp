#pragma once
#include "Book.h"
#include <vector>
#include <string>

class Library {
public:
    Library() = default;

    // CRUD
    bool addBook(const Book& b, std::string& error);
    bool editBook(int id, const Book& newData, std::string& error);
    bool removeById(int id, std::string& error);
    bool removeByTitle(const std::string& title, std::string& error);

    // Поиск
    std::vector<Book> searchByTitle(const std::string& substring) const;
    std::vector<Book> searchByAuthor(const std::string& author) const;
    std::vector<Book> searchByGenre(const std::string& genre) const;
    std::vector<Book> searchByYearRange(int minYear, int maxYear) const;
    std::vector<Book> searchByPriceRange(double minPrice, double maxPrice) const;

    // Сортировка (меняет внутренний порядок)
    void sortByYear(bool ascending);
    void sortByCopies(bool ascending);
    void sortByTitle(bool ascending);
    void sortByAuthor(bool ascending);

    // Аналитика
    double totalValue() const;
    std::vector<Book> lowStockItems(int threshold = 2) const;
    std::vector<Book> topExpensive(int count = 3) const;

    // Доступ к данным
    const std::vector<Book>& getAll() const { return books; }
    void setAll(const std::vector<Book>& data);
    Book* findById(int id);
    const Book* findById(int id) const;
    int nextId() const;
    void clear() { books.clear(); }

private:
    std::vector<Book> books;
};