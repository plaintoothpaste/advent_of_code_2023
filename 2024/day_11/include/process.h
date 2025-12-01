#pragma once
#include "models.h"


namespace process
{
size_t lengthEstimate(const stones_t& input);

bool shouldSplit(const size_t& input);
std::pair<size_t, size_t> split(const size_t& input);
void applyRules(stones_t& stones);
}
