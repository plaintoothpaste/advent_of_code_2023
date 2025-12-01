#include <gtest/gtest.h>
#include "models.h"
#include "fileParse.h"

template<typename T>
void expectArrayEqual(const std::vector<T>& lhs, const std::vector<T>& rhs) {
	EXPECT_EQ(lhs.size(), rhs.size()) << "LHS and RHS are not the same size";

	for (int idx = 0; idx < lhs.size(); idx++) {
		SCOPED_TRACE(idx); // write to the console in which iteration the error occurred
		ASSERT_EQ(lhs[idx], rhs[idx]);
	}
}

TEST(basicTest, shouldSplit) {
	EXPECT_FALSE(process::shouldSplit(125));
	EXPECT_FALSE(process::shouldSplit(2));
	EXPECT_FALSE(process::shouldSplit(7));
	EXPECT_TRUE(process::shouldSplit(22));
}

TEST(basicTest, basicSplit) {
	const auto expected = std::pair<size_t, size_t>{ 1,7 };
	EXPECT_EQ(expected, process::split(17));
}

TEST(stepTest, firstStep) {
	const auto input = stones_t{ 125, 17 };
	stones_t actual = input;
	const auto expected = stones_t{ 253000, 1, 7 };
	process::applyRules(actual);
	expectArrayEqual(actual, expected);
}
