#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "file_parse.h"
struct color
{
    size_t red;
    size_t green;
    size_t blue;

    color(size_t r, size_t g, size_t b)
        : red(r), green(g), blue(b)
    {}
};

constexpr auto bag = color(12,13,14);

struct game
{
    size_t id;
    std::vector<color> games;

    game(std::string line) {
        
    }
};

int main() {
    
    return 0;
}
