#include "fileParse.h"

namespace parsing {
FileHandle::FileHandle(const std::filesystem::path& file_path) {
    file_.open(file_path);
    if (! file_.is_open()) {
        throw std::exception("File does not exist or could not be opened");
    }
}

std::string FileHandle::getLine() {
    auto line = std::string{};
    std::getline(file_, line);
    return line;
}

std::vector<int> lineToInt(std::string&& content) {
    auto out = std::vector<int>{};
    out.reserve(content.size());
    for (const auto c : content)
        out.emplace_back(c - '0');
    return out;
}
}
