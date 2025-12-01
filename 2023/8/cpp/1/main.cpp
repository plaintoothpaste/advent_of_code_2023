#include <algorithm>
#include <array>
#include <cassert>
#include <execution>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

// convert the three letter number to an int
int strToNum(const std::string& line) {
    auto out = 0;
    for (const auto [i, c] : enumerate(line)) {
        const auto off = (static_cast<int>(c) - 65);
        const auto power = static_cast<int>(std::pow(26, i));
        out += off * power;
    }
    return out;
    // AAA -> 0+0+0 = 0
    // zzz -> 25+25*26+25*26*26 = 17575
    // ABA -> 0+1*26+0 = 26
    // AAC -> 0+0+2*26*26 = 1352
    // XKM -> 23+10*26+12*26*26 = 8395
    // JKH -> 9 + 10*26+7*26*26 = 5001
}

enum class left_right { left, right };

std::vector<int> negativeOneArray(const size_t size) {
    auto out = std::vector<int>{};
    out.resize(size);
    for (int& v : out) {
        v = -1;
    }
    return out;
}

std::vector<left_right> parseLr(fileParse::FileHandle& f) {
    const std::function<std::vector<left_right>(std::string)> fn_lr = [](const std::string& line) {
        auto out = std::vector<left_right>{};
        out.reserve(line.size());
        for (const char c : line) {
            switch (c) {
            case 'L':
                out.push_back(left_right::left);
                break;
            case 'R':
                out.push_back(left_right::right);
                break;
            default:
                throw std::exception("not allowed char");
            }
        }
        return out;
    };
    auto pattern = parseSingleLine(f, fn_lr);
    f.getLine(); // empty line

    std::cout << "size of lr=" << pattern.size() << " \n";
    return pattern;
}

std::tuple<std::vector<int>, std::vector<int>, int> getMaps(fileParse::FileHandle& f, const int zzz) {
    auto left_map = negativeOneArray(zzz + 1);
    auto right_map = negativeOneArray(zzz + 1);

    static bool first_line = false;
    int current_index = -1;
    const std::function<int(std::string)> fn_nums = [&left_map, &right_map, &current_index](const std::string& line) {
        const auto index = strToNum(line.substr(0, 3));
        left_map[index] = strToNum(line.substr(7, 3));
        right_map[index] = strToNum(line.substr(12, 3));
        if (! first_line) {
            current_index = index;
            first_line = true;
        }
        return 0;
    };
    const auto _ = fileParse::parse(f, fn_nums);

    return { left_map, right_map, current_index };
}

std::pair<int, int> travelPath(const std::vector<int>& left, const std::vector<int>& right, const std::vector<left_right>& instructions, int index, const int zzz) {
    for (const auto [i, step] : enumerate(instructions)) {
        auto new_index = -1;
        switch (step) {
        case left_right::left:
            new_index = left[index];
            break;
        case left_right::right:
            new_index = right[index];
            break;
        }
        if (new_index == zzz) {
            return { new_index, i + 1 };
        }
        index = new_index;
    }
    return { index, instructions.size() };
};
std::vector<std::pair<int, int>> getEnds(const std::vector<int>& lm, const std::vector<int>& rm, const std::vector<left_right>& p, const int zzz) {
    auto indexes = negativeOneArray(zzz + 1);
    int prev = -1;
    for (int& i : indexes) {
        ++prev;
        i = prev;
    }
    auto end = std::vector<std::pair<int, int>>{};
    end.resize(zzz + 1, { -1, -1 });

    auto length = negativeOneArray(zzz + 1);
    std::for_each(std::execution::par, indexes.begin(), indexes.end(), [&](const int index) {
        if (lm[index] != -1) {
            end[index] = travelPath(lm, rm, p, index, zzz);
        }
    });

    // tests
    for (const auto [index, e] : end) {
        if (index != -1) {
            assert(end[index].first != -1);
        }
    }
    return end;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    // testing
    assert(strToNum("AAA") == 0);
    const int zzz = strToNum("ZZZ");
    assert(zzz == 17575);
    assert(strToNum("ABA") == 26);
    assert(strToNum("AAC") == 1352);
    assert(strToNum("XKM") == 8395);
    assert(strToNum("JKH") == 5001);

    auto file_handle = fileParse::FileHandle(argv[1]);

    const auto pattern = parseLr(file_handle);
    auto [left_map, right_map, current_index] = getMaps(file_handle, zzz);
    const auto end = getEnds(left_map, right_map, pattern, zzz);

    auto steps = 0;
    while (true) {
        if (current_index == zzz) {
            break;
        }

        std::cout << "step: " << current_index << " : " << end[current_index].first << " \n";
        steps += end[current_index].second;
        current_index = end[current_index].first;
    }

    std::cout << "final=" << steps << " \n";
}
