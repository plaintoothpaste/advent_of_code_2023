#include <iostream>

#include "fileParse.h"
#include "process.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Only argument allowed is a input file";
		return -1;
	}
	auto f = parsing::FileHandle(argv[1]);
	auto stones = parsing::read(f);
	stones.reserve(process::lengthEstimate(stones));

	for (size_t _ = 0; _ < 25; ++_) {
		process::applyRules(stones);
	}
	std::cout << stones.size() << "\n";
	return 0;
}
