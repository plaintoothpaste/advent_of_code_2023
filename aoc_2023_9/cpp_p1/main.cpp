#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"
const static bool debug = true;


bool allN(const std::vector<int>& v, const int n) {
    return std::ranges::all_of(v, [&](const int i) { return i == n; });
}

void printVec(const std::vector<int>& v) {
    for (const int value : v) {
        std::cout << value << ", ";
    }
    std::cout << " \n";
}
std::vector<int> extrapolate(std::vector<int> v) {
    if (allN(v, v.back())) {
        v.push_back(v.back());
        if (debug) {
            printVec(v);
        }
        return v;
    }

    auto delta = std::vector<int>{};
    delta.resize(v.size() - 1);
    for (size_t i = 0; i < v.size() - 1; ++i) {
        delta[i] = v[i + 1] - v[i];
    }

    const auto extrapolated_delta = extrapolate(delta);
    v.push_back(v.back() + extrapolated_delta.back());
    if (debug) {
        printVec(v);
    }
    return v;
}


int main() {
    // notes: each stage is always in order
    auto file_handle = FileHandle("input.txt");
    const std::function<std::vector<int>(std::string)> f = [](const std::string& line) {
        return lineToIntVector(line);
    };
    const auto readings = parse(file_handle, f);
    const auto acc = std::accumulate(readings.begin(), readings.end(), 0, [](const int summer, const std::vector<int>& v) { return summer + extrapolate(v).back(); });

    // 114
    std::cout << "final=" << acc << " \n";
}
