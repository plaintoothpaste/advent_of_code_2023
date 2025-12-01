
#include <iostream>

#include "fileParse.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto f = parsing::FileHandle(argv[1]);
    return 0;
}
