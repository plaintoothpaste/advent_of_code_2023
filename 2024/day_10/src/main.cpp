#include <iostream>

#include "fileParse.h"
#include "process.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto f = parsing::FileHandle(argv[1]);
    const auto field = parsing::read(f);
    const auto zeros = parsing::findZero(field);
    int summer = 0;
	for (const auto zero : zeros) {
	    summer += process::completePath(field, zero);
    }
    std::cout << summer << "\n";
    return 0;
}
