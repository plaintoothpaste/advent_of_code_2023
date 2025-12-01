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

using Galaxy = std::vector<std::vector<int>>;

int charToNum(const char c) {
    switch (c) {
    case '.':
        return 0;
    case '#':
        return 1;
    default:
        throw std::exception("could not convert char");
    }
}

std::vector<int> lineToNums(const std::string& line) {
    auto out = std::vector<int>{};
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

using RowColumn = std::pair<std::vector<int>, std::vector<int>>;

RowColumn getPositions(const Galaxy& g) {
    auto rows = std::vector<int>{};
    auto cols = std::vector<int>{};
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
    auto rows = std::vector<int>{};
    rows.resize(g.size(), 0);
    auto cols = std::vector<int>{};
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
int calculateDistance(const int row1, const int col1, const int row2, const int col2, const RowColumn& empties) {
    int distance = std::abs(row2 - row1) + std::abs(col2 - col1);
    const int min_row = std::min(row1, row2);
    const int min_col = std::min(col1, col2);
    const int max_row = std::max(row1, row2);
    const int max_col = std::max(col1, col2);

    const auto& [er, ec] = empties;
    for (int i = min_row + 1; i < max_row; i++) {
        if (er[i] == 0) {
            distance++;
        }
    }
    for (int i = min_col + 1; i < max_col; i++) {
        if (ec[i] == 0) {
            distance++;
        }
    }
    return distance;
}
int allDistances(const RowColumn& locations, const RowColumn& empties) {
    const auto& [rows, cols] = locations;
    int distance = 0;
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
    const std::function<std::vector<int>(std::string)> f = [](const std::string& line) { return lineToNums(line); };
    auto galaxy = fileParse::parse(file_handle, f);
    print(galaxy);
    const auto locations = getPositions(galaxy);
    const auto empty = getEmptyRowColumn(galaxy);
    print(locations);
    printEmpty(empty);

    const auto distance = allDistances(locations, empty);
    std::cout << distance << "\n";
}
