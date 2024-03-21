#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ostream>
#include <random>
#include <ranges>
#include <string>
#include <vector>
#include "file_parse.h"


using field = std::vector<std::vector<unsigned short>>;
field convert(const std::vector<std::string>& lines) {
    auto f = field{};
    f.reserve(lines.size());
    for (const std::string& row : lines) {
        f.emplace_back();
        f.back().reserve(lines.size());
        for (const char c : row) {
            switch (c) {
            case '.':
                f.back().push_back(0);
                break;
            case '#':
                f.back().push_back(1);
                break;
            default:
                throw std::exception("could not convert char");
            }
        }
    }
    return f;
}

/// Assumes that a line of values are supplied, it returns all the locations of symmetry along the line
std::vector<size_t> symmetries(const field& f, const std::vector<size_t>& rows, const std::vector<size_t>& cols) {
    auto symms = std::vector<size_t>{};

    const auto values_match = [&](const size_t lhs, const size_t rhs) { return f[rows[lhs]][cols[lhs]] == f[rows[rhs]][cols[rhs]]; };
    for (auto s = size_t{ 0 }; s < size(rows) - 1; s++) {
        auto good_reflection = true;
        for (auto check = size_t{ 0 }; check <= s; check++) {
            const auto reflected_check = 2 * (s - check) + 1;
            if (reflected_check < size(rows) && ! values_match(check, reflected_check)) {
                good_reflection = false;
                break;
            }
        }
        if (good_reflection) {
            symms.push_back(s);
        }
    }
    return symms;
}
void test_symmetries() {
    const auto f = field{ { 1, 0, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 1, 1, 0 } };
    const auto test_1 = symmetries(f, { 0, 0, 0, 0 }, { 0, 1, 2, 3 });
    const auto expect_1 = std::vector<size_t>{ 1 };
    assert(test_1 == expect_1);

    const auto test_2 = symmetries(f, { 2, 2, 2, 2 }, { 0, 1, 2, 3 });
    const auto expect_2 = std::vector<size_t>{ 0, 1, 2 };
    assert(test_2 == expect_2);

    const auto test_3 = symmetries(f, { 0, 1, 2, 3 }, { 0, 0, 0, 0 });
    const auto expect_3 = std::vector<size_t>{ 2 };
    assert(test_3 == expect_3);
}

/// determine common vertical symmetries across all rows
std::vector<size_t> rowSymmetries(const field& f) {
    const auto num_cols = f.back().size();
    auto cols = std::vector<size_t>{};
    auto rows = std::vector<size_t>{};
    cols.reserve(num_cols);
    rows.resize(num_cols, 0);
    for (size_t i = 0; i < num_cols; ++i) {
        cols.push_back(i);
    }

    auto existing_symmetries = symmetries(f, rows, cols);
    for (auto r = size_t{ 1 }; r < f.size(); ++r) {
        for (auto& v : rows) { // set to current row
            v = r;
        }
        const auto new_symmetries = symmetries(f, rows, cols);
        const auto old_symmetries = existing_symmetries;
        existing_symmetries.clear();
        std::ranges::set_intersection(existing_symmetries, new_symmetries, std::back_inserter(existing_symmetries));
    }
    return existing_symmetries;
}

/// determine common horizontal symmetries across all columns
std::vector<size_t> columnSymmetries(const field& f) {
    const auto num_rows = f.size();
    auto cols = std::vector<size_t>{};
    auto rows = std::vector<size_t>{};
    cols.resize(num_rows, 0);
    rows.reserve(num_rows);
    for (size_t i = 0; i < num_rows; ++i) {
        rows.push_back(i);
    }

    auto existing_symmetries = symmetries(f, rows, cols);
    for (auto c = size_t{ 1 }; c < f.back().size(); ++c) {
        for (auto& v : cols) { // set to current col
            v = c;
        }
        const auto new_symmetries = symmetries(f, rows, cols);
        const auto old_symmetries = existing_symmetries;
        existing_symmetries.clear();
        std::ranges::set_intersection(existing_symmetries, new_symmetries, std::back_inserter(existing_symmetries));
    }
    return existing_symmetries;
}


struct allSymmetries {
    const std::vector<size_t> row_symmetries; // locations where all rows are symmetrical vertically
    const std::vector<size_t> column_symmetries; // locations where all collumns are symmetrical horizontally

    explicit allSymmetries(const field& f)
        : row_symmetries(rowSymmetries(f))
        , column_symmetries(columnSymmetries(f)) {
    }

    size_t score() const {
        size_t score = 0;
        for (const auto r : row_symmetries) {
            score += r + 1;
        }
        for (const auto c : column_symmetries) {
            score += 100 * (c + 1);
        }
        return score;
    }
};

struct SingleField {
    const field f;
    const size_t rows;
    const size_t cols;
    const allSymmetries symmetries;

    explicit SingleField(const std::vector<std::string>& lines)
        : f(convert(lines))
        , rows(size(lines))
        , cols(size(lines.back()))
        , symmetries(f) {
    }

    friend std::ostream& operator<<(std::ostream& o, const SingleField& data) {
        const int data_width = static_cast<int>(data.rows) / 10 + 1;

        o << "---\n";
        for (size_t i = 1; i <= data.f.back().size(); ++i) {
            o << std::setw(data_width) << i;
        }
        o << "\n";

        // first all vertical symmetries
        if (! data.symmetries.row_symmetries.empty()) {
            size_t index = 0;
            for (size_t i = 0; i <= data.f.size(); ++i) {
                if (i == data.symmetries.row_symmetries[index]) {
                    o << std::setw(data_width) << '>';
                } else if (i == data.symmetries.row_symmetries[index] + 1) {
                    o << std::setw(data_width) << '<';
                    index++;
                } else {
                    o << std::setw(data_width) << ' ';
                }
            }
        }
        o << "\n";

        // each line
        size_t index = 0;
        for (size_t i = 0; i < data.f.size(); ++i) {
            const auto& line = data.f[i];
            for (const auto c : line) {
                switch (c) {
                case 0:
                    o << std::setw(data_width) << '.';
                    break;
                case 1:
                    o << std::setw(data_width) << '#';
                    break;
                default:
                    throw std::exception("could not convert char");
                }
            }
            // print horizontal symmetry
            if (i == data.symmetries.column_symmetries[index]) {
                o << std::setw(data_width) << 'v';
            } else if (i == data.symmetries.column_symmetries[index] + 1) {
                o << std::setw(data_width) << '^';
                index++;
            } else {
                o << std::setw(data_width) << ' ';
            }
            o << i + 1 << "\n";
        }
        o << "---\n";
        return o;
    }
    size_t score() const {
        return symmetries.score();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    // total should be the sum of columns to the left, plus 100 times rows above
    // tests
    {
        test_symmetries();
        const auto input = std::vector<std::string>{ "#..#", ".##.", "....", "....", ".##." };
        const auto actual = SingleField(input);
        const auto expected = field{ { 1, 0, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 1, 1, 0 } };
        assert(actual.f[0] == expected[0]);
        assert(actual.f[1] == expected[1]);
        assert(actual.f[2] == expected[2]);
        std::cout << actual;
    }
    std::cout << "All tests passed\n";

    auto file_handle = fileParse::FileHandle(argv[1]);
    auto score = 0;
    for (const auto field_string : fileParse::readFileByParagraph(file_handle)) {
        const auto field = SingleField(field_string);
        std::cout << field;
        score += field.score();
    }

    std::cout << "total = " << score;
    // 405
}
