#ifndef MENU_H
#define MENU_H

#include "Warehouse.h"
#include <string>

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