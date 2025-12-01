#include "fileParse.h"

#include <cassert>
#include <filesystem>
#include <iostream>

namespace parsing {
FileHandle::FileHandle(const std::filesystem::path& file_path) {
    file_.open(file_path);
    if (! file_.is_open()) {
        throw std::exception("File does not exist or could not be opened");
    }
}

std::string FileHandle::getLine(std::string delim) {
    auto line = std::string{};
    if (delim.empty()) {
	    std::getline(file_, line);
		return line;
    }
    assert(delim.size()==1);
    std::getline(file_, line, delim.back());
		return line;
}

stones_t read(FileHandle& fid) {
    auto out = stones_t{};
    while (!fid.eof()) {
	    const auto token = fid.getLine(" ");
        out.emplace_back(std::stoul(token));
    }
    return out;
}

}
