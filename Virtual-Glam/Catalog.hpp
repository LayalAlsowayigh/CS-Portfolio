/* 
 * Virtual Glam by Layal
 * Author: Layal Alsowayigh
 */

#ifndef CATALOG_HPP
#define CATALOG_HPP

#include "Item.hpp"
#include <vector>
#include <string>

class Catalog {
public:
    void add(const Item& it);
    void showAll() const;
    void save(const std::string& filename) const;
    bool load(const std::string& filename);
    void searchByType(const std::string& type) const;
    void showStats() const;

    void sortByPrice();
    void sortByName();

    void searchByBrand(const std::string& brand) const;
    void searchByColor(const std::string& color) const;

    void quickAddGlamStarter();

private:
    std::vector<Item> items;
};

#endif

