#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <numeric>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

enum class spring { broken, working, unknown };

std::ostream& operator<<(std::ostream& o, const spring& spring) {
    switch (spring) {
    case spring::broken:
        o << '#';
        break;
    case spring::unknown:
        o << '?';
        break;
    case spring::working:
        o << '.';
        break;
    }
    return o;
}

std::vector<spring> lineToEnums(const std::string& line) {
    auto out = std::vector<spring>{};
    out.reserve(line.size());
    for (const char c : line) {
        switch (c) {
        case '.':
            out.push_back(spring::working);
            break;
        case '#':
            out.push_back(spring::broken);
            break;
        case '?':
            out.push_back(spring::unknown);
            break;
        default:
            throw std::exception("could not convert char");
        }
    }
    return out;
}
// can use std function in the parse file for int to vector

std::vector<int> convert(const std::vector<spring>& s) {
    auto out = std::vector<int>{};
    out.emplace_back(0);

    for (const auto e : s) {
        if (e == spring::broken) {
            out.back()++;
        } else if (e == spring::working && out.back() != 0) {
            out.emplace_back(0);
        }
    }
    if (out.back() == 0) {
        out.pop_back();
    }
    return out;
}

struct SingleRow {
    std::vector<spring> symbols;
    std::vector<int> numbers;

    SingleRow(const std::string& line) {
        const auto limit = line.find_first_of(" ");
        const auto beginning = line.substr(0, limit);
        const auto ending = line.substr(limit + 1, line.size());
        symbols = lineToEnums(beginning);
        numbers = fileParse::lineToIntVector(ending, ",");
    }
    bool noUnknowns() const {
        return ! std::ranges::any_of(symbols, [](const spring& s) { return s == spring::unknown; });
    }
    bool valid() const {
        return numbers == convert(symbols);
    }
    void recalculateNumber() {
        numbers = convert(symbols);
    }
};

std::ostream& operator<<(std::ostream& o, const SingleRow& data) {
    for (const auto e : data.symbols) {
        o << e;
    }
    o << " ";
    for (const auto i : data.numbers) {
        o << i << ",";
    }
    o << "\n";
    return o;
}

std::vector<SingleRow> permute(SingleRow initial) {
    if (initial.noUnknowns()) {
        initial.recalculateNumber();
        return { initial };
    }

    auto out = std::vector<SingleRow>{};
    for (auto& i : initial.symbols) {
        if (i == spring::unknown) {
            {
                i = spring::broken;
                const auto brocken = permute(initial);
                out.reserve(out.size() + brocken.size());
                out.insert(out.end(), brocken.begin(), brocken.end());
            }
            {
                i = spring::working;
                const auto working = permute(initial);
                out.reserve(out.size() + working.size());
                out.insert(out.end(), working.begin(), working.end());
            }
            return out;
        }
    }
    throw std::exception();
}
int permuteCount(SingleRow initial) {
    if (initial.noUnknowns()) {
        return initial.valid() ? 1 : 0;
    }

    for (auto& i : initial.symbols) {
        if (i == spring::unknown) {
            i = spring::broken;
            const auto brocken = permuteCount(initial);
            i = spring::working;
            const auto working = permuteCount(initial);
            return brocken + working;
        }
    }
    throw std::exception();
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    // tests
    {
        const auto lhs = std::vector<int>{ 1, 2, 3 };
        const auto rhs = convert(std::vector<spring>{ spring::broken, spring::working, spring::broken, spring::broken, spring::working, spring::broken, spring::broken, spring::broken });
        assert(lhs.size() == rhs.size());
        for (const auto [i, _] : enumerate(lhs)) {
            assert(lhs[i] == rhs[i]);
        }
    }
    std::cout << "All tests passed\n";

    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<SingleRow(std::string)> f = [](const std::string& line) { return SingleRow(line); };
    auto data = fileParse::parse(file_handle, f);
    int total = 0;
    for (const auto i : data) {
        const auto permutations = permuteCount(i);
        total += permutations;
        // std::cout << i << "permutations = " << permutations << "\n---\n";
    }

    std::cout << "total = " << total;
    // 1,4,1,1,4,10
}
