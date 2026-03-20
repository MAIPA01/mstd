#include <gtest/gtest.h>
#include <mstd/bimap.hpp>

namespace mstd::test {
	class BimapTest : public ::testing::Test {
	protected:
		mstd::unordered_bimap<std::string, int> bm;
	};

	TEST_F(BimapTest, IteratorRangeBasedForLoop) {
		bm.insert({ "One", 1 });
		bm.insert({ "Two", 2 });
		bm.insert({ "Three", 3 });

		std::vector<std::pair<std::string, int>> expected = {
			{ "One",	 1 },
			  { "Two",   2 },
			{ "Three", 3 }
		};

		size_t i = 0;
			for (const auto& pair : bm) {
				ASSERT_LT(i, expected.size());
				EXPECT_EQ(pair.first, expected[i].first);
				EXPECT_EQ(pair.second, expected[i].second);
				i++;
			}
		EXPECT_EQ(i, 3);
	}

	TEST_F(BimapTest, ConstIteration) {
		bm.insert({ "Alpha", 100 });
		const auto& c_bm = bm;

		auto it			 = c_bm.begin();
		EXPECT_EQ(it->first, "Alpha");
		EXPECT_EQ(it->second, 100);
	}

	TEST_F(BimapTest, ConflictResolutionComplex) {
		bm.insert({ "A", 1 });
		bm.insert({ "B", 2 });

		bm.insert({ "C", 1 });

		EXPECT_EQ(bm.size(), 2);
		EXPECT_TRUE(bm.contains("C"));
		EXPECT_FALSE(bm.contains("A"));
		EXPECT_EQ(bm.at_value(1), "C");

		bm.insert({ "B", 3 });

		EXPECT_EQ(bm.size(), 2);
		EXPECT_EQ(bm.at("B"), 3);
		EXPECT_FALSE(bm.contains_value(2));
	}

	TEST_F(BimapTest, EraseConsistency) {
		bm.insert({ "X", 1 });
		bm.insert({ "Y", 2 });
		bm.insert({ "Z", 3 });

		bm.erase("Y");

		EXPECT_EQ(bm.size(), 2);
		EXPECT_EQ(bm.at("Z"), 3);
		EXPECT_EQ(bm.at_value(3), "Z");

		auto it = bm.begin();
		EXPECT_EQ(it->first, "X");
		EXPECT_EQ((++it)->first, "Z");
	}

	TEST_F(BimapTest, Equality) {
		mstd::unordered_bimap<int, int> bm1 = {
			{ 1, 10 },
			   { 2, 20 }
		};
		mstd::unordered_bimap<int, int> bm2 = {
			{ 1, 10 },
			   { 2, 20 }
		};

		EXPECT_TRUE(bm1 == bm2);

		bm1.clear();
		EXPECT_NE(bm1, bm2);
	}
} // namespace mstd::test