#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int> readAndConvert(const std::string& file_path) {
    std::ifstream file;
    file.open(file_path);

    auto out = std::vector<int>{};
    while (!file.eof()) {
        auto line = std::string{};
        std::getline(file, line);
        auto conversions = std::string{};
        for (const auto letter : line) {
            try {
                std::stoi(&letter);
                conversions+=letter;
            }
            catch (...) {}
        }
        if (conversions.size() == 1) {
            const auto num = std::string{}+ conversions.front() + conversions.front();
            out.push_back(std::stoi(num));
        }
        else if (conversions.size() > 1) {
            const auto num = std::string{}+ conversions.front() + conversions.back();
            out.push_back(std::stoi(num));
        }
    }
    return out;
}

int main(int argc, char* argv[]) {
    if (argc!=2){
        throw std::exception("Only argument allowed is a input file")
    }
    const std::vector<int> v = readAndConvert(argv[1]);
    for (auto value : v) {
        std::cout << value << "\n";
    }
    std::cout << std::accumulate(v.begin(),v.end(),0);
    return 0;
}
