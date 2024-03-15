#include <file_parse.h>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int convertLine(const std::string& line) {
    auto conversions = std::string{};
    for (const auto letter : line) {
        try {
            const auto _ = std::stoi(&letter);
            conversions += letter;
        } catch (...) {
        }
    }
    switch (conversions.size()) {
    case 0:
        throw std::exception("line found without numbers");
    case 1:
        return std::stoi(std::string{} + conversions.front() + conversions.front());
    case 2:
        return std::stoi(conversions);
    default:
        return std::stoi(std::string{} + conversions.front() + conversions.back());
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto file_handle = fileParse::FileHandle(argv[1]);
    const std::function<int(std::string)> fn = [](const std::string& l) { return convertLine(l); };

    const std::vector<int> v = fileParse::parse(file_handle, fn);
    std::cout << std::accumulate(v.begin(), v.end(), 0);
    return 0;
}
