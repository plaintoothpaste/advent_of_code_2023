#pragma once
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include <filesystem>
		

namespace parsing {
class FileHandle {
    std::ifstream file_;

public:
    explicit FileHandle(const std::filesystem::path& file_path);

    ~FileHandle() {
        file_.close(); // not required since ifsteam closes files automatically 
    }
    std::string getLine();

    [[nodiscard]] bool eof() const {
        return file_.eof();
    }
};


std::vector<int> lineToInt(std::string&& content);
} // namespace fileParse

