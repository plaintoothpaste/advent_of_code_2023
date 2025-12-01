#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
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

const size_t galaxy_expansion = 1000000;

using Galaxy = std::vector<std::vector<size_t>>;

size_t charToNum(const char c) {
    switch (c) {
    case '.':
        return 0;
    case '#':
        return 1;
    default:
        throw std::exception("could not convert char");
    }
}

std::vector<size_t> lineToNums(const std::string& line) {
    auto out = std::vector<size_t>{};
    out.reserve(line.size());
    for (const char c : line) {
        out.emplace_back(charToNum(c));
    }
    return out;
}

void print(const Galaxy& g) {
    for (const auto& row : g) {
        for (const auto& value : row) {
            std::cout << value;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

using RowColumn = std::pair<std::vector<size_t>, std::vector<size_t>>;

RowColumn getPositions(const Galaxy& g) {
    auto rows = std::vector<size_t>{};
    auto cols = std::vector<size_t>{};
    for (const auto& [row_index, row] : enumerate(g)) {
        for (const auto& [col_index, value] : enumerate(row)) {
            if (value != 0) {
                rows.push_back(row_index);
                cols.push_back(col_index);
            }
        }
    }
    return { rows, cols };
}

RowColumn getEmptyRowColumn(const Galaxy& g) {
    auto rows = std::vector<size_t>{};
    rows.resize(g.size(), 0);
    auto cols = std::vector<size_t>{};
    cols.resize(g[0].size(), 0);
    for (const auto& [row_index, row] : enumerate(g)) {
        for (const auto& [col_index, value] : enumerate(row)) {
            if (value != 0) {
                rows[row_index]++;
                cols[col_index]++;
            }
        }
    }
    return { rows, cols };
}

void print(const RowColumn& rc) {
    const auto& [r, c] = rc;
    for (const auto& [i, _] : enumerate(r)) {
        std::cout << "row=" << r[i] << " col=" << c[i] << "\n";
    }
}
void printEmpty(const RowColumn& rc) {
    const auto& [r, c] = rc;
    std::cout << "Empty rows = [";
    for (const auto& [i, v] : enumerate(r)) {
        if (v == 0) {
            std::cout << i << ", ";
        }
    }
    std::cout << "]\n";
    std::cout << "Empty cols = [";
    for (const auto& [i, v] : enumerate(c)) {
        if (v == 0) {
            std::cout << i << ", ";
        }
    }
    std::cout << "]\n";
}
/// distance in 'blocks' rather then arithmetic.
size_t calculateDistance(const size_t row1, const size_t col1, const size_t row2, const size_t col2, const RowColumn& empties) {
    const size_t min_row = row1<=row2 ? row1 : row2;
    const size_t min_col = col1<=col2 ? col1 : col2;
    const size_t max_row = row1>row2 ? row1 : row2;
    const size_t max_col = col1>col2? col1 : col2;
    size_t distance = max_row - min_row + max_col - min_col;

    const auto& [er, ec] = empties;
    for (size_t i = min_row + 1; i < max_row; i++) {
        if (er[i] == 0) {
            distance+=galaxy_expansion-1;
        }
    }
    for (size_t i = min_col + 1; i < max_col; i++) {
        if (ec[i] == 0) {
            distance+=galaxy_expansion-1;
        }
    }
    return distance;
}
size_t allDistances(const RowColumn& locations, const RowColumn& empties) {
    const auto& [rows, cols] = locations;
    size_t distance = 0;
    for (size_t index = 0; index < rows.size(); index++) {
        const auto row1 = rows[index];
        const auto col1 = cols[index];

        for (size_t sub_index = index; sub_index < rows.size(); sub_index++) {
            const auto row2 = rows[sub_index];
            const auto col2 = cols[sub_index];
            distance += calculateDistance(row1, col1, row2, col2, empties);
        }
    }
    return distance;
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<std::vector<size_t>(std::string)> f = [](const std::string& line) { return lineToNums(line); };
    auto galaxy = fileParse::parse(file_handle, f);
    print(galaxy);
    const auto locations = getPositions(galaxy);
    const auto empty = getEmptyRowColumn(galaxy);
    print(locations);
    printEmpty(empty);

    const auto distance = allDistances(locations, empty);
    std::cout << distance << "\n";
}
