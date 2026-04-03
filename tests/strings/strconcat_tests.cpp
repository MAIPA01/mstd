#include <gtest/gtest.h>
#include <pch.hpp>

namespace mstd::test {
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
		char arr[]		   = "Array";
		const char* ptr	   = "Pointer";
		std::string str	   = "StdString";
		char c			   = '!';

		std::string result = concat(arr, " ", ptr, " ", str, c);
		EXPECT_EQ(result, "Array Pointer StdString!");
	}
} // namespace mstd::test