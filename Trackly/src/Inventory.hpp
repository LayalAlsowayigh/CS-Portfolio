#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "Item.hpp"
#include <string>
#include <vector>

struct Sale {
    int    item_id;
    int    qty;
    double revenue;
};

class Inventory {
public:
    bool load(const std::string& filename);
    bool save(const std::string& filename) const;

    void add(const Item& it);
    void list_all() const;
    void list_low_stock() const;

    const Item* find_by_id(int id) const;
    Item*       find_by_id_mut(int id);

    bool   record_sale(int id, int qty, Sale& out_sale);
    bool   append_sale_csv(const std::string& sales_file, const Sale& s) const;
    static double sum_sales_csv(const std::string& sales_file);

    double total_stock_value() const;
    double total_potential_revenue() const;

    void sort_by_name();
    void sort_by_stock();
    void search_by_name(const std::string& part) const;

    bool  restock(int id, int qty_to_add);
    bool  delete_by_id(int id);
    void  export_lowstock_csv(const std::string& filename) const;

    const std::vector<Item>& get_items() const { return items; }

private:
    std::vector<Item> items;
};

#endif
