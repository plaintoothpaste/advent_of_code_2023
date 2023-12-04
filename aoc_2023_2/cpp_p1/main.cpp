#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "file_parse.h"

enum color { red, green, blue };
using color_store = std::array<size_t, 3>;
constexpr auto bag = color_store{ 12,13,14 };

std::pair<color, size_t> parseSingle(const std::string& token) {

}

struct game
{
    size_t id;
    std::vector<color_store> games;

    game(const std::string line) {
        // Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue

        // id
        const auto prefix_n = size_t{ 5 }; //"Game "
        const auto delimiter = std::string{ ":" };
        id = std::stoi(line.substr(prefix_n, line.find(delimiter)));

        // games
        const auto main_section = line.substr(line.find(delimiter), line.size());
        games.emplace_back(color_store{0, 0, 0});
        auto index = size_t{ 0 };
        while (index < main_section.size()) {
            const auto next_commer = main_section.find(',', index);
            const auto next_semicolon = main_section.find(';', index);

            if (next_commer == next_semicolon && next_commer == main_section.size()) {
                index = main_section.size();
                continue;
            }

            const auto [c, val] = parseSingle(main_section.substr(index, next_commer));
            games.back()[c] = val;
            index = next_commer;
            if (next_semicolon < next_commer) {
                games.emplace_back(color_store{0, 0, 0});
                index = next_semicolon;
            }
        }

    }
};

int main() {

    return 0;
}
