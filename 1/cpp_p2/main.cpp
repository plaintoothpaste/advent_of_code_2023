#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <optional>

const auto matchers_letters = std::vector<std::pair<std::string,std::string>>{
    {"0","zero"},
    {"1","one"},
    {"2","two"},
    {"3","three"},
    {"4","four"},
    {"5","five"},
    {"6","six"},
    {"7","seven"},
    {"8","eight"},
    {"9","nine"},
};


std::pair<std::string, std::string> matchDigitOrLetters(std::pair<std::string, std::string> pair) {
    auto [res, input] = pair;
    if (input.empty()) {
        return pair;
    }
    // as a digit
    try {
        const auto front = input.front();
        std::stoi(&front);
        res += front;
    }
    catch (...) {}

    // as letters
    for (const auto& [i,num] : matchers_letters) {
        if (input.starts_with(num)) {
            //input = input.erase(0,num.size()); // incase second number begins in first number
            res+=i;
            break;
        }
    }
    input = input.erase(0,1);
    return matchDigitOrLetters({ res,input});
}

std::vector<int> readAndConvert(const std::string& file_path) {
    std::ifstream file;
        file.open(file_path);

        auto out = std::vector<int>{};
        while (!file.eof()) {
            auto line = std::string{};
            std::getline(file, line);
            const auto [conversions, _] = matchDigitOrLetters({"",line});

            if (conversions.size() == 1) {
                const auto num = std::string{} + conversions.front() + conversions.front();
                out.push_back(std::stoi(num));
            }
            else if (conversions.size() > 1) {
                const auto num = std::string{} + conversions.front() + conversions.back();
                out.push_back(std::stoi(num));
            }
        }
    return out;
}

int main() {
    const std::vector<int> v = readAndConvert("input.txt");
    for (const auto value : v) {
        std::cout << value << "\n";
    }
    std::cout << std::accumulate(v.begin(), v.end(), 0);
    return 0;
}
