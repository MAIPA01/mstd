#include <mstd/math_functions.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
	TEST(ARITHMETIC_TESTS_MATH_FUNCTIONS, ALL) {
		// signum
		ASSERT_EQ(mstd::signum(0), 0);
		ASSERT_EQ(mstd::signum(1), 1);
		ASSERT_EQ(mstd::signum(-1), -1);

		ASSERT_EQ(mstd::signum(0u), 0);
		ASSERT_EQ(mstd::signum(1u), 1);

		ASSERT_FLOAT_EQ(mstd::signum(0.f), 0.f);
		ASSERT_FLOAT_EQ(mstd::signum(1.f), 1.f);
		ASSERT_FLOAT_EQ(mstd::signum(-1.f), -1.f);

		// step
		ASSERT_EQ(mstd::step(1, 2), 1);
		ASSERT_EQ(mstd::step(1, 0), 0);

		// remap
		ASSERT_FLOAT_EQ(mstd::remap(0.5f, 0.f, 1.f, -1.f, 1.f), 0.f);

		// deg_to_rad
		ASSERT_DOUBLE_EQ(mstd::deg_to_rad(90.0), M_PI_2);

		// rad_to_deg
		ASSERT_DOUBLE_EQ(mstd::rad_to_deg(M_PI_2), 90.0);

		// epsilon_eq
		ASSERT_TRUE(mstd::epsilon_equal(90.f, 90.1f, 0.1f));

		// saturate
		ASSERT_EQ(mstd::saturate(2), 1);

		// fract
		ASSERT_EQ(mstd::fract(1), 0);
		ASSERT_FLOAT_EQ(mstd::fract(1.2f), 0.2f);

		// Q_rstqr
		ASSERT_FLOAT_EQ(mstd::Q_rsqrt(2.f), 0.70693f);
	}
}