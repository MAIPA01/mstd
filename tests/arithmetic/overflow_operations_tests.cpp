#include <mstd/overflow_operations.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
	TEST(ARITHMETIC_TESTS_OVERFLOW_OPERATIONS, ALL) {
		uint8_t a = 2;
		uint8_t b = 3;
		uint8_t c = 0;

		// add
		ASSERT_FALSE(mstd::add_overflow(a, b, c));
		a = 255;
		ASSERT_TRUE(mstd::add_overflow(a, b, c));

		// mul
		b = 1;
		ASSERT_FALSE(mstd::mul_overflow(a, b, c));
		b = 2;
		ASSERT_TRUE(mstd::mul_overflow(a, b, c));

		// sub
		ASSERT_FALSE(mstd::sub_overflow(a, b, c));
		a = 0;
		ASSERT_TRUE(mstd::sub_overflow(a, b, c));

		// div
		int8_t ai = -128;
		int8_t bi = 1;
		int8_t ci = 0;
		ASSERT_FALSE(mstd::div_overflow(ai, bi, ci));
		bi = -1;
		ASSERT_TRUE(mstd::div_overflow(ai, bi, ci));
	}
}