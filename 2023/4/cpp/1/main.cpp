#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "file_parse.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);

    using pair_vec = std::vector<std::vector<size_t>>;
    const std::function<pair_vec(std::string)> fn = [](const std::string& line) {
        // remove everything before the first colon
        // remove all letters and pipe |
        const auto start = line.find(':');
        const auto s = line.substr(start + 1);

        auto out = pair_vec{};
        out.resize(2);
        auto buffer = std::string{};
        buffer.reserve(10);
        auto getter = 0;
        for (const char c : s) {
            if ((c == ' ' || c == '|') && ! buffer.empty()) {
                out[getter].push_back(std::stoll(buffer));
                buffer.clear();
            }
            if (c == '|')
                getter++;
            else if (c != ' ')
                buffer.push_back(c);
        }
        if (! buffer.empty()) {
            out[getter].push_back(std::stoll(buffer));
            buffer.clear();
        }
        return out;
    };
    const auto data = parse(file_handle, fn);

    const auto fn_acc = [](const size_t& acc, const std::vector<std::vector<size_t>>& vecs) {
        auto winners = vecs[0];
        auto selection = vecs[1];
        // sort each
        std::ranges::sort(winners);
        std::ranges::sort(selection);

        auto coincident = std::vector<size_t>{};
        coincident.reserve(5);
        std::ranges::set_intersection(winners, selection, std::back_inserter(coincident));
        if (coincident.empty())
            return acc;
        const auto points = static_cast<size_t>(pow(2, coincident.size() - size_t{ 1 }));
        return acc + points;
    };
    const size_t count = std::accumulate(data.begin(), data.end(), size_t{ 0 }, fn_acc);
    std::cout << count;
    //  13
    return 0;
}
