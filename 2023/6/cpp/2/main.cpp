#include <array>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include "enumerate.h"
#include "file_parse.h"

// count how many different race options will beat the current record
int count(double time, double record) {
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
        if (delta < 1e-5 && delta >= 0) {
            --i_high;
        }
    }
    return std::abs(i_high - i_low);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);

    const std::function<size_t(std::string)> fn = [](const std::string& line) {
        const auto allowed = std::string{"0123456789"};
        auto nums = std::string{};
        for (auto c : line) {
            if (allowed.find_first_of(c)!=std::string::npos) {
                nums.push_back(c);
            }

        }
        return std::stoll(nums);
    };

    const auto time = fileParse::parseSingleLine(file_handle, fn);
    const auto record = fileParse::parseSingleLine(file_handle, fn);

    const auto better_count = count(static_cast<double>(time), static_cast<double>(record));
    std::cout << "better=" << better_count << " \n";
}
