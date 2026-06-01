#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool FileIO::load(const std::string& filename, std::vector<Book>& out, std::string& error) {
    out.clear();
    if (!fs::exists(filename)) {
        fs::path p(filename);
        if (p.has_parent_path()) {
            try { fs::create_directories(p.parent_path()); }
            catch (const std::exception& e) {
                error = std::string("Не удалось создать директорию: ") + e.what();
                return false;
            }
        }
        std::ofstream ofs(filename);
        if (!ofs) {
            error = "Не удалось создать файл " + filename;
            return false;
        }
        return true;
    }
    
    std::ifstream ifs(filename);
    if (!ifs) {
        error = "Не удалось открыть файл " + filename;
        return false;
    }
    
    std::string line;
    int lineNumber = 0;
    while (std::getline(ifs, line)) {
        ++lineNumber;
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        
        Book b;
        std::string err;
        if (!Book::fromFileString(line, b, err)) {
            std::cerr << "[!] Строка " << lineNumber << ": " << err << " (\"" << line << "\")\n";
            continue; 
        }
        out.push_back(b);
    }
    return true;
}

bool FileIO::save(const std::string& filename, const std::vector<Book>& books, std::string& error) {
    fs::path p(filename);
    if (p.has_parent_path()) {
        try { fs::create_directories(p.parent_path()); }
        catch (const std::exception& e) {
            error = std::string("Не удалось создать директорию: ") + e.what();
            return false;
        }
    }
    
    std::ofstream ofs(filename);
    if (!ofs) {
        error = "Не удалось открыть файл для записи: " + filename;
        return false;
    }
    
    for (const auto& b : books) {
        ofs << b.toFileString() << "\n";
    }
    return true;
}