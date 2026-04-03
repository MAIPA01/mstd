#include <gtest/gtest.h>
#include <pch.hpp>

namespace mstd::test {
	class OrderedMapTest : public ::testing::Test {
	protected:
		mstd::ordered_map<std::string, int> map;
	};

	TEST_F(OrderedMapTest, DefaultConstructor) {
		EXPECT_TRUE(map.empty());
		EXPECT_EQ(map.size(), 0);
	}

	TEST_F(OrderedMapTest, InitializerListConstructor) {
		mstd::ordered_map<std::string, int> m = {
			{ "one",	 1 },
			  { "two",   2 },
			{ "three", 3 }
		};
		EXPECT_EQ(m.size(), 3);
		EXPECT_EQ(m["one"], 1);

		auto it = m.begin();
		EXPECT_EQ(it->first, "one");
		++it;
		EXPECT_EQ(it->first, "two");
	}

	TEST_F(OrderedMapTest, InsertNewElement) {
		map.insert(map.end(), { "key1", 100 });
		EXPECT_EQ(map.size(), 1);
		EXPECT_TRUE(map.contains("key1"));
		EXPECT_EQ(map["key1"], 100);
	}

	TEST_F(OrderedMapTest, InsertExistingKeyMovesElement) {
		map = {
			{ "A", 1 },
			{ "B", 2 },
			{ "C", 3 }
		};

		map.insert(map.end(), { "A", 10 });

		EXPECT_EQ(map.size(), 3);
		EXPECT_EQ(map["A"], 10);

		auto it = map.begin();
		EXPECT_EQ(it->first, "B");
		EXPECT_EQ((++it)->first, "C");
		EXPECT_EQ((++it)->first, "A");
	}

	TEST_F(OrderedMapTest, OperatorSquareBracketsInsertsMissingKey) {
		int& value = map["new_key"];
		EXPECT_EQ(value, 0);
		EXPECT_EQ(map.size(), 1);

		value = 50;
		EXPECT_EQ(map["new_key"], 50);
	}

	TEST_F(OrderedMapTest, EraseKey) {
		map = {
			{ "X", 10 },
			 { "Y", 20 },
			  { "Z", 30 }
		};
		map.erase("Y");

		EXPECT_EQ(map.size(), 2);
		EXPECT_FALSE(map.contains("Y"));

		EXPECT_EQ(map["Z"], 30);
		auto it = map.begin();
		EXPECT_EQ(it->first, "X");
		EXPECT_EQ((++it)->first, "Z");
	}

	TEST_F(OrderedMapTest, FindReturnsCorrectIterator) {
		map = {
			{ "target", 99 }
		};
		auto it = map.find("target");
		ASSERT_NE(it, map.end());
		EXPECT_EQ(it->second, 99);

		auto it_missing = map.find("ghost");
		EXPECT_EQ(it_missing, map.end());
	}

	TEST_F(OrderedMapTest, EqualityOperator) {
		mstd::ordered_map<int, int> m1 = {
			{ 1, 10 },
			   { 2, 20 }
		};
		mstd::ordered_map<int, int> m2 = {
			{ 1, 10 },
			   { 2, 20 }
		};
		mstd::ordered_map<int, int> m3 = {
			{ 2, 20 },
			   { 1, 10 }
		};

		EXPECT_EQ(m1, m2);
		EXPECT_NE(m1, m3);
	}

	TEST_F(OrderedMapTest, ClearRemovesAll) {
		map = {
			{ "A", 1 },
			{ "B", 2 }
		};
		map.clear();
		EXPECT_TRUE(map.empty());
		EXPECT_EQ(map.size(), 0);
		EXPECT_FALSE(map.contains("A"));
	}
} // namespace mstd::test