#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "enumerate.h"
#include "file_parse.h"

int main() {
    const std::function<std::vector<size_t>(std::string)> fn = [](const std::string& line) {
        // remove everything before the first colon
        // remove all letters and pipe |
        const auto start = line.find(':');
        const auto s = line.substr(start + 1);
        const auto pipe = s.find('|');
        const auto clean_str = s.substr(0, pipe) + s.substr(pipe + 1);

        auto out = std::vector<size_t>{};
        out.reserve(13);
        auto buffer = std::string{};
        buffer.reserve(10);

        for (const char c : clean_str) {
            if (c == ' ' && ! buffer.empty()) {
                out.push_back(std::stoll(buffer));
                buffer.clear();
            }
            if (c != ' ')
                buffer.push_back(c);
        }
        if (! buffer.empty()) {
            out.push_back(std::stoll(buffer));
            buffer.clear();
        }
        return out;
    };
    const auto data = parse("example.txt", fn);

    const auto fn_acc = [](const size_t& acc, std::vector<size_t> vec) {
        // sort each section
        std::sort(vec.begin(), vec.begin() + 5);
        std::sort(vec.begin() + 5, vec.end());

        auto coincident = std::vector<size_t>{};
        coincident.reserve(5);
        std::set_intersection(vec.begin(), vec.begin() + 5, vec.begin() + 5, vec.end(), std::back_inserter(coincident));
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
