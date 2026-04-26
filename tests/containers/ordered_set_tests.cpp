#include <gtest/gtest.h>
#include <pch.hpp>

namespace mstd::test {
	class OrderedSetTest : public ::testing::Test {
	protected:
		mstd::ordered_set<int> set;
	};

	TEST_F(OrderedSetTest, DefaultConstructorIsEmpty) {
		EXPECT_TRUE(set.empty());
		EXPECT_EQ(set.size(), 0);
	}

	TEST_F(OrderedSetTest, InitializerListConstructor) {
		mstd::ordered_set<int> s = { 1, 2, 3, 2, 1 };
		EXPECT_EQ(s.size(), 3);

		std::vector<int> expected = { 3, 2, 1 };
		size_t i				  = 0;
			for (const auto& val : s) { EXPECT_EQ(val, expected[i++]); }
	}

	TEST_F(OrderedSetTest, InsertNewElements) {
		set.insert(set.end(), 10);
		set.insert(set.end(), 20);
		set.insert(set.begin(), 5);

		EXPECT_EQ(set.size(), 3);
		auto it = set.begin();
		EXPECT_EQ(*it++, 5);
		EXPECT_EQ(*it++, 10);
		EXPECT_EQ(*it++, 20);
	}

	TEST_F(OrderedSetTest, InsertExistingElementMovesIt) {
		set = { 1, 2, 3, 4 };
		set.insert(set.end(), 2);

		EXPECT_EQ(set.size(), 4);
		std::vector<int> expected = { 1, 3, 4, 2 };
		size_t i				  = 0;
			for (const auto& val : set) { EXPECT_EQ(val, expected[i++]); }
	}

	TEST_F(OrderedSetTest, EraseElement) {
		set = { 10, 20, 30 };
		set.erase(20);

		EXPECT_EQ(set.size(), 2);
		EXPECT_FALSE(set.contains(20));
		EXPECT_EQ(*set.begin(), 10);
		EXPECT_EQ(*(++set.begin()), 30);
	}

	TEST_F(OrderedSetTest, EraseNonExistentElement) {
		set = { 1, 2 };
		set.erase(99);
		EXPECT_EQ(set.size(), 2);
	}

	TEST_F(OrderedSetTest, FindAndContains) {
		set = { 100, 200, 300 };

		EXPECT_TRUE(set.contains(200));
		EXPECT_FALSE(set.contains(400));

		auto it = set.find(200);
		EXPECT_NE(it, set.end());
		EXPECT_EQ(*it, 200);

		auto it_missing = set.find(500);
		EXPECT_EQ(it_missing, set.end());
	}

	TEST_F(OrderedSetTest, ReverseIterators) {
		set		 = { 1, 2, 3 };
		auto rit = set.rbegin();
		EXPECT_EQ(*rit++, 3);
		EXPECT_EQ(*rit++, 2);
		EXPECT_EQ(*rit++, 1);
		EXPECT_EQ(rit, set.rend());
	}

	TEST_F(OrderedSetTest, EqualityOperators) {
		mstd::ordered_set<int> s1 = { 1, 2, 3 };
		mstd::ordered_set<int> s2 = { 1, 2, 3 };
		mstd::ordered_set<int> s3 = { 3, 2, 1 };

		EXPECT_EQ(s1, s2);
		EXPECT_NE(s1, s3);
	}

	TEST_F(OrderedSetTest, Clear) {
		set = { 1, 2, 3, 4, 5 };
		set.clear();
		EXPECT_TRUE(set.empty());
		EXPECT_EQ(set.size(), 0);
		EXPECT_FALSE(set.contains(1));
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
	TEST_F(OrderedSetTest, NonCopyableValue) {
		mstd::ordered_set<NonCopyableValue> valueContainer;

		// EMPLACE
		{
			// valueContainer.emplace(valueContainer.cend(), 1); // is not copyable

			ASSERT_TRUE(true);
		}

		// EMPLACE BACK
		{
			// valueContainer.emplace_back(2); // is not copyable

			ASSERT_TRUE(true);
		}

		// INSERT
		{
			// auto test = NonCopyableValue(3);
			// valueContainer.insert(valueContainer.cend(), NonCopyableValue(4)); // is not copyable
			// valueContainer.insert(valueContainer.cend(), test);							// is not copyable
			// valueContainer.insert(valueContainer.cend(), std::move(test)); // is not copyable

			// valueContainer.insert(valueContainer.cend(), { NonCopyableValue(3) });		// is not copyable
			// valueContainer.insert(valueContainer.cend(), { test });						// is not copyable
			// valueContainer.insert(valueContainer.cend(), { std::move(test) });			// is not copyable

			ASSERT_TRUE(true);
		}

		// INSERT BACK
		{
			// auto test = NonCopyableValue(5);
			// valueContainer.insert_back(NonCopyableValue(6)); // is not copyable
			// valueContainer.insert_back(test);							// is not copyable
			// valueContainer.insert_back(std::move(test)); // is not copyable

			// valueContainer.insert_back({ NonCopyableValue(3) });			// is not copyable
			// valueContainer.insert_back({ test });						// is not copyable
			// valueContainer.insert_back({ std::move(test) });				// is not copyable

			ASSERT_TRUE(true);
		}

		// ERASE
		{
			// auto test = NonCopyableValue(3);
			// valueContainer.erase(NonCopyableValue(4)); // is not copyable
			// valueContainer.erase(test); // is not copyable
			// valueContainer.erase(std::move(test)); // is not copyable

			ASSERT_TRUE(true);
		}

		// CONTAINS
		{
			auto test	= NonCopyableValue(3);
			std::ignore = valueContainer.contains(NonCopyableValue(4));
			std::ignore = valueContainer.contains(test);
			std::ignore = valueContainer.contains(std::move(test));

			ASSERT_TRUE(true);
		}

		// FIND
		{
			auto test	= NonCopyableValue(3);
			std::ignore = valueContainer.find(NonCopyableValue(4));
			std::ignore = valueContainer.find(test);
			std::ignore = valueContainer.find(std::move(test));

			ASSERT_TRUE(true);
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
	TEST_F(OrderedSetTest, NonMovableValue) {
		mstd::ordered_set<NonMovableValue> valueContainer;

		// EMPLACE
		{
			// valueContainer.emplace(valueContainer.cend(), 1); // is not movable

			ASSERT_TRUE(true);
		}

		// EMPLACE BACK
		{
			// valueContainer.emplace_back(2); // is not movable

			ASSERT_TRUE(true);
		}

		// INSERT
		{
			// auto test = NonMovableValue(3);
			// valueContainer.insert(valueContainer.cend(), NonMovableValue(4)); // is not movable
			// valueContainer.insert(valueContainer.cend(), test);							// is not movable
			// valueContainer.insert(valueContainer.cend(), std::move(test)); // is not movable

			// valueContainer.insert(valueContainer.cend(), { NonMovableValue(3) });		// is not movable
			// valueContainer.insert(valueContainer.cend(), { test });						// is not movable
			// valueContainer.insert(valueContainer.cend(), { std::move(test) });			// is not movable

			ASSERT_TRUE(true);
		}

		// INSERT BACK
		{
			// auto test = NonMovableValue(5);
			// valueContainer.insert_back(NonMovableValue(6)); // is not movable
			// valueContainer.insert_back(test);							// is not movable
			// valueContainer.insert_back(std::move(test)); // is not movable

			// valueContainer.insert_back({ NonMovableValue(3) });			// is not movable
			// valueContainer.insert_back({ test });						// is not movable
			// valueContainer.insert_back({ std::move(test) });				// is not movable

			ASSERT_TRUE(true);
		}

		// ERASE
		{
			// auto test = NonMovableValue(3);
			// valueContainer.erase(NonMovableValue(4)); // is not movable
			// valueContainer.erase(test); // is not movable
			// valueContainer.erase(std::move(test)); // is not movable

			ASSERT_TRUE(true);
		}

		// CONTAINS
		{
			auto test	= NonMovableValue(3);
			std::ignore = valueContainer.contains(NonMovableValue(4));
			std::ignore = valueContainer.contains(test);
			std::ignore = valueContainer.contains(std::move(test));

			ASSERT_TRUE(true);
		}

		// FIND
		{
			auto test	= NonMovableValue(3);
			std::ignore = valueContainer.find(NonMovableValue(4));
			std::ignore = valueContainer.find(test);
			std::ignore = valueContainer.find(std::move(test));

			ASSERT_TRUE(true);
		}
	}
} // namespace mstd::test