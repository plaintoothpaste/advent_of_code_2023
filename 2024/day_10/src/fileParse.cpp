#include "fileParse.h"

#include <filesystem>
#include <iostream>

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

#ifdef debug
void printer(const auto c){
    std::cout << c;
}
#endif
#ifndef debug
void printer(const auto c){
    return;
}
#endif



field_t read(FileHandle& fid) {
    auto out = field_t{};
    while(!fid.eof()) {
        out.emplace_back();
        const auto line = fid.getLine();
        out.back().reserve(line.size());
        for (const auto c : line) {
            const auto cc = std::string{c};
            printer(cc);
            out.back().emplace_back(std::stoul(cc));
        }
    }
    return out;
}


pair_t findZero(const field_t& field) {
    auto out = pair_t{};
    for (auto y_i = size_t{0}; y_i < field.size(); ++y_i) {
		for (auto x_i = size_t{0}; x_i < field[y_i].size(); ++x_i) {
			if (field[y_i][x_i]==0) {
	            out.emplace_back(x_i,y_i);
            }
		}
	}
	return out;
}
}
