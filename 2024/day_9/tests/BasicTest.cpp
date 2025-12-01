#include <gtest/gtest.h>
#include "compactor.h"

template<typename T>
concept integral = std::is_integral_v<T>;

template<typename T>
    requires integral<T>
// requires std::is_integral_v<T>
void expectArrayEqual(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    EXPECT_EQ(lhs.size(), rhs.size()) << "LHS and RHS are not the same size";

    for (int idx = 0; idx < lhs.size(); idx++) {
        SCOPED_TRACE(idx); // write to the console in which iteration the error occurred
        ASSERT_EQ(lhs[idx], rhs[idx]);
    }
}

TEST(BaselineTests, basicExpansionTest) {
    const auto input = std::vector<int>{ 2, 3, 3, 3, 1 };
    const auto expected = std::string("00...111...2");
    const auto actual = Compactor(input).toString();
    EXPECT_EQ(expected, actual);
}
TEST(BaselineTests, realExpansionTest) {
    const auto input = std::vector<int>{ 2, 3, 3, 3, 1, 3, 3, 1, 2, 1, 4, 1, 4, 1, 3, 1, 4, 0, 2 };
    const auto expected = std::string("00...111...2...333.44.5555.6666.777.888899");
    const auto actual = Compactor(input).toString();
    EXPECT_EQ(expected, actual);
}

TEST(BaselineTests, basicNextStep) {
    auto input = Compactor({ 2, 3, 3, 3, 1 });
    EXPECT_EQ(input.toString(), std::string("00...111...2"));
    input.next();
    EXPECT_EQ(input.toString(), std::string("002..111...."));
        input.next();
    EXPECT_EQ(input.toString(), std::string("0021.11....."));
}

TEST(BaselineTests, basicChecksum) {
    auto input = Compactor({ 2, 3, 3, 3, 1 });
    EXPECT_EQ(input.toString(), std::string("00...111...2"));
    input.next();
    EXPECT_EQ(input.toString(), std::string("002..111...."));
    EXPECT_EQ(input.checksum(),4);
}