cmake_minimum_required(VERSION 3.16)
project(LibraryApp LANGUAGES CXX)

# Требуем C++17 (filesystem, structured bindings и т.д.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/Book.cpp
    src/Library.cpp
    src/FileIO.cpp
    src/Menu.cpp
    src/Utf8Helper.cpp
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# include-пути (чтобы #include "Book.h" работал из любого .cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Флаги компилятора
if(MSVC)
    # /utf-8 — корректная работа с кириллицей в исходниках и строковых литералах
    # /W4 — строгие предупреждения
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /W4)
else()
    # GCC / Clang
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
        -finput-charset=UTF-8
        -fexec-charset=UTF-8
    )
endif()

# Линковка std::filesystem (нужна только для старых GCC < 9)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()

# Копируем папку data/ в директорию со скомпилированным бинарником,
# чтобы программа могла найти data/catalog.txt при запуске.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
    COMMENT "Copying data/ to output directory"
)