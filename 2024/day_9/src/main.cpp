#include <iostream>

#include "fileParse.h"
#include "compactor.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    auto f = parsing::FileHandle(argv[1]);
    auto compactor = Compactor(parsing::lineToInt(f.getLine()));
    while (!compactor.fullyCompact()) {
        compactor.next();
        //std::cout << compactor.toString() << "\n";
    }
    std::cout << "final=" << compactor.checksum() << " \n";
    return 0;
}
