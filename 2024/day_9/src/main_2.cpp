#include <iostream>

#include "fileParse.h"
#include "compactor.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Only argument allowed is a input file";
        return -1;
    }
    const auto start = std::chrono::system_clock::now();
    auto f = parsing::FileHandle(argv[1]);
    auto compactor = Compactor2(parsing::lineToInt(f.getLine()));
    while (!compactor.fullyCompact()) {
        compactor.next();
        std::cout << compactor.toString() << "\n";
    }
    std::cout << "final result=" << compactor.checksum() << " \n";

    const auto delta = std::chrono::system_clock::now() - start;
    std::cout << "Duration [ms]= " << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() << " \n";
    return 0;
}
