#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <limits>
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

std::vector<size_t> expandedSeeds(const std::vector<size_t>& concentrated) {
    auto out = std::vector<size_t>{};
    auto head = concentrated.begin();
    while (head != concentrated.end()) {
        const auto starter = *head;
        out.push_back(starter);
        ++head;
        for (size_t i = 1; i <= *head; ++i) {
            out.push_back(starter + i);
        }
        ++head;
    }
    return out;
}

int main() {
    // notes: each stage is always in order
    auto file_handle = FileHandle("input.txt");

    const std::function<std::vector<std::pair<size_t,size_t>>(std::string)> fn_seed = [](const std::string& line) {
        const auto v = lineToSizetVector(line.substr(7));
        auto out = std::vector<std::pair<size_t,size_t>>{};
        for (size_t i = 0; i < v.size(); i+=2) {
            out.emplace_back(v[i],v[i+1]);
        }
        return out;
    };
    const auto seeds = partialUntilNewLine(file_handle, fn_seed)[0];

    const std::function<std::vector<size_t>(std::string)> fn = [](const std::string& line) {
        if (line.substr(0, 1).find_first_of("0123456789") != std::string::npos) {
            return lineToSizetVector(line);
        }
        return std::vector<size_t>{};
    };
    auto master_store = std::vector<std::vector<mapper>>{};
    while (! file_handle.eof()) {
        auto number_rows = partialUntilNewLine(file_handle, fn);
        number_rows.erase(number_rows.begin());

        auto tmp = std::vector<mapper>{};
        for (auto& res : number_rows) {
            tmp.emplace_back(res[1], res[2], res[0]);
        }
        master_store.push_back(tmp);
    }
    
    std::for_each(std::execution::par_unseq,seeds.begin(), seeds.end(),[master_store](const auto& pair) {
        size_t abs_min = std::numeric_limits<size_t>::max();

        const auto [start,range] = pair;
        for (size_t r = start; r < start+range; ++r) {
            auto current = r;
            for (const auto& mappers : master_store) {
                for (const auto& mapper : mappers) {
                    if (const auto out = mapper.map(current); out != current) {
                        current = out;
                        break;
                    }
                }
            }
            abs_min = current < abs_min ? current : abs_min;
        }
        std::cout << "---abs min=" << abs_min << " \n";
    });

}
