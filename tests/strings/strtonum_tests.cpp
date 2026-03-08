#include <mstd/strtonum.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(StrNumTest, HexadecimalParsing) {
        int32_t val = 0;
        EXPECT_TRUE(strtonum("0x1F", val));
        EXPECT_EQ(val, 31);

        uint16_t uval = 0;
        EXPECT_FALSE(strtounum("0xFFFF", uval));
        EXPECT_EQ(uval, 0);

        EXPECT_TRUE(strtonum("0xabc", val));
        EXPECT_EQ(val, 2748);
    }

    TEST(StrNumTest, OctalParsing) {
        int32_t val = 0;
        EXPECT_TRUE(strtonum("0c10", val));
        EXPECT_EQ(val, 8);

        EXPECT_TRUE(strtonum("0c77", val));
        EXPECT_EQ(val, 63);
    }

    TEST(StrNumTest, BinaryParsing) {
        uint8_t val = 0;
        EXPECT_TRUE(strtounum("0b1010", val));
        EXPECT_EQ(val, 10);

        EXPECT_TRUE(strtounum("0b11111111", val));
        EXPECT_EQ(val, 255);
    }

    TEST(StrNumTest, DecimalWithSigns) {
        int32_t val = 0;
        EXPECT_TRUE(strtonum("--10", val));
        EXPECT_EQ(val, 10);

        EXPECT_TRUE(strtonum("-+-5", val));
        EXPECT_EQ(val, 5);

        EXPECT_TRUE(strtonum("---5", val));
        EXPECT_EQ(val, -5);
    }

    TEST(StrNumTest, OverflowProtection) {
        uint8_t val = 0;
        EXPECT_FALSE(strtounum("256", val));
        EXPECT_FALSE(strtounum("0b100000000", val));

        int8_t sval = 0;
        EXPECT_FALSE(strtonum("0xFF", sval));
    }

    TEST(StrFPTest, BasicFloatParsing) {
        double val = 0.0;
        EXPECT_TRUE(strtofp("123.45", val));
        EXPECT_NEAR(val, 123.45, 0.0001);

        float fval = 0.0f;
        EXPECT_TRUE(strtofp("-0.001", fval));
        EXPECT_NEAR(fval, -0.001f, 0.000001f);
    }

    TEST(StrFPTest, MultipleSignsFloat) {
        double val = 0.0;
        EXPECT_TRUE(strtofp("--1.5", val));
        EXPECT_EQ(val, 1.5);
    }

    TEST(StrFPTest, InvalidFormats) {
        double val = 0.0;
        int32_t ival = 0;

        EXPECT_FALSE(strtofp(".", val));
        EXPECT_FALSE(strtonum("0x", ival));
        EXPECT_FALSE(strtonum("abc", ival));
        EXPECT_FALSE(strtonum("", ival));
    }
}