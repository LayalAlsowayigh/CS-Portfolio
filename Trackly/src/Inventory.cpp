#include "Inventory.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

bool Inventory::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    items.clear();
    std::string line;
    Item it;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (Item::fromCSV(line, it)) items.push_back(it);
    }
    std::cout << "loaded " << items.size() << " items from " << filename << "\n";
    return true;
}

bool Inventory::save(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return false;
    for (const auto& it : items) out << it.toCSV() << "\n";
    std::cout << "saved " << items.size() << " items to " << filename << "\n";
    return true;
}

void Inventory::add(const Item& it) { items.push_back(it); }

void Inventory::list_all() const {
    if (items.empty()) { std::cout << "no items.\n"; return; }
    for (const auto& it : items) it.print_row();
}

void Inventory::list_low_stock() const {
    int hits = 0;
    for (const auto& it : items) {
        if (it.qty <= it.restock_level) { it.print_row(); ++hits; }
    }
    if (!hits) std::cout << "no low-stock items.\n";
}

const Item* Inventory::find_by_id(int id) const {
    for (const auto& it : items) if (it.id == id) return &it;
    return nullptr;
}
Item* Inventory::find_by_id_mut(int id) {
    for (auto& it : items) if (it.id == id) return &it;
    return nullptr;
}

bool Inventory::record_sale(int id, int q, Sale& out_sale) {
    Item* it = find_by_id_mut(id);
    if (!it) return false;
    if (q <= 0 || q > it->qty) return false;
    it->qty -= q;
    out_sale.item_id = id;
    out_sale.qty = q;
    out_sale.revenue = q * it->unit_price;
    return true;
}

bool Inventory::append_sale_csv(const std::string& sales_file, const Sale& s) const {
    std::ofstream out(sales_file, std::ios::app);
    if (!out) return false;
    out << s.item_id << "," << s.qty << "," << s.revenue << "\n";
    return true;
}

double Inventory::sum_sales_csv(const std::string& sales_file) {
    std::ifstream in(sales_file);
    if (!in) return 0.0;
    double total = 0.0;
    std::string line, sid, sq, srevenue;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        if (!std::getline(ss, sid, ',')) continue;
        if (!std::getline(ss, sq, ',')) continue;
        if (!std::getline(ss, srevenue)) continue;
        try { total += std::stod(srevenue); } catch (...) {}
    }
    return total;
}

double Inventory::total_stock_value() const {
    double s = 0.0;
    for (const auto& it : items) s += it.stock_value();
    return s;
}
double Inventory::total_potential_revenue() const {
    double s = 0.0;
    for (const auto& it : items) s += it.potential_rev();
    return s;
}

void Inventory::sort_by_name() {
    if (items.empty()) return;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (size_t i = 0; i + 1 < items.size(); ++i) {
            if (items[i].name > items[i+1].name) {
                std::swap(items[i], items[i+1]);
                swapped = true;
            }
        }
    }
    std::cout << "sorted by name (A->Z)\n";
}

void Inventory::sort_by_stock() {
    for (size_t i = 0; i + 1 < items.size(); ++i) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < items.size(); ++j) {
            if (items[j].qty > items[maxIdx].qty) maxIdx = j;
        }
        if (maxIdx != i) std::swap(items[i], items[maxIdx]);
    }
    std::cout << "sorted by stock (high->low)\n";
}

void Inventory::search_by_name(const std::string& part) const {
    if (part.empty()) { std::cout << "empty search.\n"; return; }
    std::string needle = part; 
    std::transform(needle.begin(), needle.end(), needle.begin(), ::tolower);
    int hits = 0;
    for (const auto& it : items) {
        std::string hay = it.name;
        std::transform(hay.begin(), hay.end(), hay.begin(), ::tolower);
        if (hay.find(needle) != std::string::npos) { it.print_row(); ++hits; }
    }
    if (!hits) std::cout << "no matches for: " << part << "\n";
}

bool Inventory::restock(int id, int qty_to_add) {
    if (qty_to_add <= 0) return false;
    Item* it = find_by_id_mut(id);
    if (!it) return false;
    it->qty += qty_to_add;
    return true;
}

bool Inventory::delete_by_id(int id) {
    auto before = items.size();
    items.erase(std::remove_if(items.begin(), items.end(),
                               [&](const Item& x){ return x.id == id; }),
                items.end());
    return items.size() != before;
}

void Inventory::export_lowstock_csv(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) { std::cout << "could not export file\n"; return; }
    for (const auto& it : items) {
        if (it.qty <= it.restock_level) out << it.toCSV() << "\n";
    }
}
