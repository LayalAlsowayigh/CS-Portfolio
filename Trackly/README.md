# Trackly — Smart Inventory & Sales Tracker

Trackly is a C++17 console application that manages inventory, sales, and reports.
It supports:
- Adding/editing/deleting items
- Saving & loading from CSV files
- Recording sales (with revenue logging)
- Low-stock alerts + export to CSV
- Sorting, searching, and reports

### Turkish Food Demo
For testing, Trackly includes a seeded Turkish food menu 🍢🥙🍵:
- Adana Kebab, Iskender, Doner Wraps, Lahmacun, Baklava,etc.

### Build
```bash
g++ -std=c++17 -Wall -Wextra src/main.cpp src/Item.cpp src/Inventory.cpp -o trackly
./trackly