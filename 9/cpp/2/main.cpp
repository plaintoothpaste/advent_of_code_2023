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
const static bool debug = false;


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
std::vector<int> preExtrapolate(std::vector<int> v) {
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

    const auto extrapolated_delta = preExtrapolate(delta);
    v.insert(v.begin(), v.front() - extrapolated_delta.front());
    if (debug) {
        printVec(v);
    }
    return v;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<std::vector<int>(std::string)> f = [](const std::string& line) {
        return fileParse::lineToIntVector(line);
    };
    const auto readings = fileParse::parse(file_handle, f);
    const auto acc = std::accumulate(readings.begin(), readings.end(), 0, [](const int summer, const std::vector<int>& v) { return summer + extrapolate(v).back(); });
    const auto pre_acc = std::accumulate(readings.begin(), readings.end(), 0, [](const int summer, const std::vector<int>& v) { return summer + preExtrapolate(v).front(); });

    // 114
    std::cout << "final=" << acc << " \n";
    std::cout << "final=" << pre_acc << " \n";
}
