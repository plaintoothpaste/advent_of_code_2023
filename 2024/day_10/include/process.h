#pragma once
#include "models.h"
#include <vector>


namespace process
{
pair_t validNeighbors(const field_t& f, const pair_single_t& loc);

int completePath(const field_t& f, const pair_single_t& loc);

int allPaths(const field_t& f, const pair_single_t& loc);
}
