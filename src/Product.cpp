#include "Product.h"
#include <sstream>
#include <iomanip>

Product::Product() : id(0), price(0.0), quantity(0) {}

Product::Product(int id, const std::string& name, const std::string& category,
                 double price, int quantity)
    : id(id), name(name), category(category), price(price), quantity(quantity) {}

int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
std::string Product::getCategory() const { return category; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }

void Product::setId(int id) { this->id = id; }
void Product::setName(const std::string& name) { this->name = name; }
void Product::setCategory(const std::string& category) { this->category = category; }
void Product::setPrice(double price) { this->price = price; }
void Product::setQuantity(int quantity) { this->quantity = quantity; }

std::string Product::toFileString() const {
    std::ostringstream oss;
    oss << id << ","
        << name << ","
        << category << ","
        << std::fixed << std::setprecision(2) << price << ","
        << quantity;
    return oss.str();
}

Product Product::fromFileString(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    Product p;

    // id
    std::getline(iss, token, ',');
    p.id = std::stoi(token);

    // name
    std::getline(iss, token, ',');
    p.name = token;

    // category
    std::getline(iss, token, ',');
    p.category = token;

    // price
    std::getline(iss, token, ',');
    p.price = std::stod(token);

    // quantity
    std::getline(iss, token, ',');
    p.quantity = std::stoi(token);

    return p;
}

std::ostream& operator<<(std::ostream& os, const Product& p) {
    os << "ID: " << p.id
       << " | Название: " << p.name
       << " | Категория: " << p.category
       << " | Цена: " << std::fixed << std::setprecision(2) << p.price
       << " | Количество: " << p.quantity;
    return os;
}