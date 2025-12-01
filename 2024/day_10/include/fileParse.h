#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "models.h"

namespace parsing {
class FileHandle {
    std::ifstream file_;

public:
    explicit FileHandle(const std::filesystem::path& file_path);

    ~FileHandle() {
        file_.close(); // not required since ifsteam closes files automatically 
    }

    FileHandle(const FileHandle& other)=delete;
    FileHandle& operator=(const FileHandle& other)=delete;

    std::string getLine();

    [[nodiscard]] bool eof() const {
        return file_.eof();
    }
};

field_t read(FileHandle& fid);

pair_t findZero(const field_t& field);
} // namespace fileParse

