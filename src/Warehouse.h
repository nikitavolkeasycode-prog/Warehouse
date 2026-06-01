#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Product.h"
#include <vector>
#include <string>
#include <functional>

class Warehouse {
private:
    std::vector<Product> products;

    static std::string toLower(const std::string& str);

public:
    Warehouse() = default;

    // CRUD operations
    bool addProduct(const Product& product);
    bool editProduct(int id, const Product& updatedProduct);
    bool removeById(int id);
    bool removeByName(const std::string& name);

    // Validation
    static bool isValidPrice(double price);
    static bool isValidQuantity(int quantity);
    static bool isValidString(const std::string& str);
    bool isIdUnique(int id) const;

    // Search and filter
    std::vector<Product> searchByName(const std::string& query) const;
    std::vector<Product> searchByCategory(const std::string& category) const;
    std::vector<Product> searchByPriceRange(double minPrice, double maxPrice) const;

    // Sorting
    void sortByPrice(bool ascending = true);
    void sortByQuantity(bool ascending = true);
    void sortByName(bool ascending = true);

    // Analytics
    double getTotalValue() const;
    std::vector<Product> getLowStockItems(int threshold = 5) const;
    std::vector<Product> getTopExpensive(int count = 3) const;

    // Access
    const std::vector<Product>& getAllProducts() const;
    Product* findById(int id);
    void setProducts(const std::vector<Product>& newProducts);
    void clear();
    size_t size() const;
};

#endif // WAREHOUSE_H