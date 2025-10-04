/* 
 * Virtual Glam by Layal
 * Author: Layal Alsowayigh
 */

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <iostream>

static const bool kCuteMode = true;

struct Item {
    std::string type;   // Setting Spray, Primer, Foundation, Blush
    std::string name;   // Airbrush Flawless Setting Spray
    std::string color;  // nude,neutral medium,clear
    double      price;  // 38.0
    std::string brand;  // Charlotte Tilbury

    Item() : price(0.0) {}
    Item(const std::string& t, const std::string& n,
         const std::string& c, double p, const std::string& b)
        : type(t), name(n), color(c), price(p), brand(b) {}

    std::string emoji() const {
        if (type == "Setting Spray") return "💦";
        if (type == "Primer")        return "💧";
        if (type == "Moisturizer")   return "🧴";
        if (type == "Foundation")    return "🧪";
        if (type == "Setting Powder")return "🫧";
        if (type == "Bronzer" || type == "Bronzer Stick") return "🌞";
        if (type == "Blush" || type == "Blush Stick" || type == "Blush Duo") return "🌸";
        if (type == "Highlighter" || type == "Highlighter Palette") return "✨";
        if (type == "Palette")       return "🎨";
        if (type == "Liner")         return "✏️";
        return "💄";
    }

    void display() const {
        if (kCuteMode) {
            std::cout << emoji() << " " << name
                      << " | " << type
                      << " | " << color
                      << " | $" << price
                      << " | " << brand << "\n";
        } else {
            std::cout << name
                      << " | " << type
                      << " | " << color
                      << " | $" << price
                      << " | " << brand << "\n";
        }
    }

    std::string toCSV() const;
    static bool fromCSVLine(const std::string& line, Item& out);
};

#endif
