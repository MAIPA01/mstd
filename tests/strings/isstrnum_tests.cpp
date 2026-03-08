#include <mstd/isstrnum.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(StrValidatorTest, IsHex) {
        EXPECT_TRUE(isstrhex("0x1234567890ABCDEF"));
        EXPECT_TRUE(isstrhex("0xabcdef"));
        EXPECT_TRUE(isstrhex("0x0"));

        EXPECT_FALSE(isstrhex("123"));
        EXPECT_FALSE(isstrhex("0x"));
        EXPECT_FALSE(isstrhex("0xG"));
        EXPECT_FALSE(isstrhex("0X123"));
    }

    TEST(StrValidatorTest, IsOctal) {
        EXPECT_TRUE(isstroct("0c01234567"));
        EXPECT_TRUE(isstroct("0c77"));

        EXPECT_FALSE(isstroct("0c8"));
        EXPECT_FALSE(isstroct("0123"));
        EXPECT_FALSE(isstroct("0c"));
    }

    TEST(StrValidatorTest, IsBinary) {
        EXPECT_TRUE(isstrbin("0b101010"));
        EXPECT_TRUE(isstrbin("0b0"));

        EXPECT_FALSE(isstrbin("0b12"));
        EXPECT_FALSE(isstrbin("0b"));
    }

    TEST(StrValidatorTest, IsInteger) {
        EXPECT_TRUE(isstrnum("12345"));
        EXPECT_TRUE(isstrnum("-500"));
        EXPECT_TRUE(isstrnum("++--10"));
        EXPECT_TRUE(isstrnum("0x1A"));
        EXPECT_TRUE(isstrnum("0b101"));
        EXPECT_TRUE(isstrnum("0c17"));

        EXPECT_FALSE(isstrnum(""));
        EXPECT_FALSE(isstrnum("-"));
        EXPECT_FALSE(isstrnum("12.3"));
        EXPECT_FALSE(isstrnum("123a"));
    }

    TEST(StrValidatorTest, IsUnsignedInteger) {
        EXPECT_TRUE(isstrunum("123"));
        EXPECT_TRUE(isstrunum("+++42"));
        EXPECT_TRUE(isstrunum("0xFF"));

        EXPECT_FALSE(isstrunum("-123"));
        EXPECT_FALSE(isstrunum("+"));
    }

    TEST(StrValidatorTest, IsFloatingPoint) {
        EXPECT_TRUE(isstrfp("123.45"));
        EXPECT_TRUE(isstrfp("-0.001"));
        EXPECT_TRUE(isstrfp("---10.0"));
        EXPECT_TRUE(isstrfp("100"));
        EXPECT_FALSE(isstrfp("+5."));

        EXPECT_TRUE(isstrfp("5.0"));
        EXPECT_FALSE(isstrfp("5."));
        EXPECT_TRUE(isstrfp(".5"));
        EXPECT_FALSE(isstrfp("12.34.56"));
    }
}