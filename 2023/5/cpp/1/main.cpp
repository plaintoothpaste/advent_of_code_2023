#include <array>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

class mapper {
public:
    mapper(const size_t lower, const size_t count, const size_t out_lower)
        : lower(lower)
        , count(count)
        , out_lower(out_lower) {
    }
    size_t map(const size_t in) const {
        if (in < lower || in > lower + count) {
            return in;
        }
        const int delta = in - lower;
        return out_lower + delta;
    }

private:
    size_t lower;
    size_t count;
    size_t out_lower;
};


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    // notes: each stage is always in order
    const std::function<std::vector<size_t>(std::string)> fn_seed = [](const std::string& line) { return fileParse::lineToSizetVector(line.substr(7)); };
    auto seeds = fileParse::partialUntilNewLine(file_handle, fn_seed)[0];

    const std::function<std::vector<size_t>(std::string)> fn = [](const std::string& line) {
        if (line.substr(0, 1).find_first_of("0123456789") != std::string::npos) {
            return fileParse::lineToSizetVector(line);
        }
        return std::vector<size_t>{};
    };
    while (! file_handle.eof()) {
        auto number_rows = fileParse::partialUntilNewLine(file_handle, fn);
        number_rows.erase(number_rows.begin());

        auto is_converted = std::vector<bool>{};
        is_converted.resize(seeds.size(), false);
        for (auto& res : number_rows) {
            const auto m = mapper(res[1], res[2], res[0]);
            std::cout << "----\n";
            for (auto [i, seed] : enumerate(seeds)) {
                if (! is_converted[i]) {
                    const auto new_seed = m.map(seed);
                    if (new_seed != seed) {
                        is_converted[i] = true;
                        seed = new_seed;
                        std::cout << "seed[" << i << "]: " << seed << "\n";
                    }
                }
            }
        }
    }

    std::cout << "---Final results\n";
    for (auto [i, seed] : enumerate(seeds)) {
        std::cout << "seed[" << i << "]: " << seed << "\n";
    }
    const auto abs_min = *std::ranges::min_element(seeds);
    std::cout << "---abs min=" << abs_min << " \n";
}
