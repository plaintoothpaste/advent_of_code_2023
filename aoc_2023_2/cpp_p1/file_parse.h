#pragma once
#include <functional>

template <typename T>
std::vector<T> parse(const std::string& file_path, std::function<T(std::string)> parse_fn) {
    std::ifstream file;
    file.open(file_path);

    auto out = std::vector<T>{};
    while (!file.eof()) {
        auto line = std::string{};
        std::getline(file, line);
        out.emplace_back(parse_fn(line));
    }
    return out;
}
