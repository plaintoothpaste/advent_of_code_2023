#include <algorithm>
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

struct rowCol {
    size_t row = 0;
    size_t column = 0;
};

std::vector<rowCol> allRowCol(const field& ref) {
    auto out = std::vector<rowCol>{};
    out.reserve(ref.size() * ref.back().size());
    for (size_t row = 0; row < ref.size(); ++row) {
        for (size_t col = 0; col < ref.back().size(); ++col) {
            out.emplace_back(row, col);
        }
    }
    return out;
}

std::vector<rowCol> removeRow(std::vector<rowCol>&& locations, const size_t r) {
    std::erase_if(locations, [&r](const rowCol& rc) { return rc.row == r; });
    return locations;
}

std::vector<rowCol> removeColumn(std::vector<rowCol>&& locations, const size_t c) {
    std::erase_if(locations, [&c](const rowCol& rc) { return rc.column == c; });
    return locations;
}

field rollNorth(const field& original) {
    auto field = original;
    auto locations = allRowCol(field);
    locations = removeRow(std::move(locations), 0);
    auto current_limit = field.size() - 1;
    while (! locations.empty()) {
        for (const auto loc : locations) {
            auto& current = field[loc.row][loc.column];
            auto& previous = field[loc.row - 1][loc.column];
            if (current == stone::moveable && previous == stone::empty) {
                current = stone::empty;
                previous = stone::moveable;
            }
        }
        locations = removeRow(std::move(locations), current_limit);
        current_limit--;
    }
    return field;
}

field rollSouth(const field& original) {
    auto field = original;
    auto locations = allRowCol(field);
    std::ranges::sort(locations, [](const rowCol& lhs, const rowCol& rhs) { return lhs.row > rhs.row; });
    locations = removeRow(std::move(locations), field.size() - 1);
    auto current_limit = 0;
    while (! locations.empty()) {
        for (const auto loc : locations) {
            auto& current = field[loc.row][loc.column];
            auto& previous = field[loc.row + 1][loc.column];
            if (current == stone::moveable && previous == stone::empty) {
                current = stone::empty;
                previous = stone::moveable;
            }
        }
        locations = removeRow(std::move(locations), current_limit);
        current_limit++;
    }
    return field;
}

field rollWest(const field& original) {
    auto field = original;
    auto locations = allRowCol(field);
    std::ranges::sort(locations, [](const rowCol& lhs, const rowCol& rhs) { return lhs.column < rhs.column; });
    locations = removeColumn(std::move(locations), 0);
    auto current_limit = field.back().size() - 1;
    while (! locations.empty()) {
        for (const auto loc : locations) {
            auto& current = field[loc.row][loc.column];
            auto& previous = field[loc.row][loc.column - 1];
            if (current == stone::moveable && previous == stone::empty) {
                current = stone::empty;
                previous = stone::moveable;
            }
        }
        locations = removeColumn(std::move(locations), current_limit);
        current_limit--;
    }
    return field;
}

field rollEast(const field& original) {
    auto field = original;
    auto locations = allRowCol(field);
    std::ranges::sort(locations, [](const rowCol& lhs, const rowCol& rhs) { return lhs.column > rhs.column; });
    locations = removeColumn(std::move(locations), field.back().size() - 1);
    auto current_limit = 0;
    while (! locations.empty()) {
        for (const auto loc : locations) {
            auto& current = field[loc.row][loc.column];
            auto& previous = field[loc.row][loc.column + 1];
            if (current == stone::moveable && previous == stone::empty) {
                current = stone::empty;
                previous = stone::moveable;
            }
        }
        locations = removeColumn(std::move(locations), current_limit);
        current_limit++;
    }
    return field;
}
void test_rolling() {
    {
        // North
        const auto input = field{ { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::moveable, stone::empty } };
        const auto expected = field{ { stone::empty, stone::moveable, stone::empty }, { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty } };
        const auto actual = rollNorth(input);
        assert(actual == expected);
    }
    {
        // South
        const auto input = field{ { stone::empty, stone::moveable, stone::empty }, { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty } };
        const auto expected = field{ { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::moveable, stone::empty } };
        const auto actual = rollSouth(input);
        assert(actual == expected);
    }
    {
        // west
        const auto input = field{ { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::moveable }, { stone::empty, stone::empty, stone::empty } };
        const auto expected = field{ { stone::empty, stone::empty, stone::empty }, { stone::moveable, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty } };
        const auto actual = rollWest(input);
        assert(actual == expected);
    }
    {
        // east
        const auto input = field{ { stone::empty, stone::empty, stone::empty }, { stone::moveable, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::empty } };
        const auto expected = field{ { stone::empty, stone::empty, stone::empty }, { stone::empty, stone::empty, stone::moveable }, { stone::empty, stone::empty, stone::empty } };
        const auto actual = rollEast(input);
        assert(actual == expected);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }

    test_rolling();
    std::cout << "Testing passed \n";

    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<std::vector<stone>(std::string)> parse_fn = [](const std::string& line) { return convertLine(line); };
    const field f = fileParse::parse(file_handle, parse_fn);
    std::cout << f;

    auto existing_field_hare = f;
    auto existing_field_tortoise = f;
    bool finished = false;
    size_t iters = 0;

    constexpr size_t end_point = 1000000000;
    for (size_t cycle = 1; cycle < end_point; ++cycle) {
        const auto new_field_hare = rollEast(rollSouth(rollWest(rollNorth(existing_field_hare))));

        if (cycle % 2 == 0) {
            const auto new_field_tortoise = rollEast(rollSouth(rollWest(rollNorth(existing_field_tortoise))));

            if (new_field_hare == new_field_tortoise) {
                // cycle found
                iters = end_point % (cycle / 2);
                finished = true;
            }
            existing_field_tortoise = new_field_tortoise;
        }
        if (finished) {
            break;
        }
        existing_field_hare = new_field_hare;
    }

    // run through iters to find the final result
    auto final = f;
    for (size_t cycle = 0; cycle < iters; ++cycle) {
        final = rollEast(rollSouth(rollWest(rollNorth(final))));
    }
    std::cout << final;
}
