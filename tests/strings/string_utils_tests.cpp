#include <gtest/gtest.h>
#include <mstd/string_types.hpp>

namespace mstd::test {
	TEST(StringInfoTest, SingleChar) {
		char c = 'A';
		EXPECT_EQ(utils::string_size(c), 1);

		unsigned char uc = 'B';
		EXPECT_EQ(utils::string_size(uc), 1);
	}

	TEST(StringInfoTest, StringLiteral) {
		EXPECT_EQ(utils::string_size("Hello"), 5);

		char arr[] = "Test";
		EXPECT_EQ(utils::string_size(arr), 4);
	}

	TEST(StringInfoTest, CStylePointer) {
		const char* ptr = "Dynamic";
		EXPECT_EQ(utils::string_size(ptr), 7);

		const char* null_ptr = nullptr;
		EXPECT_EQ(utils::string_size(null_ptr), 0);
	}

	TEST(StringInfoTest, StdString) {
		std::string s = "C++ Standard String";
		EXPECT_EQ(utils::string_size(s), 19);

		EXPECT_EQ(utils::string_size(std::string("Temporary")), 9);
	}

	TEST(StringInfoTest, ConstVolatility) {
		const volatile char c = 'X';
		EXPECT_EQ(utils::string_size(c), 1);

		const std::string cs = "ConstString";
		EXPECT_EQ(utils::string_size(cs), 11);
	}

	TEST(StringInfoTest, CompileTimeOptimization) {
		static_assert(utils::string_size("ConstexprTest") == 13, "string_size should be constexpr for literals");
	}
} // namespace mstd::test