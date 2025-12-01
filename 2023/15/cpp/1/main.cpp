
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>
#include "file_parse.h"

int hash(const int current, const char letter) {
    const auto letter_as_number = static_cast<int>(static_cast<unsigned int>(letter));
    return (current + letter_as_number) * 17 % 256;
}

int hashWord(const std::string& word) {
    int accumulator = 0;
    for (const auto c : word) {
        accumulator = hash(accumulator, c);
    }
    return accumulator;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const auto data = fileParse::splitByCommer(file_handle);
    auto total = 0;
    for (const auto& d : data) {
        const auto hd = hashWord(d);
        total += hd;
        std::cout << d << "=>" << hd <<"\n";
    }
    std::cout << "total =>" << total;
}
