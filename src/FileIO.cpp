#include "FileIO.h"
#include <fstream>
#include <iostream>

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
        lineNum++;
        // Skip empty lines
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