#include "FileIO.h"
#include <fstream>
#include <iostream>
#include <string>

// UTF-8 BOM: 0xEF, 0xBB, 0xBF в начале файла.
// Блокнот Windows добавляет его при сохранении в UTF-8. Без удаления
// первая строка ломает парсинг (ID превращается в мусор типа "\u00ef\u00bb\u00bf1").
static void stripUtf8Bom(std::string& s) {
    if (s.size() >= 3 &&
        static_cast<unsigned char>(s[0]) == 0xEF &&
        static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF) {
        s.erase(0, 3);
    }
}

bool FileIO::load(const std::string& filename, Warehouse& warehouse) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    warehouse.clear();
    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        // BOM может появиться только в самом начале файла — снимаем один раз.
        if (lineNum == 0) stripUtf8Bom(line);

        lineNum++;
        if (line.empty()) continue;

        try {
            Product p = Product::fromFileString(line);
            if (!warehouse.addProduct(p)) {
                std::cerr << "Предупреждение: строка " << lineNum
                          << " содержит некорректные данные (пропущена): "
                          << line << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Предупреждение: ошибка парсинга строки " << lineNum
                      << ": " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Загружено товаров: " << warehouse.size() << std::endl;
    return true;
}

bool FileIO::save(const std::string& filename, const Warehouse& warehouse) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи " << filename << std::endl;
        return false;
    }

    for (const auto& product : warehouse.getAllProducts()) {
        file << product.toFileString() << "\n";
    }

    file.close();
    std::cout << "Сохранено товаров: " << warehouse.size() << std::endl;
    return true;
}