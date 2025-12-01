#include "process.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace process
{
	size_t lengthEstimate(const stones_t& input) {
		return static_cast<size_t>(pow(2, 25 + static_cast<double>(input.size())) / 2);
	}

	size_t numberDigits(const size_t& input) {
		return static_cast<size_t>(std::log10(input)) + 1;
	}

	bool shouldSplit(const size_t& input)
	{
		return numberDigits(input) % 2 == 0;
	}

	std::pair<size_t, size_t> split(const size_t& input) {
		const auto digits = numberDigits(input) / 2;
		const auto rep = std::to_string(input);
		return { std::stoll(rep.substr(0,digits)), std::stoll(rep.substr(digits,digits)) };
	}

	void applyRules(stones_t& stones) {
		auto index = size_t{0};
		while (index < stones.size()) {
			auto& val = stones[index];

			if (val == 0) {
				val = 1;
			}
			else if (shouldSplit(val)) {
				const auto [lhs, rhs] = split(val);
				val = lhs;
				stones.emplace_back(rhs);
				index++;
				std::swap(stones[index], stones.back());
			}
			else {
				val *= 2024;
			}
			index++;
		}
	}
}
