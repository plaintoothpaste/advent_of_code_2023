#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

#include "file_parse.h"

enum class color { red, green, blue };
using color_store = std::array<size_t, 3>;

std::optional<std::string> removeWord(const std::string& token, const std::string& word) {
    const auto limit = token.find(word);
    if (limit < token.size()) {
        return { token.substr(0, limit) };
    }
    return {};
}

std::pair<color, size_t> parseSingle(const std::string& token) {
    const auto limit = token.find_first_of(",;");
    const auto sub = token.substr(0, limit);

    if (const auto trimmed = removeWord(sub, "red")) {
        return { color::red, std::stoll(trimmed.value()) };
    }
    if (const auto trimmed = removeWord(sub, "blue")) {
        return { color::blue, std::stoll(trimmed.value()) };
    }
    if (const auto trimmed = removeWord(sub, "green")) {
        return { color::green, std::stoll(trimmed.value()) };
    }
    const auto msg = "could not process the sub token:" + token;
    throw std::exception(msg.c_str());
}

struct game {
    size_t id;
    std::vector<color_store> games;

    game(const std::string line) {
        // Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue

        // id
        const auto prefix_n = size_t{ 5 }; //"Game "
        const auto delimiter = std::string{ ":" };
        id = std::stoi(line.substr(prefix_n, line.find(delimiter)));

        // games
        const auto main_section = line.substr(line.find(delimiter) + 1, line.size());
        games.emplace_back(color_store{ 0, 0, 0 });
        auto index = size_t{ 0 };
        while (true) {
            const auto next_commer = main_section.find(',', index);
            const auto next_semicolon = main_section.find(';', index);
            const auto next = next_semicolon < next_commer ? next_semicolon : next_commer;
            const auto sub = main_section.substr(index, next - index);

            const auto [c, val] = parseSingle(sub);
            games.back()[static_cast<size_t>(c)] = val;

            if (next_commer == next_semicolon) {
                break;
            }
            index = next + 1;

            if (next_semicolon < next_commer) {
                games.emplace_back(color_store{ 0, 0, 0 });
            }
        }
    }
    color_store minimal() const {
        auto min_bag = color_store{};
        for (const auto& g : games) {
            min_bag[static_cast<size_t>(color::red)] = std::max(min_bag[static_cast<size_t>(color::red)],g[static_cast<size_t>(color::red)]);
            min_bag[static_cast<size_t>(color::green)] = std::max(min_bag[static_cast<size_t>(color::green)],g[static_cast<size_t>(color::green)]);
            min_bag[static_cast<size_t>(color::blue)] = std::max(min_bag[static_cast<size_t>(color::blue)],g[static_cast<size_t>(color::blue)]);
        }
        return min_bag;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<game(std::string)> fn = [](const std::string& line) { return game(line); };
    const auto data = fileParse::parse(file_handle, fn);
    const auto result = std::accumulate(data.begin(), data.end(),size_t{0},
        [](const size_t sum, const game &g) {
            const color_store minimal = g.minimal();
            return sum + minimal[0]*minimal[1]*minimal[2];
        });

    std::cout << result;
    return 0;
}
