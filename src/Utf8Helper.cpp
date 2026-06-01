#include "Utf8Helper.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Utf8 {

#ifdef _WIN32
static std::wstring toWide(const std::string& s) {
    if (s.empty()) return {};
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (len <= 0) return {};
    std::wstring ws(len - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), len);
    return ws;
}

static std::string toUtf8(const std::wstring& ws) {
    if (ws.empty()) return {};
    int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (len <= 0) return {};
    std::string s(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, s.data(), len, nullptr, nullptr);
    return s;
}
#endif

std::string readLine(const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
        std::cout.flush();
    }
#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    wchar_t buffer[1024];
    DWORD read = 0;
    if (!ReadConsoleW(hIn, buffer, 1023, &read, nullptr) || read == 0) {
        return "";
    }
    while (read > 0 && (buffer[read - 1] == L'\r' || buffer[read - 1] == L'\n')) {
        --read;
    }
    return toUtf8(std::wstring(buffer, read));
#else
    std::string s;
    std::getline(std::cin, s);
    return s;
#endif
}

int readInt(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            size_t pos = 0;
            int val = std::stoi(s, &pos);
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos == s.size()) return val;
        } catch (...) {}
        std::cout << "[!] Ошибка ввода. Введите целое число.\n" << std::flush;
    }
}

double readDouble(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        try {
            size_t pos = 0;
            double val = std::stod(s, &pos);
            while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
            if (pos == s.size()) return val;
        } catch (...) {}
        std::cout << "[!] Ошибка ввода. Введите число.\n" << std::flush;
    }
}

std::string toLower(const std::string& s) {
#ifdef _WIN32
    std::wstring ws = toWide(s);
    for (wchar_t& c : ws) c = towlower(c);
    return toUtf8(ws);
#else
    std::string result = s;
    for (char& c : result) c = std::tolower(c); 
    return result;
#endif
}

bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    return toLower(haystack).find(toLower(needle)) != std::string::npos;
}

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toLower(a) == toLower(b);
}

} // namespace Utf8