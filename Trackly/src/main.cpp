#include "Inventory.hpp"
#include <iostream>
#include <limits>
#include <string>

static int read_int(const std::string& prompt, int minv = -2147483647, int maxv = 2147483647) {
    int x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x && x >= minv && x <= maxv) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "enter a valid number.\n";
    }
}
static double read_double(const std::string& prompt, double minv = -1e-12, double maxv = 1e18) {
    double x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x && x >= minv && x <= maxv) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "enter a valid number.\n";
    }
}
static std::string read_line(const std::string& prompt) {
    std::string s; std::cout << prompt; std::getline(std::cin, s); return s;
}

int main() {
    Inventory inv;
    const std::string invfile   = "data/inventory.csv";
    const std::string salesfile = "data/sales.csv";
    const std::string lowfile   = "data/lowstock.csv";

    std::cout << "Trackly — Smart Inventory & Sales Tracker (FINAL v1.0)\n";

    if (!inv.load(invfile)) {
        std::cout << "no inventory file found, starting empty.\n";
    }

    while (true) {
        std::cout << "\n=== Trackly Menu ===\n"
                  << "1) list all\n"
                  << "2) add item\n"
                  << "3) save\n"
                  << "4) load\n"
                  << "5) record sale\n"
                  << "6) list low-stock\n"
                  << "7) report totals\n"
                  << "8) sort by name (A->Z)\n"
                  << "9) sort by stock (high->low)\n"
                  << "10) search by name\n"
                  << "11) restock item\n"
                  << "12) delete item\n"
                  << "13) export low-stock report\n"
                  << "14) exit\n"
                  << "choice: ";
        int ch;
        if (!(std::cin >> ch)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (ch == 1) {
            inv.list_all();
        } else if (ch == 2) {
            int id = read_int("id: ");
            std::string name = read_line("name: ");
            std::string cat  = read_line("category: ");
            int qty = read_int("qty: ", 0);
            double uc = read_double("unit cost: ", 0);
            double up = read_double("unit price: ", 0);
            int rl = read_int("restock level (alert at or below): ", 0);
            inv.add(Item(id, name, cat, qty, uc, up, rl));
            std::cout << "added.\n";
        } else if (ch == 3) {
            inv.save(invfile);
        } else if (ch == 4) {
            if (!inv.load(invfile)) std::cout << "could not load file.\n";
        } else if (ch == 5) {
            int id = read_int("item id: ");
            int q  = read_int("qty sold: ", 1);
            Sale s;
            if (inv.record_sale(id, q, s)) {
                inv.append_sale_csv(salesfile, s);
                std::cout << "sale recorded: revenue $" << s.revenue << "\n";
                const Item* it = inv.find_by_id(id);
                if (it && it->qty <= it->restock_level) {
                    std::cout << "ALERT: low stock for \"" << it->name << "\" (qty " << it->qty << ")\n";
                }
            } else {
                std::cout << "could not record sale (bad id or qty too high).\n";
            }
        } else if (ch == 6) {
            inv.list_low_stock();
        } else if (ch == 7) {
            double value = inv.total_stock_value();
            double pot   = inv.total_potential_revenue();
            double sold  = Inventory::sum_sales_csv(salesfile);
            std::cout << "inventory value (at cost): $" << value << "\n";
            std::cout << "potential revenue (unsold): $" << pot << "\n";
            std::cout << "revenue to date (sales.csv): $" << sold << "\n";
        } else if (ch == 8) {
            inv.sort_by_name();
        } else if (ch == 9) {
            inv.sort_by_stock();
        } else if (ch == 10) {
            std::string part = read_line("name contains: ");
            inv.search_by_name(part);
        } else if (ch == 11) {
            int id = read_int("item id: ");
            int q  = read_int("qty to add: ", 1);
            if (inv.restock(id, q)) std::cout << "restocked.\n";
            else std::cout << "item not found or bad qty.\n";
        } else if (ch == 12) {
            int id = read_int("item id to delete: ");
            if (inv.delete_by_id(id)) std::cout << "item deleted.\n";
            else std::cout << "item not found.\n";
        } else if (ch == 13) {
            inv.export_lowstock_csv(lowfile);
            std::cout << "low-stock exported to " << lowfile << "\n";
        } else if (ch == 14) {
            std::cout << "bye!\n";
            break;
        } else {
            std::cout << "invalid choice.\n";
        }
    }
    return 0;
}
