#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <numeric>
#include <optional>
#include <ostream>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"


using Field = std::vector<std::vector<unsigned short>>;

bool rowSymmetricalAt(const size_t symmetry_index, const Field& f, const size_t row) {
    const auto& row_vals = f[row];
    auto offset = 0;
    while (symmetry_index >= offset && symmetry_index + 1 + offset < row_vals.size()) {
        const auto lh = row_vals[symmetry_index - offset];
        const auto rh = row_vals[symmetry_index + 1 + offset];
        if (lh != rh) {
            return false;
        }
        ++offset;
    }
    return true;
};
bool colSymmetricalAt(const size_t symmetry_index, const Field& f, const size_t col) {
    auto offset = 0;
    while (symmetry_index >= offset && symmetry_index + 1 + offset < f.size()) {
        const auto upper = f[symmetry_index - offset][col];
        const auto lower = f[symmetry_index + 1 + offset][col];
        if (upper != lower) {
            return false;
        }
        ++offset;
    }
    return true;
};

bool allRowsSymmetricalAt(const size_t symmetry_index, const Field& f) {
    for (const auto& [ri, row] : enumerate(f)) {
        if (! rowSymmetricalAt(symmetry_index, f, ri)) {
            return false;
        }
    }
    return true;
};

bool allColsSymmetricalAt(const size_t symmetry_index, const Field& f) {
    for (size_t ci = 0; ci < f[0].size(); ++ci) {
        if (! colSymmetricalAt(symmetry_index, f, ci)) {
            return false;
        }
    }
    return true;
};

enum class symmetryLine { vertical, horizontal };

struct SingleSymmetry {
    symmetryLine type;
    size_t location;
    SingleSymmetry(const symmetryLine t, size_t l)
        : type(t)
        , location(l) {
    }

    size_t score() const {
        return type == symmetryLine::vertical ? location + 1 : 100 * (location + 1);
    }
};

class SingleField {
public:
    Field field;
    std::vector<SingleSymmetry> all_symmetries;

    explicit SingleField(const std::vector<std::string>& lines) {
        field.reserve(lines.size());
        for (const std::string& row : lines) {
            field.emplace_back();
            field.back().reserve(lines.size());
            for (const char c : row) {
                switch (c) {
                case '.':
                    field.back().push_back(0);
                    break;
                case '#':
                    field.back().push_back(1);
                    break;
                default:
                    throw std::exception("could not convert char");
                }
            }
        }

        for (size_t s = 0; s < field.back().size() - 1; ++s) {
            if (allRowsSymmetricalAt(s, field)) {
                all_symmetries.emplace_back(symmetryLine::horizontal, s);
            }
        }

        for (size_t s = 0; s < field.size() - 1; ++s) {
            if (allRowsSymmetricalAt(s, field)) {
                all_symmetries.emplace_back(symmetryLine::vertical, s);
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& o, const SingleField& data) {
        auto s = std::vector<std::vector<std::string>>{};
        s.resize(data.field.size() + 2);
        for (auto& line : s) {
            line.resize(data.field.back().size() + 2);
        }
        auto id = 1;
        for (auto& c : s[0]) {
            c = std::to_string(id);
            id++;
        }

        id = 1;
        for (auto& line : s) {
            line.back() = std::to_string(id);
            id++;
        }
        return o;
    }

    size_t score() const {
        return std::accumulate(all_symmetries.begin(), all_symmetries.end(), size_t{ 0 }, [](const size_t& acc, const auto s) { return acc + s.score(); });
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
        const auto input = std::vector<std::string>{ "#..#", ".##.", "...." };
        const auto actual = SingleField(input);
        const auto expected = Field{ { 1, 0, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } };
        assert(actual.field[0] == expected[0]);
        assert(actual.field[1] == expected[1]);
        assert(actual.field[2] == expected[2]);
        std::cout << actual;

        assert(rowSymmetricalAt(0, actual.field, 0) == false);
        assert(rowSymmetricalAt(1, actual.field, 0) == true);
        assert(rowSymmetricalAt(2, actual.field, 1) == false);
        assert(colSymmetricalAt(1, actual.field, 1) == false);
        assert(colSymmetricalAt(1, actual.field, 3) == true);

        assert(allRowsSymmetricalAt(1, actual.field) == true);
        assert(allColsSymmetricalAt(1, actual.field) == false);
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
