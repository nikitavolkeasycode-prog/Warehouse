#define NOMINMAX
#include <Windows.h>
#include "Menu.h"
#include "FileIO.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <sstream>
#include <clocale>

Menu::Menu(const std::string& filename) : dataFilename(filename) {}

void Menu::waitForEnter() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int Menu::readInt(const std::string& prompt, bool allowEmpty) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            // Ввод оборвался (Ctrl+Z / EOF) — сбрасываем состояние cin
            std::cin.clear();
            if (allowEmpty) return -1;
            continue;
        }
        if (allowEmpty && input.empty()) return -1;
        try {
            size_t pos;
            int value = std::stoi(input, &pos);
            if (pos == input.length()) return value;
            std::cerr << "Ошибка: введите целое число.\n";
        } catch (const std::exception&) {
            std::cerr << "Ошибка: введите целое число.\n";
        }
    }
}

double Menu::readDouble(const std::string& prompt, bool allowEmpty) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            if (allowEmpty) return -1.0;
            continue;
        }
        if (allowEmpty && input.empty()) return -1.0;
        try {
            size_t pos;
            double value = std::stod(input, &pos);
            if (pos == input.length()) return value;
            std::cerr << "Ошибка: введите число.\n";
        } catch (const std::exception&) {
            std::cerr << "Ошибка: введите число.\n";
        }
    }
}

std::string Menu::readString(const std::string& prompt, bool allowEmpty) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            // Ввод оборвался (Ctrl+Z / EOF) — сбрасываем состояние cin
            std::cin.clear();
            if (allowEmpty) return "";
            continue;
        }
        if (allowEmpty && input.empty()) return "";
        if (!input.empty()) return input;
        std::cerr << "Ошибка: строка не может быть пустой.\n";
    }
}

void Menu::displayProducts(const std::vector<Product>& products, const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n";
    if (products.empty()) {
        std::cout << "Нет товаров для отображения.\n";
        return;
    }
    for (const auto& p : products) {
        std::cout << p << "\n";
        std::cout << "----------------------------------------\n";
    }
    std::cout << "Всего записей: " << products.size() << "\n";
}

void Menu::displayMainMenu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║     СКЛАДСКОЙ УЧЁТ ТОВАРОВ           ║\n";
    std::cout << "╠══════════════════════════════════════╣\n";
    std::cout << "║ 1. Добавить товар                    ║\n";
    std::cout << "║ 2. Редактировать товар               ║\n";
    std::cout << "║ 3. Удалить товар                     ║\n";
    std::cout << "║ 4. Поиск и фильтрация                ║\n";
    std::cout << "║ 5. Сортировка                        ║\n";
    std::cout << "║ 6. Аналитика                         ║\n";
    std::cout << "║ 7. Показать все товары               ║\n";
    std::cout << "║ 8. Сохранить в файл                  ║\n";
    std::cout << "║ 9. Загрузить из файла                ║\n";
    std::cout << "║ 0. Выход                              ║\n";
    std::cout << "╚══════════════════════════════════════╝\n";
    std::cout << "Выберите действие: ";
}

void Menu::handleAddProduct() {
    std::cout << "\n=== ДОБАВЛЕНИЕ НОВОГО ТОВАРА ===\n";

    int id;
    while (true) {
        id = readInt("Введите ID товара: ");
        if (!warehouse.isIdUnique(id)) {
            std::cerr << "Ошибка: товар с таким ID уже существует.\n";
        } else {
            break;
        }
    }

    std::string name = readString("Введите название товара: ");
    std::string category = readString("Введите категорию товара: ");
    double price = readDouble("Введите цену товара: ");
    while (!Warehouse::isValidPrice(price)) {
        std::cerr << "Ошибка: цена не может быть отрицательной.\n";
        price = readDouble("Введите цену товара: ");
    }

    int quantity = readInt("Введите количество товара: ");
    while (!Warehouse::isValidQuantity(quantity)) {
        std::cerr << "Ошибка: количество не может быть отрицательным.\n";
        quantity = readInt("Введите количество товара: ");
    }

    Product newProduct(id, name, category, price, quantity);
    if (warehouse.addProduct(newProduct)) {
        std::cout << "Товар успешно добавлен!\n";
    } else {
        std::cerr << "Ошибка: не удалось добавить товар.\n";
    }
}

void Menu::handleEditProduct() {
    std::cout << "\n=== РЕДАКТИРОВАНИЕ ТОВАРА ===\n";
    int id = readInt("Введите ID товара для редактирования: ");

    Product* product = warehouse.findById(id);
    if (product == nullptr) {
        std::cerr << "Ошибка: товар с ID " << id << " не найден.\n";
        return;
    }

    std::cout << "Текущие данные:\n" << *product << "\n\n";

    std::string newName = readString("Новое название (Enter - оставить без изменений): ", true);
    if (newName.empty()) newName = product->getName();

    std::string newCategory = readString("Новая категория (Enter - оставить без изменений): ", true);
    if (newCategory.empty()) newCategory = product->getCategory();

    std::string priceStr;
    double newPrice;
    while (true) {
        std::cout << "Новая цена (Enter - оставить без изменений): ";
        if (!std::getline(std::cin, priceStr)) { std::cin.clear(); continue; }
        if (priceStr.empty()) {
            newPrice = product->getPrice();
            break;
        }
        try {
            newPrice = std::stod(priceStr);
            if (Warehouse::isValidPrice(newPrice)) break;
            std::cerr << "Ошибка: цена не может быть отрицательной.\n";
        } catch (const std::exception&) {
            std::cerr << "Ошибка: введите число.\n";
        }
    }

    std::string quantStr;
    int newQuantity;
    while (true) {
        std::cout << "Новое количество (Enter - оставить без изменений): ";
        if (!std::getline(std::cin, quantStr)) { std::cin.clear(); continue; }
        if (quantStr.empty()) {
            newQuantity = product->getQuantity();
            break;
        }
        try {
            newQuantity = std::stoi(quantStr);
            if (Warehouse::isValidQuantity(newQuantity)) break;
            std::cerr << "Ошибка: количество не может быть отрицательным.\n";
        } catch (const std::exception&) {
            std::cerr << "Ошибка: введите целое число.\n";
        }
    }

    Product updated(id, newName, newCategory, newPrice, newQuantity);

    if (warehouse.editProduct(id, updated)) {
        std::cout << "Товар успешно обновлён!\n";
    } else {
        std::cerr << "Ошибка: не удалось обновить товар.\n";
    }
}

void Menu::handleRemoveProduct() {
    std::cout << "\n=== УДАЛЕНИЕ ТОВАРА ===\n";
    std::cout << "1. Удалить по ID\n";
    std::cout << "2. Удалить по названию\n";
    std::cout << "Выберите способ удаления: ";

    int choice = readInt("");
    switch (choice) {
        case 1: {
            int id = readInt("Введите ID товара для удаления: ");
            if (warehouse.removeById(id)) {
                std::cout << "Товар успешно удалён.\n";
            } else {
                std::cerr << "Ошибка: товар с таким ID не найден.\n";
            }
            break;
        }
        case 2: {
            std::string name = readString("Введите название товара для удаления: ");
            if (warehouse.removeByName(name)) {
                std::cout << "Все товары с таким названием удалены.\n";
            } else {
                std::cerr << "Ошибка: товары с таким названием не найдены.\n";
            }
            break;
        }
        default:
            std::cerr << "Неверный выбор.\n";
    }
}

void Menu::handleSearch() {
    std::cout << "\n=== ПОИСК И ФИЛЬТРАЦИЯ ===\n";
    std::cout << "1. Поиск по названию\n";
    std::cout << "2. Поиск по категории\n";
    std::cout << "3. Поиск по диапазону цен\n";
    std::cout << "Выберите тип поиска: ";

    int choice = readInt("");
    switch (choice) {
        case 1: {
            std::string query = readString("Введите название (частичное совпадение): ");
            displayProducts(warehouse.searchByName(query), "РЕЗУЛЬТАТЫ ПОИСКА ПО НАЗВАНИЮ");
            break;
        }
        case 2: {
            std::string category = readString("Введите категорию: ");
            displayProducts(warehouse.searchByCategory(category), "РЕЗУЛЬТАТЫ ПОИСКА ПО КАТЕГОРИИ");
            break;
        }
        case 3: {
            double minPrice = readDouble("Введите минимальную цену: ");
            while (!Warehouse::isValidPrice(minPrice)) {
                std::cerr << "Ошибка: цена не может быть отрицательной.\n";
                minPrice = readDouble("Введите минимальную цену: ");
            }
            double maxPrice = readDouble("Введите максимальную цену: ");
            while (!Warehouse::isValidPrice(maxPrice)) {
                std::cerr << "Ошибка: цена не может быть отрицательной.\n";
                maxPrice = readDouble("Введите максимальную цену: ");
            }
            if (minPrice > maxPrice) {
                std::swap(minPrice, maxPrice);
            }
            displayProducts(warehouse.searchByPriceRange(minPrice, maxPrice), "РЕЗУЛЬТАТЫ ПОИСКА ПО ДИАПАЗОНУ ЦЕН");
            break;
        }
        default:
            std::cerr << "Неверный выбор.\n";
    }
}

void Menu::handleSort() {
    std::cout << "\n=== СОРТИРОВКА ===\n";
    std::cout << "1. По цене (возрастание)\n";
    std::cout << "2. По цене (убывание)\n";
    std::cout << "3. По количеству (возрастание)\n";
    std::cout << "4. По количеству (убывание)\n";
    std::cout << "5. По названию (А-Я)\n";
    std::cout << "6. По названию (Я-А)\n";
    std::cout << "Выберите тип сортировки: ";

    int choice = readInt("");
    switch (choice) {
        case 1: warehouse.sortByPrice(true); break;
        case 2: warehouse.sortByPrice(false); break;
        case 3: warehouse.sortByQuantity(true); break;
        case 4: warehouse.sortByQuantity(false); break;
        case 5: warehouse.sortByName(true); break;
        case 6: warehouse.sortByName(false); break;
        default:
            std::cerr << "Неверный выбор.\n";
            return;
    }
    std::cout << "Сортировка выполнена.\n";
    displayProducts(warehouse.getAllProducts(), "ОТСОРТИРОВАННЫЙ СПИСОК ТОВАРОВ");
}

void Menu::handleAnalytics() {
    std::cout << "\n=== АНАЛИТИКА ===\n";

    // Общая стоимость
    double total = warehouse.getTotalValue();
    std::cout << "Общая стоимость всех товаров на складе: "
              << std::fixed << std::setprecision(2) << total << " руб.\n\n";

    // Товары с остатком <= 5
    auto lowStock = warehouse.getLowStockItems(5);
    displayProducts(lowStock, "ТОВАРЫ С ОСТАТКОМ \u2264 5 ШТ.");

    // Топ-3 самых дорогих
    auto topExpensive = warehouse.getTopExpensive(3);
    displayProducts(topExpensive, "ТОП-3 САМЫХ ДОРОГИХ ТОВАРОВ");
}

void Menu::handleListAll() {
    displayProducts(warehouse.getAllProducts(), "ВСЕ ТОВАРЫ НА СКЛАДЕ");
}

void Menu::handleSave() {
    std::cout << "\n=== СОХРАНЕНИЕ ===\n";
    if (FileIO::save(dataFilename, warehouse)) {
        std::cout << "Данные успешно сохранены в файл " << dataFilename << "\n";
    }
}

void Menu::handleLoad() {
    std::cout << "\n=== ЗАГРУЗКА ===\n";
    if (FileIO::load(dataFilename, warehouse)) {
        std::cout << "Данные успешно загружены из файла " << dataFilename << "\n";
    }
}

void Menu::run() {
    // На случай, если Menu::run() вызывается не из main() (тесты и т.п.),
    // повторно устанавливаем UTF-8 локаль. Реализация в Menu.h.
    enableUtf8Locale();

    std::cout << "Добро пожаловать в систему складского учёта!\n";

    // Автоматическая загрузка данных при старте
    handleLoad();

    int choice = -1;
    while (choice != 0) {
        displayMainMenu();
        choice = readInt("", false);

        switch (choice) {
            case 1: handleAddProduct(); break;
            case 2: handleEditProduct(); break;
            case 3: handleRemoveProduct(); break;
            case 4: handleSearch(); break;
            case 5: handleSort(); break;
            case 6: handleAnalytics(); break;
            case 7: handleListAll(); break;
            case 8: handleSave(); break;
            case 9: handleLoad(); break;
            case 0:
                std::cout << "Спасибо за использование системы складского учёта!\n";
                break;
            default:
                std::cerr << "Неверный выбор. Пожалуйста, выберите от 0 до 9.\n";
        }

        if (choice != 0) {
            waitForEnter();
        }
    }
}
