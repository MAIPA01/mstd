#include <gtest/gtest.h>
#include <mstd/hash.hpp>
#include <unordered_set>

namespace mstd::test {
	TEST(HashTest, Consistency) {
		std::string s = "test";
		int i		  = 42;

		size_t h1	  = hash_combine(s, i);
		size_t h2	  = hash_combine(s, i);

		EXPECT_EQ(h1, h2);
		EXPECT_NE(h1, 0);
	}

	TEST(HashTest, OrderMatters) {
		size_t h1 = hash_combine(std::string("A"), std::string("B"));
		size_t h2 = hash_combine(std::string("B"), std::string("A"));

		EXPECT_NE(h1, h2);
	}

	TEST(HashTest, VariadicArguments) {
		size_t h2 = hash_combine(1, 2);
		size_t h3 = hash_combine(1, 2, 3);
		size_t h4 = hash_combine(1, 2, 3, 4);

		EXPECT_NE(h2, h3);
		EXPECT_NE(h3, h4);
	}

	TEST(HashTest, RangeHashing) {
		std::vector<int> vec = { 1, 2, 3, 4, 5 };
		size_t seed1		 = 0;
		hash_range(seed1, vec.begin(), vec.end());

		size_t seed2 = 0;
		hash_append(seed2, 1, 2, 3, 4, 5);

		EXPECT_EQ(seed1, seed2);
	}

	TEST(HashTest, EmptyRange) {
		std::vector<int> empty_vec;
		size_t seed = 12345;
		hash_range(seed, empty_vec.begin(), empty_vec.end());

		EXPECT_EQ(seed, 12345);
	}

	TEST(HashTest, CollisionResistanceBasic) {
		std::unordered_set<size_t> hashes;

		hashes.insert(hash_combine(1, 2));
		hashes.insert(hash_combine(2, 1));
		hashes.insert(hash_combine(1, 1));
		hashes.insert(hash_combine(0, 0));

		EXPECT_EQ(hashes.size(), 4);
	}
} // namespace mstd::test