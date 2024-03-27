#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>
#include "file_parse.h"

enum class stone {
    empty,
    fixed,
    moveable,
};

using field = std::vector<std::vector<stone>>;
std::vector<stone> convertLine(const std::string& line) {
    auto out = std::vector<stone>{};
    out.reserve(line.size());
    for (const auto& c : line) {
        switch (c) {
        case '.':
            out.push_back(stone::empty);
            break;
        case '#':
            out.push_back(stone::fixed);
            break;
        case 'O':
            out.push_back(stone::moveable);
            break;
        default:
            throw std::exception("could not convert char");
        }
    }
    return out;
}

size_t load(const field& f) {
    auto load_multiplier = f.size();
    auto score = size_t{ 0 };
    for (const auto& row : f) {
        for (const auto& v : row) {
            if (v == stone::moveable) {
                score += load_multiplier;
            }
        }
        load_multiplier--;
    }
    return score;
}

std::ostream& operator<<(std::ostream& o, const field& data) {
    auto load_multiplier = data.size();
    for (const auto& line : data) {
        for (const auto& s : line) {
            switch (s) {
            case stone::empty:
                o << '.';
                break;
            case stone::moveable:
                o << 'O';
                break;
            case stone::fixed:
                o << '#';
                break;
            }
        }
        o << " " << load_multiplier << "\n";
        load_multiplier--;
    }
    o << "score: " << load(data) << "\n";
    return o;
}

field rollNorth(const field& original) {
    auto field = original;
    const auto total_columns = original.back().size();
    for (auto re_roll = original.size(); re_roll > 1; re_roll--) {
        for (auto row_index = size_t{ 1 }; row_index < re_roll; row_index++) {
            auto& current = field[row_index];
            auto& previous = field[row_index - 1];

            for (auto column_index = size_t{ 0 }; column_index < total_columns; column_index++) {
                if (current[column_index] == stone::moveable && previous[column_index] == stone::empty) {
                    current[column_index] = stone::empty;
                    previous[column_index] = stone::moveable;
                }
            }
        }
    }
    return field;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<std::vector<stone>(std::string)> parse_fn = [](const std::string& line) { return convertLine(line); };
    const field f = fileParse::parse(file_handle, parse_fn);
    const field f_north = rollNorth(f);

    std::cout << f;
    std::cout << f_north;
}
