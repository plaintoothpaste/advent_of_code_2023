#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "enumerate.h"
#include "file_parse.h"

// given space delimited string return the vector
std::vector<size_t> lineToSizetVec(const std::string& str) {
    auto buffer = std::string{};
    buffer.reserve(10);
    auto out = std::vector<size_t>{};
    for (const char c : str) {
        if (c == ' ' && ! buffer.empty()) {
            out.push_back(std::stoll(buffer));
            buffer.clear();
        } else if (c != ' ')
            buffer.push_back(c);
    }
    if (! buffer.empty()) {
        out.push_back(std::stoll(buffer));
        buffer.clear();
    }
    return out;
}

struct card {
    using vec_st = std::vector<size_t>;
    vec_st winners;
    vec_st selection;

    card(const std::string& line) {
        // remove everything before the first colon
        // remove all letters and pipe |
        const auto start = line.find(':');
        const auto s = line.substr(start + 1);
        const auto split = s.find('|');

        winners = lineToSizetVec(s.substr(0, split));
        selection = lineToSizetVec(s.substr(split + 1));
    }

    size_t num_wins() {
        std::ranges::sort(winners);
        std::ranges::sort(selection);
        auto out = vec_st{};
        std::ranges::set_intersection(winners, selection, std::back_inserter(out));
        return out.size();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<size_t(std::string)> fn = [](const std::string& line) { return card(line).num_wins(); };
    const auto data = parse(file_handle, fn);
    auto multiplier = std::vector<size_t>{};
    multiplier.resize(data.size(), 1);
    size_t total = 0;

    for (const auto [index, wins] : enumerate(data)) {
        total += multiplier[index];

        for (auto i = index + 1; i <= index + wins; ++i) {
            if (i > data.size())
                break;
            multiplier[i] += multiplier[index];
        }
    }
    std::cout << total;
    //  30
    return 0;
}
