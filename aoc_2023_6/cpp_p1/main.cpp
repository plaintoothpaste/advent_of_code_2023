#include <array>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

// count how many different race options will beat the current record
int count(int time, int record) {
    // distance = n * (time - n) = n * time - n^2
    // all n such that distance>record
    // record <  n * time - n^2
    // 0 < - n^2 + n * time - record
    // 0 > n^2 - n * time + record
    // n ==>   time +- sqrt((time)^2 - 4 * 1 * record)
    //        ----------------------------------------
    //                           2
    // the distance between the two results is the count of n that are faster then record
    // optional faster option by squaring record
    const auto n_low = (time - sqrt(time * time - 4 * record)) / 2;
    const auto n_high = (time + sqrt(time * time - 4 * record)) / 2;

    // check if number is close to round if so +1
    auto i_low = static_cast<int>(n_low);
    /*{
        const auto delta = n_low - static_cast<double>(i_low);
        if (delta < 1e-5 && delta>=0) {
            ++i_low;
        }
    }*/

    auto i_high = static_cast<int>(n_high);
    {
        const auto delta = n_high - static_cast<double>(i_high);
        if (delta < 1e-5 && delta>=0) {
            --i_high;
        }
    }
    return std::abs(i_high - i_low);
}

int main() {
    // notes: each stage is always in order
    auto file_handle = FileHandle("input.txt");


    const std::function<std::vector<size_t>(std::string)> fn_time = [](const std::string& line) { return lineToSizetVector(line.substr(5)); };
    const auto times = parseSingleLine(file_handle, fn_time);

    const std::function<std::vector<size_t>(std::string)> fn_rec = [](const std::string& line) { return lineToSizetVector(line.substr(10)); };
    const auto records = parseSingleLine(file_handle, fn_rec);

    size_t multiple_distances = 1;
    for (const auto& [i, time] : enumerate(times)) {
        const auto better_count = count(static_cast<int>(time), static_cast<int>(records[i]));
        std::cout << "better [" << i << "]=" << better_count << " \n";
        multiple_distances *= static_cast<size_t>(better_count);
    }
    std::cout << "better multiples=" << multiple_distances << " \n";
}
