#include "Menu.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <clocale>
#include <Windows.h>

int main() {
    // --- Корректное отображение и ввод кириллицы ---
    // Переключаем консоль Windows на кодовую страницу UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // Устанавливаем локаль C/C++ в UTF-8, чтобы std::cin/cout работали
    // с кириллицей корректно (иначе std::getline читает байты в старой
    // кодировке и русские буквы "портятся" или попадают как пустая строка)
    setlocale(LC_ALL, ".UTF8");
    std::locale::global(std::locale(".UTF8"));

    // Пытаемся найти файл данных в нескольких возможных местах
    // (запуск из Visual Studio меняет рабочую директорию)
    std::string dataFilename;

    const std::vector<std::string> candidates = {
        "data/products.txt",
        "../data/products.txt",
        "../../data/products.txt",
        "../../../data/products.txt"
    };

    for (const auto& path : candidates) {
        if (std::filesystem::exists(path)) {
            dataFilename = path;
            break;
        }
    }

    if (dataFilename.empty()) {
        std::cerr << "Ошибка: не удалось найти файл data/products.txt ни в одной из директорий:\n";
        for (const auto& path : candidates) {
            std::cerr << "  - " << std::filesystem::absolute(path).string() << "\n";
        }
        std::cerr << "Текущая рабочая директория: "
                  << std::filesystem::current_path().string() << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Используется файл данных: "
              << std::filesystem::absolute(dataFilename).string() << "\n";

    Menu menu(dataFilename);
    menu.run();

    return EXIT_SUCCESS;
}
