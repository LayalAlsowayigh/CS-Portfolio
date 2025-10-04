/* 
 * Virtual Glam by Layal
 * Author: Layal Alsowayigh
 */

#include "Catalog.hpp"
#include <iostream>
#include <fstream>
#include <utility>
#include <map>

void Catalog::add(const Item& it) { 
    items.push_back(it); 
}

void Catalog::showAll() const {
    if (items.empty()) { 
        std::cout << "your glam box is empty ✨\n"; 
        return; 
    }
    std::cout << "\n=== Virtual Glam by Layal ✨💄 (" << items.size() << " items) ===\n";
    for (const auto& it : items) it.display();
}

void Catalog::save(const std::string& filename) const {
    std::ofstream out(filename);
    for (const auto& it : items) out << it.toCSV() << "\n";
    std::cout << "saved to " << filename << " 💾\n";
}

bool Catalog::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    items.clear();
    std::string line;
    Item it;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (Item::fromCSVLine(line, it)) items.push_back(it);
    }
    std::cout << "loaded " << items.size() << " items from " << filename << " 📂\n";
    return true;
}

void Catalog::sortByPrice() {
    for (size_t i = 0; i + 1 < items.size(); ++i) {
        size_t minIdx = i;
        for (size_t j = i + 1; j < items.size(); ++j) {
            if (items[j].price < items[minIdx].price) minIdx = j;
        }
        if (minIdx != i) std::swap(items[i], items[minIdx]);
    }
    std::cout << "sorted by price ↑ 💲\n";
}

void Catalog::sortByName() {
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
    std::cout << "sorted by name A→Z 🔤\n";
}

void Catalog::searchByBrand(const std::string& brand) const {
    int hits = 0;
    for (const auto& it : items)
        if (it.brand == brand) { it.display(); ++hits; }
    if (!hits) std::cout << "no items for brand: " << brand << "\n";
}

void Catalog::searchByColor(const std::string& color) const {
    int hits = 0;
    for (const auto& it : items)
        if (it.color == color) { it.display(); ++hits; }
    if (!hits) std::cout << "no items for color: " << color << "\n";
}

void Catalog::quickAddGlamStarter() {
    // skincare first
    add(Item("Moisturizer","Charlotte’s Magic Cream","none",66.0,"Charlotte Tilbury"));
    add(Item("Primer","REFY Face Primer","clear",28.0,"REFY"));

    // foundation 
    add(Item("Foundation","Triclone Skin Tech Foundation","Light 160 Cool",49.0,"Haus Labs"));
    add(Item("Foundation","Triclone Skin Tech Foundation","Medium 250 Neutral",49.0,"Haus Labs"));
    add(Item("Foundation","Triclone Skin Tech Foundation","Deep 460 Warm",49.0,"Haus Labs"));

    // powders
    add(Item("Setting Powder","Easy Bake Loose Powder - Pound Cake","light neutral",38.0,"Huda Beauty"));
    add(Item("Setting Powder","Easy Bake Loose Powder - Cherry Blossom","bright pink",38.0,"Huda Beauty"));
    add(Item("Setting Powder","Easy Bake Loose Powder - Cinnamon Bun","tan neutral",38.0,"Huda Beauty"));

    // sculpt
    add(Item("Bronzer Stick","SoftSculpt Shaping Stick","Light",32.0,"Makeup by Mario"));
    add(Item("Bronzer","Nu Bronzer (Le Nu Glow Powder Bronzer)","warm bronze",55.0,"YSL"));

    // color
    add(Item("Blush Stick","Soft Pop Blush Stick","Peachy",30.0,"Makeup by Mario"));
    add(Item("Blush Duo","Major Headlines Double-Take Crème & Powder Blush Duo","She's Vibrant (rose)",38.0,"Patrick Ta"));
    add(Item("Blush","Nu Blush (Le Nu Glow Powder Blush)","rosy nude",45.0,"YSL"));

    // highlight
    add(Item("Highlighter Palette","Backstage Glow Face Palette - 001 Universal","universal",48.0,"Dior"));

    // lock it with spray
    add(Item("Setting Spray","Airbrush Flawless Setting Spray","clear",38.0,"Charlotte Tilbury"));
}

// show only items of a specific type, e.g., "Foundation" or "Blush"
void Catalog::searchByType(const std::string& type) const {
    int hits = 0;
    for (const auto& it : items) {
        if (it.type == type) { it.display(); ++hits; }
    }
    if (!hits) std::cout << "no items for type: " << type << "\n";
}

void Catalog::showStats() const {
    if (items.empty()) {
        std::cout << "no items to analyze.\n";
        return;
    }

    std::map<std::string, std::pair<int, double>> perType;
    double totalSum = 0.0;
    int totalCount = 0;

    const Item* cheapest = &items[0];
    const Item* priciest = &items[0];

    for (const auto& it : items) {
        auto& entry = perType[it.type];
        entry.first  += 1;
        entry.second += it.price;
        totalSum     += it.price;
        totalCount   += 1;

        if (it.price < cheapest->price) cheapest = &it;
        if (it.price > priciest->price) priciest = &it;
    }

    std::cout << "\n=== stats ===\n";
    std::cout << "total items: " << totalCount << "\n";
    std::cout << "overall average price: $" << (totalSum / totalCount) << "\n\n";

    std::cout << "per type (count, avg price):\n";
    for (const auto& kv : perType) {
        const std::string& t = kv.first;
        int count = kv.second.first;
        double sum = kv.second.second;
        std::cout << " - " << t << ": " << count
                  << " item(s), avg $" << (sum / count) << "\n";
    }

    std::cout << "\ncheapest: ";
    cheapest->display();
    std::cout << "priciest: ";
    priciest->display();
    std::cout << "\n";
}
