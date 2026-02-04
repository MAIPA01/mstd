#include <mstd/string.hpp>
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

    TEST(StringInfoTest, SingleChar) {
        char c = 'A';
        EXPECT_EQ(string_size(c), 1);

        unsigned char uc = 'B';
        EXPECT_EQ(string_size(uc), 1);
    }

    TEST(StringInfoTest, StringLiteral) {
        EXPECT_EQ(string_size("Hello"), 5);

        char arr[] = "Test";
        EXPECT_EQ(string_size(arr), 4);
    }

    TEST(StringInfoTest, CStylePointer) {
        const char* ptr = "Dynamic";
        EXPECT_EQ(string_size(ptr), 7);

        const char* null_ptr = nullptr;
        EXPECT_EQ(string_size(null_ptr), 0);
    }

    TEST(StringInfoTest, StdString) {
        std::string s = "C++ Standard String";
        EXPECT_EQ(string_size(s), 19);

        EXPECT_EQ(string_size(std::string("Temporary")), 9);
    }

    TEST(StringInfoTest, ConstVolatility) {
        const char volatile c = 'X';
        EXPECT_EQ(string_size(c), 1);

        const std::string cs = "ConstString";
        EXPECT_EQ(string_size(cs), 11);
    }

    TEST(StringInfoTest, CompileTimeOptimization) {
        static_assert(string_size("ConstexprTest") == 13, "string_size should be constexpr for literals");
    }

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

    TEST(StringConcatTest, ConcatToExistingString) {
        std::string base = "Start: ";
        concat_to(base, std::string("Hello"), " ", 'A', "!");

        EXPECT_EQ(base, "Start: Hello A!");
    }

    TEST(StringConcatTest, ConcatNewString) {
        std::string result = concat("C++", " ", "Fold", " ", "Expressions");
        EXPECT_EQ(result, "C++ Fold Expressions");
    }

    TEST(StringConcatTest, ConcatEfficiency) {
        std::string out;
        const char* part = "MeasureMe";

        concat_to(out, part, part, part);

        EXPECT_EQ(out, "MeasureMeMeasureMeMeasureMe");
        EXPECT_GE(out.capacity(), 27);
    }

    TEST(StringConcatTest, MixedTypes) {
        char arr[] = "Array";
        const char* ptr = "Pointer";
        std::string str = "StdString";
        char c = '!';

        std::string result = concat(arr, " ", ptr, " ", str, c);
        EXPECT_EQ(result, "Array Pointer StdString!");
    }

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