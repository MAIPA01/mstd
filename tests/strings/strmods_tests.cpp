#include <mstd/strmods.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(StringModifyTest, TrimBasic) {
        EXPECT_EQ(trim("  hello  "), "hello");
        EXPECT_EQ(trim("\t\n world \r"), "world");
    }

    TEST(StringModifyTest, TrimNoSpaces) {
        EXPECT_EQ(trim("no_spaces"), "no_spaces");
    }

    TEST(StringModifyTest, TrimEmptyOrOnlySpaces) {
        EXPECT_EQ(trim("   "), "");
        EXPECT_EQ(trim(""), "");
    }

    TEST(StringModifyTest, TrimOnlyStartOrEnd) {
        EXPECT_EQ(trim("left"), "left");
        EXPECT_EQ(trim("  left"), "left");
        EXPECT_EQ(trim("right  "), "right");
    }
}