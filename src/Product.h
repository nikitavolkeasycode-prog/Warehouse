#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <ostream>

class Product {
private:
    int id;
    std::string name;
    std::string category;
    double price;
    int quantity;

public:
    Product();
    Product(int id, const std::string& name, const std::string& category,
            double price, int quantity);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getCategory() const;
    double getPrice() const;
    int getQuantity() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setCategory(const std::string& category);
    void setPrice(double price);
    void setQuantity(int quantity);

    // Serialization
    std::string toFileString() const;
    static Product fromFileString(const std::string& line);

    // Display
    friend std::ostream& operator<<(std::ostream& os, const Product& p);
};

#endif // PRODUCT_H