/* 
 * Virtual Glam by Layal
 * Author: Layal Alsowayigh
 */

#include "Item.hpp"
#include <sstream>

std::string Item::toCSV() const {
    // type,name,color,price,brand
    std::ostringstream oss;
    oss << type << "," << name << "," << color << "," << price << "," << brand;
    return oss.str();
}

bool Item::fromCSVLine(const std::string& line, Item& out) {
    std::stringstream ss(line);
    std::string t, n, c, p, b;

    if (!std::getline(ss, t, ',')) return false;
    if (!std::getline(ss, n, ',')) return false;
    if (!std::getline(ss, c, ',')) return false;
    if (!std::getline(ss, p, ',')) return false;
    if (!std::getline(ss, b))      return false;

    try {
        out = Item(t, n, c, std::stod(p), b);
        return true;
    } catch (...) { 
        return false; 
    }
}

