#include <gtest/gtest.h>
#include <pch.hpp>
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
	TEST_F(BimapTest, NonCopyableValue) {
		mstd::unordered_bimap<NonCopyableValue, NonCopyableValue> valueContainer;

		// EMPLACE
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.emplace(NonCopyableValue(2), NonCopyableValue(2)); // is not copyable
			// valueContainer.emplace(NonCopyableValue(2), test); // is not copyable
			// valueContainer.emplace(NonCopyableValue(2), std::move(test)); // is not copyable
			// valueContainer.emplace(test, NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace(test, test); // is not copyable
			// valueContainer.emplace(test, std::move(test)); // is not copyable
			// valueContainer.emplace(std::move(test), NonCopyableValue(1)); // is not copyable
			// valueContainer.emplace(std::move(test), test); // is not copyable
			// valueContainer.emplace(std::move(test), std::move(test)); // is not copyable
		}

		// INSERT
		{
			// auto test = NonCopyableValue(1);
			// valueContainer.insert(std::make_pair(NonCopyableValue(2), NonCopyableValue(2))); // is not copyable
			// valueContainer.insert(std::make_pair(NonCopyableValue(2), test)); // is not copyable
			// valueContainer.insert(std::make_pair(NonCopyableValue(2), std::move(test))); // is not copyable
			// valueContainer.insert(std::make_pair(test, NonCopyableValue(1))); // is not copyable
			// valueContainer.insert(std::make_pair(test, test)); // is not copyable
			// valueContainer.insert(std::make_pair(test, std::move(test))); // is not copyable
			// valueContainer.insert(std::make_pair(std::move(test), NonCopyableValue(1))); // is not copyable
			// valueContainer.insert(std::make_pair(std::move(test), test)); // is not copyable
			// valueContainer.insert(std::make_pair(std::move(test), std::move(test))); // is not copyable
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
	TEST_F(BimapTest, NonMovableValue) {
		mstd::unordered_bimap<NonMovableValue, NonMovableValue> valueContainer;

		// EMPLACE
		{
			auto test = NonMovableValue(1);
			valueContainer.emplace(NonMovableValue(2), NonMovableValue(2));
			valueContainer.emplace(NonMovableValue(2), test); // is not movable
			valueContainer.emplace(NonMovableValue(2), std::move(test));
			valueContainer.emplace(test, NonMovableValue(1)); // is not movable
			valueContainer.emplace(test, test); // is not movable
			valueContainer.emplace(test, std::move(test)); // is not movable
			valueContainer.emplace(std::move(test), NonMovableValue(1));
			valueContainer.emplace(std::move(test), test); // is not movable
			valueContainer.emplace(std::move(test), std::move(test));
		}

		// INSERT
		{
			auto test = NonMovableValue(1);
			valueContainer.insert(std::make_pair(NonMovableValue(2), NonMovableValue(2)));
			valueContainer.insert(std::make_pair(NonMovableValue(2), test));
			valueContainer.insert(std::make_pair(NonMovableValue(2), std::move(test)));
			valueContainer.insert(std::make_pair(test, NonMovableValue(1)));
			valueContainer.insert(std::make_pair(test, test));
			valueContainer.insert(std::make_pair(test, std::move(test)));
			valueContainer.insert(std::make_pair(std::move(test), NonMovableValue(1)));
			valueContainer.insert(std::make_pair(std::move(test), test));
			valueContainer.insert(std::make_pair(std::move(test), std::move(test)));
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