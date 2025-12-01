#include <gtest/gtest.h>
#include "compactor.h"

TEST(partTwoTests, testSpace) {
    auto input = Compactor2({ 2, 2, 3, 3, 1, 3, 3 });
    EXPECT_EQ(input.toString(), std::string("00..111...2...333"));
    const auto space = std::pair<size_t, size_t>{2,4};
    EXPECT_EQ(input.firstSpace(), space);
    const auto space2 = std::pair<size_t, size_t>{7,10};
    EXPECT_EQ(input.nextSpace(), space2);
}

TEST(partTwoTests, testWord) {
    auto input = Compactor2({ 2, 2, 3, 3, 1, 3, 3 });
    EXPECT_EQ(input.toString(), std::string("00..111...2...333"));
    const auto word = std::pair<size_t, size_t>{14,17};
    EXPECT_EQ(input.word(), word);
    const auto word2 = std::pair<size_t, size_t>{10,11};
    EXPECT_EQ(input.word(), word2);
}