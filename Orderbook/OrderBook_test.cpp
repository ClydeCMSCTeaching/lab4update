//
// Created by aclyde on 8/17/21.
//

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(OBTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 423);
}