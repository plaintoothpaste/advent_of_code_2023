#include "process.h"

#include <algorithm>

namespace process
{
	pair_t validNeighbors(const field_t& f, const pair_single_t& loc) {
		auto n = pair_t{};
		const auto val = f[loc.second][loc.first];
		if (loc.first>0 && f[loc.second][loc.first-1]==val+1)
			n.emplace_back(loc.first-1,loc.second);

		if (loc.first<(f[loc.second].size()-1) && f[loc.second][loc.first+1]==val+1)
			n.emplace_back(loc.first+1,loc.second);

		if (loc.second>0 && f[loc.second-1][loc.first]==val+1)
			n.emplace_back(loc.first,loc.second-1);

		if (loc.second<(f.size()-1) && f[loc.second+1][loc.first]==val+1)
			n.emplace_back(loc.first,loc.second+1);

		return n;
	}

	static void _completePath(const field_t& f, const pair_single_t& loc, pair_t& reached) {
		if (f[loc.second][loc.first]==9) {
			reached.emplace_back(loc.first,loc.second);
			return;
		}
		for (const auto n : validNeighbors(f, loc)) {
			_completePath(f, n, reached);
		}
	}

	int completePath(const field_t& f, const pair_single_t& loc) {
		auto reached = pair_t{};
		_completePath(f,loc, reached);

		std::ranges::sort(reached);
		const auto bad_range = std::ranges::unique(reached);

		// [0,0,1,2,3,4,4]
		// [0,1,2,3,4,x,x]
		//            ^   ^
		// ranges.erase(bad_range.begin(),bad_range.end());

		// ranges.begin(),bad_range.begin()
		return static_cast<int>(reached.size() - bad_range.size());
	}

	int allPaths(const field_t& f, const pair_single_t& loc) {
		if (f[loc.second][loc.first]==9) {
			return 1;
		}
		auto total = 0;
		for (const auto n : validNeighbors(f, loc)) {
			total += allPaths(f, n);
		}
		return total;
	}
}
