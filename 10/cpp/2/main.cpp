#include <algorithm>
#include <array>
#include <cassert>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

enum class areaType { empty, internal, loop };

class segment {
private:
    segment() = default;

public:
    bool north_ = false;
    bool east_ = false;
    bool south_ = false;
    bool west_ = false;
    areaType type = areaType::empty;

    static segment north() {
        auto s = segment();
        s.north_ = true;
        return s;
    }
    static segment south() {
        auto s = segment();
        s.south_ = true;
        return s;
    }
    static segment east() {
        auto s = segment();
        s.east_ = true;
        return s;
    }
    static segment west() {
        auto s = segment();
        s.west_ = true;
        return s;
    }

    explicit segment(const char c) {
        switch (c) {
        case '|':
            *this = north() | south();
            break;
        case '-':
            *this = east() | west();
            break;
        case 'L':
            *this = north() | east();
            break;
        case 'J':
            *this = north() | west();
            break;
        case 'F':
            *this = east() | south();
            break;
        case '7':
            *this = west() | south();
            break;
        case '.':
            *this = segment();
            break;
        case 'S':
            *this = north() | east() | west() | south();
            break;
        }
    }

    friend segment operator^(const segment lhs, const segment rhs) {
        auto s = segment();
        s.north_ = lhs.north_ ^ rhs.north_;
        s.east_ = lhs.east_ ^ rhs.east_;
        s.south_ = lhs.south_ ^ rhs.south_;
        s.west_ = lhs.west_ ^ rhs.west_;
        return s;
    }
    bool operator==(const segment& rhs) const = default;

    operator wchar_t() const {
        if (south_ && east_ && west_ && north_) {
            return L'α';
        }
        if (north_ && south_) {
            return L'║';
        }
        if (north_ && east_) {
            return L'╚';
        }
        if (north_ && west_) {
            return L'╝';
        }
        if (east_ && west_) {
            return L'═';
        }
        if (south_ && west_) {
            return L'╗';
        }
        if (south_ && east_) {
            return L'╔';
        }
        return L'█';
    }

private:
    friend segment operator|(const segment lhs, const segment rhs) {
        auto s = segment();
        s.north_ = lhs.north_ || rhs.north_;
        s.east_ = lhs.east_ || rhs.east_;
        s.south_ = lhs.south_ || rhs.south_;
        s.west_ = lhs.west_ || rhs.west_;
        return s;
    }
};

using Field = std::vector<std::vector<segment>>;

using rowColumn = std::pair<size_t, size_t>;

void print(const Field& field) {
    for (const auto& row : field) {
        for (const segment value : row) {
            std::wcout << static_cast<wchar_t>(value);
        }
        std::wcout << "\n";
    }
    std::wcout << "\n";
}

void printTypes(const Field& field) {
    for (const auto& row : field) {
        for (const segment value : row) {
            switch (value.type) {
            case areaType::loop:
                std::wcout << static_cast<wchar_t>(value);
                break;
            case areaType::empty:
                std::wcout << L'░';
                break;
            case areaType::internal:
                std::wcout << L'▓';
                break;
            }
        }
        std::wcout << "\n";
    }
    std::wcout << "\n";
}


bool checkNorth(const rowColumn row_col, const Field& field) {
    if (row_col.first > 0) {
        const auto fn = field[row_col.first - 1][row_col.second];
        return fn.south_;
    }
    return false;
}

bool checkSouth(const rowColumn row_col, const Field& field) {
    if ((row_col.first + 1) < field.size()) {
        const auto fs = field[row_col.first + 1][row_col.second];
        return fs.north_;
    }
    return false;
}

bool checkEast(const rowColumn row_col, const Field& field) {
    if ((row_col.second + 1) < field[0].size()) {
        const auto fe = field[row_col.first][row_col.second + 1];
        return fe.west_;
    }
    return false;
}

bool checkWest(const rowColumn row_col, const Field& field) {
    if (row_col.second > 0) {
        const auto fw = field[row_col.first][row_col.second - 1];
        return fw.west_;
    }
    return false;
}

/// <summary>
/// find the first direction that is possible for the starting point
/// </summary>
segment findStartDirection(const rowColumn row_col, const Field& field) {
    if (checkNorth(row_col, field)) {
        return segment::north();
    }
    if (checkSouth(row_col, field)) {
        return segment::south();
    }
    if (checkEast(row_col, field)) {
        return segment::east();
    }
    if (checkWest(row_col, field)) {
        return segment::west();
    }
    throw std::exception();
}

segment opposite(const segment direction) {
    if (direction.north_) {
        return segment::south();
    }
    if (direction.east_) {
        return segment::west();
    }
    if (direction.west_) {
        return segment::east();
    }
    if (direction.south_) {
        return segment::north();
    }
    throw std::exception();
}

rowColumn offset(const rowColumn initial, const segment& direction) {
    if (direction.north_) {
        return { initial.first - 1, initial.second };
    }
    if (direction.east_) {
        return { initial.first, initial.second + 1 };
    }
    if (direction.west_) {
        return { initial.first, initial.second - 1 };
    }
    if (direction.south_) {
        return { initial.first + 1, initial.second };
    }
}
/// mark all non pipes are internal if left right scan says so
void leftRight(Field& field) {
    for (const auto& [row_index, row] : enumerate(field)) {
        bool north_inside = false;
        bool south_inside = false;

        for (const auto& [col_index, s] : enumerate(row)) {
            if (s.type == areaType::loop) {
                if (s.north_) {
                    north_inside = ! north_inside;
                }
                if (s.south_) {
                    south_inside = ! south_inside;
                }
            }

            if (south_inside && north_inside && s.type == areaType::empty) {
                s.type = areaType::internal;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    // testing
    assert(segment(7) == (segment(1) ^ segment(0)));
    assert(segment(1) == (segment(2) ^ segment(3)));

    // notes: each stage is always in order
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<std::vector<segment>(std::string)> f = [](const std::string& line) {
        auto out = std::vector<segment>{};
        out.reserve(line.size());
        for (const char c : line) {
            out.emplace_back(c);
        }
        return out;
    };

    _setmode(_fileno(stdout), _O_U16TEXT);
    auto field = fileParse::parse(file_handle, f);

    print(field);

    auto position = rowColumn{ 0, 0 };
    for (const auto& [row_index, row] : enumerate(field)) {
        for (const auto& [col_index, s] : enumerate(row)) {
            if (s == segment('S')) {
                position = { row_index, col_index };
            }
        }
    }
    const auto start_position = position;
    auto end_position = segment('S');
    end_position.type = areaType::loop;
    segment direction = findStartDirection(position, field);

    auto next_segment = segment('.');
    auto distance = 0;

    while (next_segment != end_position) {
        position = offset(position, direction);
        next_segment = field[position.first][position.second];
        field[position.first][position.second].type = areaType::loop;
        direction = opposite(direction) ^ next_segment;
        ++distance;
    }
    field[start_position.first][start_position.second] = segment('|'); // manually checked
    field[start_position.first][start_position.second].type = areaType::loop;

    printTypes(field);

    leftRight(field);
    printTypes(field);

    auto count = 0;
    for (const auto& row :field) {
        for (const auto& s : row) {
            count = s.type==areaType::internal ? count+1 : count;
        }
    }

    distance = distance / 2;
    // 8
    std::wcout << distance << "\n";
    std::wcout << count << "\n";
}
