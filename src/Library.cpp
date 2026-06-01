#include "Library.h"
#include <algorithm>
#include "Utf8Helper.h"

// CRUD
bool Library::addBook(const Book& b, std::string& error) {
    std::string valError;
    if (!b.isValid(valError)) {
        error = "Ошибка валидации: " + valError;
        return false;
    }
    if (findById(b.id) != nullptr) {
        error = "Книга с ID " + std::to_string(b.id) + " уже существует";
        return false;
    }
    books.push_back(b);
    return true;
}

bool Library::editBook(int id, const Book& newData, std::string& error) {
    Book* b = findById(id);
    if (!b) {
        error = "Книга с ID " + std::to_string(id) + " не найдена";
        return false;
    }
    std::string valError;
    if (!newData.isValid(valError)) {
        error = "Ошибка валидации: " + valError;
        return false;
    }
    if (newData.id != id && findById(newData.id) != nullptr) {
        error = "Новый ID " + std::to_string(newData.id) + " уже занят";
        return false;
    }
    *b = newData;
    return true;
}

bool Library::removeById(int id, std::string& error) {
    auto it = std::find_if(books.begin(), books.end(),
        [id](const Book& b) { return b.id == id; });
    if (it == books.end()) {
        error = "Книга с ID " + std::to_string(id) + " не найдена";
        return false;
    }
    books.erase(it);
    return true;
}

bool Library::removeByTitle(const std::string& title, std::string& error) {
    auto it = std::find_if(books.begin(), books.end(),
        [&](const Book& b) { return Utf8::equalsIgnoreCase(b.title, title); });
    if (it == books.end()) {
        error = "Книга с названием \"" + title + "\" не найдена";
        return false;
    }
    books.erase(it);
    return true;
}

// Поиск
std::vector<Book> Library::searchByTitle(const std::string& substring) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (Utf8::containsIgnoreCase(b.title, substring)) result.push_back(b);
    }
    return result;
}

std::vector<Book> Library::searchByAuthor(const std::string& author) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (Utf8::containsIgnoreCase(b.author, author)) result.push_back(b);
    }
    return result;
}

std::vector<Book> Library::searchByGenre(const std::string& genre) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (Utf8::containsIgnoreCase(b.genre, genre)) result.push_back(b);
    }
    return result;
}

std::vector<Book> Library::searchByYearRange(int minYear, int maxYear) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (b.year >= minYear && b.year <= maxYear) result.push_back(b);
    }
    return result;
}

std::vector<Book> Library::searchByPriceRange(double minPrice, double maxPrice) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (b.price >= minPrice && b.price <= maxPrice) result.push_back(b);
    }
    return result;
}

// Сортировка
void Library::sortByYear(bool ascending) {
    std::sort(books.begin(), books.end(),
        [ascending](const Book& a, const Book& b) {
            return ascending ? a.year < b.year : a.year > b.year;
        });
}

void Library::sortByCopies(bool ascending) {
    std::sort(books.begin(), books.end(),
        [ascending](const Book& a, const Book& b) {
            return ascending ? a.copies < b.copies : a.copies > b.copies;
        });
}

void Library::sortByTitle(bool ascending) {
    std::sort(books.begin(), books.end(),
        [ascending](const Book& a, const Book& b) {
            std::string la = Utf8::toLower(a.title);
            std::string lb = Utf8::toLower(b.title);
            return ascending ? la < lb : la > lb;
        });
}

void Library::sortByAuthor(bool ascending) {
    std::sort(books.begin(), books.end(),
        [ascending](const Book& a, const Book& b) {
            std::string la = Utf8::toLower(a.author);
            std::string lb = Utf8::toLower(b.author);
            return ascending ? la < lb : la > lb;
        });
}

// Аналитика
double Library::totalValue() const {
    double sum = 0;
    for (const auto& b : books) sum += b.price * b.copies;
    return sum;
}

std::vector<Book> Library::lowStockItems(int threshold) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (b.copies <= threshold) result.push_back(b);
    }
    return result;
}

std::vector<Book> Library::topExpensive(int count) const {
    std::vector<Book> sorted = books;
    std::sort(sorted.begin(), sorted.end(),
        [](const Book& a, const Book& b) { return a.price > b.price; });
    if ((int)sorted.size() > count) sorted.resize(count);
    return sorted;
}

// Доступ
void Library::setAll(const std::vector<Book>& data) {
    books = data;
}

Book* Library::findById(int id) {
    for (auto& b : books) if (b.id == id) return &b;
    return nullptr;
}

const Book* Library::findById(int id) const {
    for (const auto& b : books) if (b.id == id) return &b;
    return nullptr;
}

int Library::nextId() const {
    int maxId = 0;
    for (const auto& b : books) if (b.id > maxId) maxId = b.id;
    return maxId + 1;
}