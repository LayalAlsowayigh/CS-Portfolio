**Virtual Glam by Layal ✨💄**

A C++ console app that manages a makeup collection.  
I built this to practice **C++ OOP, sorting, and file I/O** in a fun way by connecting it to beauty products I love.

.....

**Features**
- Add items (type, name, shade, price, brand)
- Show all items with emojis 💄
- Sort by price (selection sort) or by name (bubble sort)
- Search by brand, color, or type
- Save & load from `glam_box.txt`
- Stats screen: counts, averages, cheapest, priciest

.....

**Build & Run**
g++ -std=c++17 -Wall -Wextra main.cpp Item.cpp Catalog.cpp -o virtual_glam
./virtual_glam
