#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include "file_parse.h"

std::vector<std::pair<size_t, size_t>> rowColIndex(const size_t rows, const size_t columns) {
    auto out = std::vector<std::pair<size_t, size_t>>{};
    out.reserve(rows * columns);
    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < columns; ++c) {
            out.emplace_back(r, c);
        }
    }
    return out;
}

struct NumberStore {
    size_t row;
    size_t column_min;
    size_t column_max;
    size_t value;

    NumberStore(const size_t ir, const std::vector<size_t>& ic, const std::vector<std::string>& line)
        : row(ir)
        , column_min(ic.front())
        , column_max(ic.back()) {
        auto string_num = std::string{};
        for (const auto i : ic) {
            string_num += line[i];
        }
        value = std::stoll(string_num);
    }
    std::optional<size_t> ifNeigbourValue(const size_t index_row, const size_t index_column) const {
        {
            const auto min = index_row == 0 ? 0 : index_row - 1;
            const auto max = index_row + 1;
            if (row < min || row > max)
                return {};
        }
        {
            const auto min = index_column == 0 ? 0 : index_column - 1;
            const auto max = index_column + 1;
            if (column_max < min || column_min > max)
                return {};
        }

        return { value };
    }
};

std::vector<NumberStore> charToNums(const std::vector<std::vector<std::string>>& lines, const std::vector<std::pair<size_t, size_t>>& row_cols) {
    auto out = std::vector<NumberStore>{};

    auto is_num_buffer = std::vector<size_t>{};
    is_num_buffer.reserve(lines.front().size());

    for (const auto [row, column] : row_cols) {
        if (column == 0 && ! is_num_buffer.empty()) {
            out.emplace_back(row - 1, is_num_buffer, lines[row - 1]); // from previous row
            is_num_buffer.clear();
        }
        const auto& s = lines[row][column];
        if ((s == "." || s == "*") && ! is_num_buffer.empty()) {
            out.emplace_back(row, is_num_buffer, lines[row]);
            is_num_buffer.clear();
        }
        if (s.find_first_of("0123456789") == 0) {
            is_num_buffer.push_back(column);
        }
    }
    return out;
}


std::vector<std::pair<size_t, size_t>> starToBool(const std::vector<std::vector<std::string>>& lines, const std::vector<std::pair<size_t, size_t>>& row_cols) {
    auto out = std::vector<std::pair<size_t, size_t>>{};
    out.reserve(row_cols.size());
    for (const auto [row, column] : row_cols) {
        if (lines[row][column]=="*") {
            out.emplace_back(row,column);
        }
    }

    return out;
}

int main() {
    const std::function<std::vector<std::string>(std::string)> fn = [](const std::string& line) {
        auto out = std::vector<std::string>{};
        for (const auto& c : line) {
            const auto s = std::string{ c };
            if (s.find_first_of("0123456789.") == 0) {
                out.push_back(s);
            } else {
                out.emplace_back("*");
            }
        }
        return out;
    };
    const auto data = parse("example.txt", fn);
    const auto all_index = rowColIndex(data.size(), data.front().size());
    const auto number_stores = charToNums(data, all_index);
    const auto is_symbol = starToBool(data,all_index);

    for (const auto & [row,col] : is_symbol) {
        
    }
    return 0;
}
