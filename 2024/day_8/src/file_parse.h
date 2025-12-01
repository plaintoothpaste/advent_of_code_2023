#pragma once
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <filesystem>
		

namespace fileParse {
class FileHandle {
    std::ifstream file;

public:
    explicit FileHandle(const std::filesystem::path& file_path) {
        file.open(file_path);
        if (! file.is_open()) {
            throw std::exception("File does not exist or could not be opened");
        }
    }

    ~FileHandle() {
        file.close(); // not required since ifsteam closes files automatically 
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
} // namespace fileParse

