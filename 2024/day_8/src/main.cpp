#include <string>
#include <unordered_map>
#include <vector>
#include "file_parse.h"
#include <iostream>
#include <filesystem>
#include <numeric>
#include <cassert>
#include <compare>

class Coord {
public:
    size_t x;
    size_t y;

    Coord(const size_t x, const size_t y): x(x), y(y)
    {}
};
Coord operator-(const Coord& lhs, const Coord& rhs) {
    assert(lhs.x >= rhs.x && lhs.y >= rhs.y);
    return { lhs.x - rhs.x, lhs.y - rhs.y };
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return { lhs.x + rhs.x, lhs.x + rhs.x };
};
std::strong_ordering operator<=>(const Coord& lhs, const Coord& rhs) {
    if (lhs.x < rhs.x or (lhs.x == rhs.x and lhs.y < rhs.y)) {
        return std::strong_ordering::less;
    }
    if (lhs.x > rhs.x or (lhs.x == rhs.x and lhs.y > rhs.y)) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equivalent;
};
using coordList = std::vector<Coord>;
using letterDict = std::unordered_map<char, coordList>;

std::pair<letterDict, Coord> parse(fileParse::FileHandle&& file_handle) {
    auto out = letterDict{};
    auto y = size_t{ 0 };
    auto x = size_t{ 0 };
    const std::string matcher = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    
    while (! file_handle.eof()) {
        auto line = file_handle.getLine();
        x = 0;
        for (const auto c : line) {
            if (matcher.find(c) == std::string::npos) {
                continue;
            }

            if (out.find(c) != out.end()) {
                // element exits
                out[c].emplace_back(x, y);
            } else {
                // element does not exist
                out.insert({ c, { { x, y } } });
            }
            x++;
        }
        y++;
    }
    x--;
    y--;
    return {out, {x,y}};
}

/// <summary>
/// find all intersections in the field
/// </summary>
/// <param name="lhs"> first coordinate</param>
/// <param name="rhs"> second coordinate</param>
/// <returns>a vec of all intersections in the field</returns>
coordList findIntersections(const Coord& lhs, const Coord& rhs, const Coord& dimensions) {
    auto out = coordList{};
    const auto delta = lhs - rhs;
    // first moving left
    auto current = lhs;
    while (current>delta) {
        current = current - delta;
        if (current.y < dimensions.y) {
            out.emplace_back(current);
        }
    }
    // next move right
    current = rhs;
    while (current<dimensions) {
        current = current + delta;
        if (current.y < dimensions.y) {
            out.emplace_back(current);
        }
    }
    return out;
}

/// determine the locations of each letter/symbol
/// remove any letters with only 1 occurance
/// create the equations of each letter locations
/// find all intersections
/// get unique of intersections
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    const std::filesystem::path p = argv[1];
    auto [locations, dimensions] = parse(fileParse::FileHandle(p));

    // f no longer exits
    
    // 5905
    std::cout << "final=" << 0 << " \n";
}
