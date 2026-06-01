#include "Menu.h"
#include <cstdlib>

int main() {
    const std::string DATA_FILE = "data/products.txt";

    Menu menu(DATA_FILE);
    menu.run();

    return EXIT_SUCCESS;
}