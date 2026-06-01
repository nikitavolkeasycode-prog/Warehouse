#ifndef MENU_H
#define MENU_H

#include "Warehouse.h"
#include <string>
#include <clocale>
#include <Windows.h>

// Кросс-компиляторная (MSVC / MinGW) установка локали в UTF-8.
// Должна вызываться ДО любых операций с std::cin / std::cout / std::getline,
// иначе кириллица будет читаться/печататься в "сыром" виде.
inline void enableUtf8Locale() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!setlocale(LC_ALL, ".UTF8")) {
        if (!setlocale(LC_ALL, "en_US.UTF-8")) {
            if (!setlocale(LC_ALL, "ru_RU.UTF-8")) {
                setlocale(LC_ALL, "");
            }
        }
    }

    try {
        std::locale::global(std::locale(".UTF8"));
    } catch (const std::exception&) {
        try {
            std::locale::global(std::locale("en_US.UTF-8"));
        } catch (const std::exception&) {
            try {
                std::locale::global(std::locale("ru_RU.UTF-8"));
            } catch (const std::exception&) {
                try { std::locale::global(std::locale("")); }
                catch (...) { std::locale::global(std::locale::classic()); }
            }
        }
    }
}

class Menu {
private:
    Warehouse warehouse;
    std::string dataFilename;

    void displayMainMenu();
    void displayProducts(const std::vector<Product>& products, const std::string& title);

    // Menu handlers
    void handleAddProduct();
    void handleEditProduct();
    void handleRemoveProduct();
    void handleSearch();
    void handleSort();
    void handleAnalytics();
    void handleListAll();
    void handleSave();
    void handleLoad();

    // Input helpers
    int readInt(const std::string& prompt, bool allowEmpty = false);
    double readDouble(const std::string& prompt, bool allowEmpty = false);
    std::string readString(const std::string& prompt, bool allowEmpty = false);
    void waitForEnter();

public:
    Menu(const std::string& filename);
    void run();
};

#endif // MENU_H