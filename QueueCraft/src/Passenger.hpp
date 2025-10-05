#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <string>

enum class Group { Elderly=0, Family=1, Regular=2 };

struct Passenger {
    int id;
    Group group;
    int arrival_min;
    int board_min;
    Passenger(int i, Group g, int a) : id(i), group(g), arrival_min(a), board_min(-1) {}
};

inline std::string group_to_str(Group g) {
    if (g == Group::Elderly) return "elderly";
    if (g == Group::Family)  return "family";
    return "regular";
}

#endif
