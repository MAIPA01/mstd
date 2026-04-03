#include <gtest/gtest.h>
#include <pch.hpp>

enum class TestEnum : uint8_t {
	None = 0,
	Bit1 = 1 << 0, // 1
	Bit2 = 1 << 1, // 2
	Bit3 = 1 << 2, // 4
	All	 = 0xFF
};

using Flags = mstd::flags<TestEnum>;

class FlagsTest : public ::testing::Test {
protected:
	Flags f1 { TestEnum::Bit1 };
	Flags f2 { TestEnum::Bit2 };
	Flags f12 { 3 }; // Bit1 | Bit2
};

TEST_F(FlagsTest, DefaultConstructorInitializesToZero) {
	constexpr Flags empty;
	EXPECT_EQ(empty.value(), 0);
	EXPECT_FALSE(empty);
}

TEST_F(FlagsTest, ConstructorFromEnum) {
	constexpr Flags f(TestEnum::Bit3);
	EXPECT_EQ(f.value(), 4);
}

TEST_F(FlagsTest, ExplicitConstructorFromValue) {
	constexpr Flags f(5); // Bit1 | Bit3
	EXPECT_EQ(f.value(), 5);
}

TEST_F(FlagsTest, BooleanOperators) {
	constexpr Flags empty;
	EXPECT_TRUE(f1);
	EXPECT_FALSE(empty);
	EXPECT_TRUE(!empty);
	EXPECT_FALSE(!f1);
}

TEST_F(FlagsTest, EqualityOperators) {
	EXPECT_EQ(f1, Flags(TestEnum::Bit1));
	EXPECT_NE(f1, f2);
	EXPECT_NE(f1, Flags());
}

TEST_F(FlagsTest, ContainsLogic) {
	using mstd::operator|;
	constexpr Flags complex = TestEnum::Bit1 | TestEnum::Bit2;

	EXPECT_TRUE(complex.contains(TestEnum::Bit1));
	EXPECT_TRUE(complex.contains(TestEnum::Bit2));
	EXPECT_TRUE(complex.contains(complex));

	EXPECT_FALSE(complex.contains(TestEnum::Bit3));
	EXPECT_FALSE(f1.contains(TestEnum::Bit1 | TestEnum::Bit2));
}

TEST_F(FlagsTest, BitwiseOR) {
	const auto combined = f1 | f2;
	EXPECT_EQ(combined.value(), 3);

	Flags f;
	f |= TestEnum::Bit3;
	EXPECT_EQ(f.value(), 4);
}

TEST_F(FlagsTest, BitwiseAND) {
	Flags complex(7); // 1 | 2 | 4
	const auto result = complex & f1;
	EXPECT_EQ(result.value(), 1);

	complex &= f2;
	EXPECT_EQ(complex.value(), 2);
}

TEST_F(FlagsTest, BitwiseXOR) {
	Flags mask(3);				   // Bit1 | Bit2
	const auto result = mask ^ f1; // Bit2
	EXPECT_EQ(result.value(), 2);

	mask ^= mask;
	EXPECT_EQ(mask.value(), 0);
}

TEST_F(FlagsTest, BitwiseNotWithLimits) {
	constexpr Flags val(TestEnum::Bit1); // 0000 0001
	const auto inverted = ~val;			 // 1111 1110 (254)

	EXPECT_EQ(inverted.value(), 0xFE);
	EXPECT_EQ(inverted.value(), std::numeric_limits<uint8_t>::max() ^ 0x01);
}

TEST(FlagsGlobalTest, FriendOperatorOrBetweenEnums) {
	using mstd::operator|;
	// [[nodiscard]] friend flags operator|(const bits_type bit, const bits_type otherBit)
	auto result = TestEnum::Bit1 | TestEnum::Bit3;

	static_assert(std::is_same_v<decltype(result), Flags>, "Should return flags object");
	EXPECT_EQ(result.value(), 5);
	EXPECT_TRUE(result.contains(TestEnum::Bit1));
	EXPECT_TRUE(result.contains(TestEnum::Bit3));
}

TEST_F(FlagsTest, AssignmentFromEnum) {
	Flags f;
	f = TestEnum::Bit2;
	EXPECT_EQ(f.value(), 2);
}

TEST(FlagsConstexprTest, IsCompileTimeReady) {
	constexpr Flags cf = TestEnum::Bit1;
	constexpr auto val = cf.value();
	constexpr bool has = cf.contains(TestEnum::Bit1);

	static_assert(val == 1, "Constexpr value failed");
	static_assert(has, "Constexpr contains failed");
}
