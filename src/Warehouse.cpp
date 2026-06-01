#define NOMINMAX
#include "Warehouse.h"
#include <algorithm>
#include <cctype>
#include <limits>
#include <string>
#include <Windows.h>

namespace {

// Корректный регистронезависимый lower-case для UTF-8 строк
// (std::tolower работает только с ASCII). Используем WinAPI:
// MultiByteToWideChar(CP_UTF8) -> CharLowerBuffW -> WideCharToMultiByte(CP_UTF8).
std::string utf8ToLower(const std::string& s) {
    if (s.empty()) return s;
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(),
                                      static_cast<int>(s.size()), nullptr, 0);
    if (wideLen <= 0) return s;
    std::wstring wide(static_cast<size_t>(wideLen), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()),
                        &wide[0], wideLen);
    CharLowerBuffW(&wide[0], wideLen);
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), wideLen,
                                      nullptr, 0, nullptr, nullptr);
    if (utf8Len <= 0) return s;
    std::string out(static_cast<size_t>(utf8Len), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), wideLen, &out[0], utf8Len,
                        nullptr, nullptr);
    return out;
}

}  // namespace

std::string Warehouse::toLower(const std::string& str) {
    return utf8ToLower(str);
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
    std::string lowerName = utf8ToLower(name);
    auto it = products.begin();
    while (it != products.end()) {
        if (utf8ToLower(it->getName()) == lowerName) {
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
    std::string lowerQuery = utf8ToLower(query);
    for (const auto& p : products) {
        if (utf8ToLower(p.getName()).find(lowerQuery) != std::string::npos) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::searchByCategory(const std::string& category) const {
    std::vector<Product> result;
    std::string lowerCategory = utf8ToLower(category);
    for (const auto& p : products) {
        if (utf8ToLower(p.getCategory()) == lowerCategory) {
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
