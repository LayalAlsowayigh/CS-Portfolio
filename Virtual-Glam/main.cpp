/* 
 * Virtual Glam by Layal
 * Author: Layal Alsowayigh
 * Description: small C++ console app to track makeup products.
 * Build: g++ -std=c++17 -Wall -Wextra main.cpp Item.cpp Catalog.cpp -o virtual_glam
 */

#include "Catalog.hpp"
#include <iostream>
#include <limits>

static void addItemInteractive(Catalog& cat) {
    std::string type, name, color, brand;
    double price = 0.0;

    std::cout << "type (Setting Spray/Primer/Moisturizer/Foundation/Setting Powder/Bronzer/Blush/Blush Stick/Blush Duo/Highlighter/Highlighter Palette/Palette/Liner): ";
    std::getline(std::cin, type);

    std::cout << "name: ";
    std::getline(std::cin, name);

    std::cout << "color (nude/rose/taupe/peach/clear/etc): ";
    std::getline(std::cin, color);

    std::cout << "price (e.g., 38.0): ";
    std::cin >> price;

    while (!std::cin || price < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "enter a valid non-negative price: ";
        std::cin >> price;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "brand ( Huda Beauty, Charlotte Tilbury): ";
    std::getline(std::cin, brand);

    cat.add(Item(type, name, color, price, brand));
    std::cout << "added! 💫\n";
}

int main() {
    Catalog cat;
    if (!cat.load("glam_box.txt")) {
        std::cout << "no file found, preloading Virtual Glam starter kit… ✨\n";
        cat.quickAddGlamStarter();
    }

    while (true) {
         std::cout << "\n=== Virtual Glam by Layal ✨💄 ===\n"
         << "1) show all\n"
         << "2) add item\n"
         << "3) sort by price\n"
         << "4) sort by name\n"
         << "5) search by brand\n"
         << "6) search by color\n"
         << "7) save\n"
         << "8) save & exit\n"
         << "9) search by type\n"
         << "10) stats\n"
         << "choice: ";

        int choice = 0;
        if (!(std::cin >> choice)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            cat.showAll();
        } else if (choice == 2) {
            addItemInteractive(cat);
        } else if (choice == 3) {
            cat.sortByPrice();
        } else if (choice == 4) {
            cat.sortByName();
        } else if (choice == 5) {
            std::string brand; std::cout << "brand: ";
            std::getline(std::cin, brand);
            cat.searchByBrand(brand);
        } else if (choice == 6) {
            std::string color; std::cout << "color: ";
            std::getline(std::cin, color);
            cat.searchByColor(color);
        } else if (choice == 7) {
            cat.save("glam_box.txt");
        } else if (choice == 8) {
            cat.save("glam_box.txt");
            std::cout << "bye! 💕\n";
            break;
        } else if (choice == 9) {
            std::string type; std::cout << "type (Foundation/Blush/Setting Powder): ";
            std::getline(std::cin, type);
            cat.searchByType(type);
        } else if (choice == 10) {
            cat.showStats();

        } else {
            std::cout << "invalid choice.\n";
        }
    }
    return 0;
}
