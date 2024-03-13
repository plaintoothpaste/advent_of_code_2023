#pragma once
#include <fstream>
#include <functional>
#include <string>
#include <vector>

class FileHandle {
    std::ifstream file;

public:
    explicit FileHandle(const std::string& file_path) {
        file.open(file_path);
    }
    ~FileHandle() {
        file.close();
    }
    std::string getLine() {
        auto line = std::string{};
        std::getline(file, line);
        return line;
    }

    [[nodiscard]] bool eof() const {
        return file.eof();
    }
};


template<typename T>
std::vector<T> parse(FileHandle& f, std::function<T(std::string)> parse_fn) {
    auto out = std::vector<T>{};
    while (! f.eof()) {
        auto line = f.getLine();
        out.push_back(parse_fn(line));
    }
    return out;
}


template<typename T>
T parseSingleLine(FileHandle& f, std::function<T(std::string)> parse_fn) {
    auto line = f.getLine();
    return parse_fn(line);
}

template<typename T>
std::vector<T> partialUntilNewLine(FileHandle& f, std::function<T(std::string)> parse_fn) {
    auto out = std::vector<T>{};
    while (! f.eof()) {
        auto line = f.getLine();
        if (line.empty() || line == "\n") {
            break;
        }
        out.push_back(parse_fn(line));
    }
    return out;
}

/// read a file into lines and then group lines by every empty line in the file
std::vector<std::vector<std::string>> readFileByParagraph(FileHandle& f) {
    auto out = std::vector<std::vector<std::string>>{};
    out.push_back({});
    while (! f.eof()) {
        const auto line = f.getLine();
        if (line.empty() || line == "\n") {
            out.back() = {};
        } else {
            out.back().push_back(line);
        }
    }
    return out;
}
/**
 * \brief The common operation of parsing a line of into a vec of int
 */
std::vector<int> lineToIntVector(std::string line, const std::string delimiter = " ") {
    auto out = std::vector<int>{};
    size_t pos = 0;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        const std::string token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        if (! token.empty()) { // if there is a double delimiter this will happen
            out.push_back(std::stoi(token));
        }
    }
    if (! line.empty()) {
        out.push_back(std::stoi(line));
    }
    return out;
}
/**
 * \brief The common operation of parsing a line of into a vec of size_t
 */
std::vector<size_t> lineToSizetVector(std::string line) {
    auto out = std::vector<size_t>{};
    const auto delimiter = std::string{ " " };
    size_t pos = 0;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        const std::string token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        if (! token.empty()) { // if there is a double delimiter this will happen
            out.push_back(std::stoll(token));
        }
    }
    if (! line.empty()) {
        out.push_back(std::stoll(line));
    }
    return out;
}
