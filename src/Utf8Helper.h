#pragma once
#include <string>

namespace Utf8 {
    // Читает строку из консоли Windows корректно (в UTF-8).
    std::string readLine(const std::string& prompt = "");
    
    // Читает целое / дробное число из консоли (безопасно, без std::cin)
    int readInt(const std::string& prompt);
    double readDouble(const std::string& prompt);
    
    // Приводит UTF-8 строку к нижнему регистру (работает с кириллицей).
    std::string toLower(const std::string& s);
    
    // Проверки строк
    bool containsIgnoreCase(const std::string& haystack, const std::string& needle);
    bool equalsIgnoreCase(const std::string& a, const std::string& b);
} // namespace Utf8