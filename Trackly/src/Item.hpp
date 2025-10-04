#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <iostream>

struct Item {
    int         id;
    std::string name;
    std::string category;
    int         qty;
    double      unit_cost;
    double      unit_price;
    int         restock_level;

    Item();
    Item(int i, const std::string& n, const std::string& c,
         int q, double uc, double up, int rl);

    double stock_value() const;
    double potential_rev() const;

    std::string toCSV() const;
    static bool fromCSV(const std::string& line, Item& out);

    void print_row() const;
};

#endif