#include <gtest/gtest.h>
#include <mstd/bit_operations.hpp>

namespace mstd::test {
	TEST(ARITHMETIC_TESTS_BIT_OPERATIONS, ALL) {
		uint32_t i = 0x0000'000Fu;
		uint32_t j = mstd::swap_endians(i);

		i &= 0x0Fu;
		j &= 0x0F00'0000u;
		j >>= 24u;

		ASSERT_EQ(i, j);
	}
} // namespace mstd::test