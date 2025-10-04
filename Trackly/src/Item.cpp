#include "Item.hpp"
#include <sstream>

Item::Item()
: id(0), qty(0), unit_cost(0.0), unit_price(0.0), restock_level(0) {}

Item::Item(int i, const std::string& n, const std::string& c,
int q, double uc, double up, int rl)
: id(i), name(n), category(c), qty(q), unit_cost(uc), unit_price(up), restock_level(rl) {}

double Item::stock_value() const { return qty * unit_cost; }
double Item::potential_rev() const { return qty * unit_price; }

std::string Item::toCSV() const {
    std::ostringstream oss;
    oss << id << "," << name << "," << category << ","
    << qty << "," << unit_cost << "," << unit_price << "," << restock_level;
    return oss.str();
}

bool Item::fromCSV(const std::string& line, Item& out) {
    std::stringstream ss(line);
    std::string sid, name, category, sqty, suc, sup, srl;

if (!std::getline(ss, sid, ',')) return false;
if (!std::getline(ss, name, ',')) return false;
if (!std::getline(ss, category, ',')) return false;
if (!std::getline(ss, sqty, ',')) return false;
if (!std::getline(ss, suc, ',')) return false;
if (!std::getline(ss, sup, ',')) return false;
if (!std::getline(ss, srl)) return false;

try {
    out.id = std::stoi(sid);
    out.name = name;
    out.category = category;
    out.qty = std::stoi(sqty);
    out.unit_cost = std::stod(suc);
    out.unit_price = std::stod(sup);
    out.restock_level = std::stoi(srl);
    return true;
    } catch (...) { return false; }
}

void Item::print_row() const {
    std::cout << "ID:" << id
    << " | " << name
    << " | " << category
    << " | qty:" << qty
    << " | cost:$" << unit_cost
    << " | price:$" << unit_price
    << " | restock@" << restock_level;
    if (qty <= restock_level) std::cout << "  <-- LOW STOCK";
    std::cout << "\n";
}