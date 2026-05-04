#include <gtest/gtest.h>
#include <pch.hpp>
#include <mstd/ordered_map.hpp>

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

	class NonCopyableValue {
	public:
		int value = 0;

		explicit NonCopyableValue(const int val) : value(val) {}

		NonCopyableValue(const NonCopyableValue&)			 = delete;
		NonCopyableValue(NonCopyableValue&&)				 = default;
		~NonCopyableValue()									 = default;

		NonCopyableValue& operator=(const NonCopyableValue&) = delete;
		NonCopyableValue& operator=(NonCopyableValue&&)		 = default;

		[[nodiscard]] bool operator==(const NonCopyableValue& other) const { return value == other.value; }
	};
} // namespace mstd::test

template<>
struct ::std::hash<mstd::test::NonCopyableValue> {
	[[nodiscard]] size_t operator()(const mstd::test::NonCopyableValue& val) const noexcept {
		return ::std::hash<int>()(val.value);
	}
};

namespace mstd::test {
	TEST_F(OrderedMapTest, NonCopyableValue) {
		mstd::ordered_map<NonCopyableValue, NonCopyableValue> valueContainer;

		// EMPLACE
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.emplace(valueContainer.cend(), NonCopyableValue(2), NonCopyableValue(2)); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), NonCopyableValue(2), test); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), NonCopyableValue(2), std::move(test)); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), test, NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), test, test); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), test, std::move(test)); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), std::move(test), NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), std::move(test), test); // is not copyable
			// valueContainer.emplace(valueContainer.cend(), std::move(test), std::move(test)); // is not copyable
		}

		// EMPLACE BACK
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.emplace_back(NonCopyableValue(2), NonCopyableValue(2)); // is not copyable
			// valueContainer.emplace_back(NonCopyableValue(2), test); // is not copyable
			// valueContainer.emplace_back(NonCopyableValue(2), std::move(test)); // is not copyable
			// valueContainer.emplace_back(test, NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace_back(test, test); // is not copyable
			// valueContainer.emplace_back(test, std::move(test)); // is not copyable
			// valueContainer.emplace_back(std::move(test), NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace_back(std::move(test), test); // is not copyable
			// valueContainer.emplace_back(std::move(test), std::move(test)); // is not copyable
		}

		// INSERT
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.insert(valueContainer.cend(), std::make_pair(NonCopyableValue(2), NonCopyableValue(2))); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(NonCopyableValue(2), test)); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(NonCopyableValue(2), std::move(test))); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(test, NonCopyableValue(1))); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(test, test)); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(test, std::move(test))); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), NonCopyableValue(1))); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), test)); // is not copyable
			// valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), std::move(test))); // is not copyable
		}

		// INSERT BACK
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.insert_back(std::make_pair(NonCopyableValue(2), NonCopyableValue(2))); // is not copyable
			// valueContainer.insert_back(std::make_pair(NonCopyableValue(2), test)); // is not copyable
			// valueContainer.insert_back(std::make_pair(NonCopyableValue(2), std::move(test))); // is not copyable
			// valueContainer.insert_back(std::make_pair(test, NonCopyableValue(1))); // is not copyable
			// valueContainer.insert_back(std::make_pair(test, test)); // is not copyable
			// valueContainer.insert_back(std::make_pair(test, std::move(test))); // is not copyable
			// valueContainer.insert_back(std::make_pair(std::move(test), NonCopyableValue(1))); // is not copyable
			// valueContainer.insert_back(std::make_pair(std::move(test), test)); // is not copyable
			// valueContainer.insert_back(std::make_pair(std::move(test), std::move(test))); // is not copyable
		}

		// ERASE
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.erase(NonCopyableValue(2)); // is not copyable
			// valueContainer.erase(test); // is not copyable
			// valueContainer.erase(std::move(test)); // is not copyable
		}

		// AT
		{
			// auto test = NonCopyableValue(1);
			// std::ignore = valueContainer.at(NonCopyableValue(2)); // cannot test because nothing was added
			// std::ignore = valueContainer.at(test); // cannot test because nothing was added
			// std::ignore = valueContainer.at(std::move(test)); // cannot test because nothing was added
		}

		// CONTAINS
		{
			auto test = NonCopyableValue(1);
			std::ignore = valueContainer.contains(NonCopyableValue(2));
			std::ignore = valueContainer.contains(test);
			std::ignore = valueContainer.contains(std::move(test));
		}

		// FIND
		{
			auto test = NonCopyableValue(1);
			std::ignore = valueContainer.find(NonCopyableValue(2));
			std::ignore = valueContainer.find(test);
			std::ignore = valueContainer.find(std::move(test));
		}
	}

	class NonMovableValue {
	public:
		int value = 0;

		explicit NonMovableValue(const int val) : value(val) {}

		NonMovableValue(const NonMovableValue&)			   = default;
		NonMovableValue(NonMovableValue&&)				   = delete;
		~NonMovableValue()								   = default;

		NonMovableValue& operator=(const NonMovableValue&) = default;
		NonMovableValue& operator=(NonMovableValue&&)	   = delete;

		[[nodiscard]] bool operator==(const NonMovableValue& other) const { return value == other.value; }
	};
} // namespace mstd::test

template<>
struct ::std::hash<mstd::test::NonMovableValue> {
	[[nodiscard]] size_t operator()(const mstd::test::NonMovableValue& val) const noexcept {
		return ::std::hash<int>()(val.value);
	}
};

namespace mstd::test {
	TEST_F(OrderedMapTest, NonMovableValue) {
		mstd::ordered_map<NonMovableValue, NonMovableValue> valueContainer;

		// EMPLACE
		{
			auto test = NonMovableValue(1);
			valueContainer.emplace(valueContainer.cend(), NonMovableValue(2), NonMovableValue(2));
			// valueContainer.emplace(valueContainer.cend(), NonMovableValue(2), test); // is not movable
			valueContainer.emplace(valueContainer.cend(), NonMovableValue(2), std::move(test));
			// valueContainer.emplace(valueContainer.cend(), test, NonMovableValue(1)); // is not movable
			// valueContainer.emplace(valueContainer.cend(), test, test); // is not movable
			// valueContainer.emplace(valueContainer.cend(), test, std::move(test)); // is not movable
			valueContainer.emplace(valueContainer.cend(), std::move(test), NonMovableValue(1));
			// valueContainer.emplace(valueContainer.cend(), std::move(test), test); // is not movable
			valueContainer.emplace(valueContainer.cend(), std::move(test), std::move(test));
		}

		// EMPLACE BACK
		{
			auto test = NonMovableValue(1);
			valueContainer.emplace_back(NonMovableValue(2), NonMovableValue(2));
			// valueContainer.emplace_back(NonMovableValue(2), test); // is not movable
			valueContainer.emplace_back(NonMovableValue(2), std::move(test));
			// valueContainer.emplace_back(test, NonMovableValue(1)); // is not movable
			// valueContainer.emplace_back(test, test); // is not movable
			// valueContainer.emplace_back(test, std::move(test)); // is not movable
			valueContainer.emplace_back(std::move(test), NonMovableValue(1));
			// valueContainer.emplace_back(std::move(test), test); // is not movable
			valueContainer.emplace_back(std::move(test), std::move(test));
		}

		// INSERT
		{
			auto test = NonMovableValue(1);
			valueContainer.insert(valueContainer.cend(), std::make_pair(NonMovableValue(2), NonMovableValue(2)));
			valueContainer.insert(valueContainer.cend(), std::make_pair(NonMovableValue(2), test));
			valueContainer.insert(valueContainer.cend(), std::make_pair(NonMovableValue(2), std::move(test)));
			valueContainer.insert(valueContainer.cend(), std::make_pair(test, NonMovableValue(1)));
			valueContainer.insert(valueContainer.cend(), std::make_pair(test, test));
			valueContainer.insert(valueContainer.cend(), std::make_pair(test, std::move(test)));
			valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), NonMovableValue(1)));
			valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), test));
			valueContainer.insert(valueContainer.cend(), std::make_pair(std::move(test), std::move(test)));
		}

		// INSERT BACK
		{
			auto test = NonMovableValue(1);
			valueContainer.insert_back(std::make_pair(NonMovableValue(2), NonMovableValue(2)));
			valueContainer.insert_back(std::make_pair(NonMovableValue(2), test));
			valueContainer.insert_back(std::make_pair(NonMovableValue(2), std::move(test)));
			valueContainer.insert_back(std::make_pair(test, NonMovableValue(1)));
			valueContainer.insert_back(std::make_pair(test, test));
			valueContainer.insert_back(std::make_pair(test, std::move(test)));
			valueContainer.insert_back(std::make_pair(std::move(test), NonMovableValue(1)));
			valueContainer.insert_back(std::make_pair(std::move(test), test));
			valueContainer.insert_back(std::make_pair(std::move(test), std::move(test)));
		}

		// ERASE
		{
			auto test = NonMovableValue(1);
			valueContainer.erase(NonMovableValue(2));
			valueContainer.erase(test);
			valueContainer.erase(std::move(test));
		}

		// CONTAINS
		{
			auto test = NonMovableValue(1);
			std::ignore = valueContainer.contains(NonMovableValue(2));
			std::ignore = valueContainer.contains(test);
			std::ignore = valueContainer.contains(std::move(test));
		}

		// AT
		{
			// auto test = NonMovableValue(2);
			// std::ignore = valueContainer.at(NonMovableValue(2));
			// std::ignore = valueContainer.at(test);
			// std::ignore = valueContainer.at(std::move(test));
		}

		// FIND
		{
			auto test = NonMovableValue(1);
			std::ignore = valueContainer.find(NonMovableValue(2));
			std::ignore = valueContainer.find(test);
			std::ignore = valueContainer.find(std::move(test));
		}
	}
} // namespace mstd::test