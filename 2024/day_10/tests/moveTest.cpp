#include <gtest/gtest.h>
#include "models.h"
#include "process.h"

template<typename T>
void expectArrayEqual(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    EXPECT_EQ(lhs.size(), rhs.size()) << "LHS and RHS are not the same size";

    for (auto idx = size_t{0}; idx < lhs.size(); idx++) {
        SCOPED_TRACE(idx); // write to the console in which iteration the error occurred
        EXPECT_EQ(lhs[idx], rhs[idx]);
    }
}

TEST(basicTest, validNeighbors) {
	const auto input = field_t{{0,1,2},{2,0,1}};
	const auto expected = pair_t{{2,1}, {1,0}};
	const auto actual = process::validNeighbors(input, {1,1});
    expectArrayEqual(actual, expected);
}

TEST(basicTest, validNeighborX) {
	const auto input = field_t{{10,10,10},{10,0,1}, {10,10,10}};
	const auto expected = pair_t{ {2,1}};
	const auto actual = process::validNeighbors(input, {1,1});
    expectArrayEqual(actual, expected);
}

TEST(basicTest, validNeighborY) {
	const auto input = field_t{{10,1,10},{10,0,10}, {10,10,10}};
	const auto expected = pair_t{ {1,0}};
	const auto actual = process::validNeighbors(input, {1,1});
    expectArrayEqual(actual, expected);
}

TEST(basicTest, validNeighborNegX) {
	const auto input = field_t{{10,10,10},{1,0,10}, {10,10,10}};
	const auto expected = pair_t{ {0,1}};
	const auto actual = process::validNeighbors(input, {1,1});
    expectArrayEqual(actual, expected);
}

TEST(basicTest, validNeighborNegY) {
	const auto input = field_t{{10,10,10},{10,0,10}, {10,1,10}};
	const auto expected = pair_t{ {1,2}};
	const auto actual = process::validNeighbors(input, {1,1});
    expectArrayEqual(actual, expected);
}

TEST(basicTest, pather) {
    const auto input = field_t{
	    {10,10,9,0,10,10,9},
        {10,10,10,1,10,9,8},
        {10,10,10,2,10,10,7},
        {6,5,4,3,4,5,6},
        {7,6,5,10,9,8,7},
        {8,7,6,10,10,10,10},
        {9,8,7,10,10,10,10}
    };
    EXPECT_EQ(process::completePath(input,{3,0}),4);
}