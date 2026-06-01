#pragma once
#include "Book.h"
#include <vector>
#include <string>

class FileIO {
public:
    static bool load(const std::string& filename, std::vector<Book>& out, std::string& error);
    static bool save(const std::string& filename, const std::vector<Book>& books, std::string& error);
};