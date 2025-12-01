#include <gtest/gtest.h>

#include "fileParse.h"

template<typename T>
void expectArrayEqual(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    EXPECT_EQ(lhs.size(), rhs.size()) << "LHS and RHS are not the same size";

    for (int idx = 0; idx < lhs.size(); idx++) {
        SCOPED_TRACE(idx); // write to the console in which iteration the error occurred
        ASSERT_EQ(lhs[idx], rhs[idx]);
    }
}

TEST(basicTest, findZero) {
    const auto input = field_t{{0,1},{2,0}};
    const auto expected = pair_t{{0,0},{1,1}};
    const auto actual = parsing::findZero(input);
    expectArrayEqual(expected,actual);
}

