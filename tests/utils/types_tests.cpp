#include <mstd/types.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    TEST(TraitsTest, ArithmeticEqualityCorrected) {
#if _MSTD_HAS_CXX20
        constexpr bool should_be_false = is_eq_arithmetic_v<10.5, 10.0, 0.1>;
        EXPECT_FALSE(should_be_false);

        constexpr bool should_be_true = is_eq_arithmetic_v<10.05, 10.0, 0.1>;
        EXPECT_TRUE(should_be_true);
#else
        constexpr bool should_be_false = is_eq_arithmetic_v<10, 9>;
        EXPECT_FALSE(should_be_false);

        constexpr bool should_be_true = is_eq_arithmetic_v<10, 10>;
        EXPECT_TRUE(should_be_true);
#endif
    }

    TEST(TraitsTest, IsInEmptyList) {
        constexpr bool in_empty = is_same_type_in_v<int, float>;
        EXPECT_FALSE(in_empty);

        constexpr bool val_in_empty = is_eq_value_in_v<10, 1>;
        EXPECT_FALSE(val_in_empty);
    }

    TEST(TraitsTest, UniqueTypesFinal) {
        using TestList = unique_types<int, float, int, char, float, int>;

        static_assert(TestList::types_num == 3, "Only 3 types should be left");

        bool is_valid = std::is_same_v<TestList, types_holder<int, float, char>>;
        EXPECT_TRUE(is_valid);
        is_valid = std::is_same_v<TestList::as_tuple, std::tuple<int, float, char>>;
        EXPECT_TRUE(is_valid);
    }

    static_assert(is_in_range_v<5, 0, 10>, "5 is in [0, 10]");
    static_assert(is_in_range_v<0, 0, 10>, "down border");
    static_assert(is_in_range_v<10, 0, 10>, "up border");
    static_assert(!is_in_range_v<-1, 0, 10>, "out of range");

#if _MSTD_HAS_CXX20
    TEST(TraitsTest, Concepts) {
        EXPECT_TRUE(integral<size_t>);
        EXPECT_TRUE(signed_integral<int>);
        EXPECT_FALSE(signed_integral<unsigned int>);
        EXPECT_TRUE(floating_point<long double>);
    }
#endif

TEST(IfVTest, BooleanConditionLogic) {
    	EXPECT_EQ((mstd::if_v<true, 1, 0>), 1);
    	EXPECT_EQ((mstd::if_v<false, 1, 0>), 0);
    }

	TEST(IfVTest, CompileTimeValidation) {
    	static_assert(mstd::if_v<true, 100, 200> == 100, "Should be 100 on true");
    	static_assert(mstd::if_v<false, 100, 200> == 200, "Should be 200 on false");

    	SUCCEED();
    }

	TEST(IfVTest, DifferentAutoTypes) {
    	static_assert(mstd::if_v<true, 'a', 'b'> == 'a');
    	static_assert(mstd::if_v<false, 3.14, 2.71> == 2.71);

    	enum class Status { Ok, Error };
    	static_assert(mstd::if_v<true, Status::Ok, Status::Error> == Status::Ok);
    }

	TEST(IfVTest, ConstantExpressionsAsCondition) {
    	constexpr int x = 10;
    	constexpr int y = 20;

    	static_assert(mstd::if_v<(x < y), 1, 0> == 1);
    	static_assert(!mstd::if_v<(sizeof(int) > 100), true, false>);
    }

	TEST(IfVTest, PointerValues) {
    	static int a = 5;
    	static float b = 10;

    	const auto result = mstd::if_v<true, &a, &b>;
    	EXPECT_EQ(result, &a);
    }
}