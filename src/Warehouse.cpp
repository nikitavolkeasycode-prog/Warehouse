#include "Warehouse.h"
#include <algorithm>
#include <cctype>
#include <limits>

std::string Warehouse::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool Warehouse::isValidPrice(double price) {
    return price >= 0.0;
}

bool Warehouse::isValidQuantity(int quantity) {
    return quantity >= 0;
}

bool Warehouse::isValidString(const std::string& str) {
    return !str.empty();
}

bool Warehouse::isIdUnique(int id) const {
    for (const auto& p : products) {
        if (p.getId() == id) return false;
    }
    return true;
}

bool Warehouse::addProduct(const Product& product) {
    if (!isValidString(product.getName()) ||
        !isValidString(product.getCategory()) ||
        !isValidPrice(product.getPrice()) ||
        !isValidQuantity(product.getQuantity()) ||
        !isIdUnique(product.getId())) {
        return false;
    }
    products.push_back(product);
    return true;
}

bool Warehouse::editProduct(int id, const Product& updatedProduct) {
    for (auto& p : products) {
        if (p.getId() == id) {
            if (updatedProduct.getId() != id && !isIdUnique(updatedProduct.getId())) {
                return false; // Cannot change to an already existing ID
            }
            if (!isValidString(updatedProduct.getName()) ||
                !isValidString(updatedProduct.getCategory()) ||
                !isValidPrice(updatedProduct.getPrice()) ||
                !isValidQuantity(updatedProduct.getQuantity())) {
                return false;
            }
            p = updatedProduct;
            return true;
        }
    }
    return false;
}

bool Warehouse::removeById(int id) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getId() == id) {
            products.erase(it);
            return true;
        }
    }
    return false;
}

bool Warehouse::removeByName(const std::string& name) {
    bool found = false;
    auto it = products.begin();
    while (it != products.end()) {
        if (toLower(it->getName()) == toLower(name)) {
            it = products.erase(it);
            found = true;
        } else {
            ++it;
        }
    }
    return found;
}

std::vector<Product> Warehouse::searchByName(const std::string& query) const {
    std::vector<Product> result;
    std::string lowerQuery = toLower(query);
    for (const auto& p : products) {
        if (toLower(p.getName()).find(lowerQuery) != std::string::npos) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::searchByCategory(const std::string& category) const {
    std::vector<Product> result;
    std::string lowerCategory = toLower(category);
    for (const auto& p : products) {
        if (toLower(p.getCategory()) == lowerCategory) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::searchByPriceRange(double minPrice, double maxPrice) const {
    std::vector<Product> result;
    for (const auto& p : products) {
        if (p.getPrice() >= minPrice && p.getPrice() <= maxPrice) {
            result.push_back(p);
        }
    }
    return result;
}

void Warehouse::sortByPrice(bool ascending) {
    std::sort(products.begin(), products.end(),
              [ascending](const Product& a, const Product& b) {
                  return ascending ? a.getPrice() < b.getPrice()
                                   : a.getPrice() > b.getPrice();
              });
}

void Warehouse::sortByQuantity(bool ascending) {
    std::sort(products.begin(), products.end(),
              [ascending](const Product& a, const Product& b) {
                  return ascending ? a.getQuantity() < b.getQuantity()
                                   : a.getQuantity() > b.getQuantity();
              });
}

void Warehouse::sortByName(bool ascending) {
    std::sort(products.begin(), products.end(),
              [ascending](const Product& a, const Product& b) {
                  return ascending ? a.getName() < b.getName()
                                   : a.getName() > b.getName();
              });
}

double Warehouse::getTotalValue() const {
    double total = 0.0;
    for (const auto& p : products) {
        total += p.getPrice() * p.getQuantity();
    }
    return total;
}

std::vector<Product> Warehouse::getLowStockItems(int threshold) const {
    std::vector<Product> result;
    for (const auto& p : products) {
        if (p.getQuantity() <= threshold) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::getTopExpensive(int count) const {
    std::vector<Product> sorted = products;
    std::sort(sorted.begin(), sorted.end(),
              [](const Product& a, const Product& b) {
                  return a.getPrice() > b.getPrice();
              });
    if (sorted.size() > static_cast<size_t>(count)) {
        sorted.resize(count);
    }
    return sorted;
}

const std::vector<Product>& Warehouse::getAllProducts() const {
    return products;
}

Product* Warehouse::findById(int id) {
    for (auto& p : products) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

void Warehouse::setProducts(const std::vector<Product>& newProducts) {
    products = newProducts;
}

void Warehouse::clear() {
    products.clear();
}

size_t Warehouse::size() const {
    return products.size();
}