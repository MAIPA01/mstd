#include <gtest/gtest.h>
#include <pch.hpp>

namespace mstd::test {

	class StableVectorTest : public ::testing::Test {
	protected:
		mstd::stable_vector<int> container;
	};

	TEST_F(StableVectorTest, DefaultConstructorIsEmpty) {
		EXPECT_TRUE(container.empty());
		EXPECT_EQ(container.size(), 0);
	}

	TEST_F(StableVectorTest, InitializerListConstructor) {
		mstd::stable_vector<int> c = { 10, 20, 30 };
		EXPECT_EQ(c.size(), 3);
		EXPECT_EQ(c.at(0), 10);
		EXPECT_EQ(c.at(2), 30);
	}

	TEST_F(StableVectorTest, ResizeIncreasesSize) {
		container.resize(5, 100);
		EXPECT_EQ(container.size(), 5);
		EXPECT_EQ(container[4], 100);
		EXPECT_TRUE(container.has_value(4));
	}

	TEST_F(StableVectorTest, PushBackAddsElements) {
		container.push_back(1);
		container.push_back(2);
		EXPECT_EQ(container.size(), 2);
		EXPECT_EQ(container.at(0), 1);
		EXPECT_EQ(container.at(1), 2);
	}

	TEST_F(StableVectorTest, InsertAtSpecificId) {
		container.insert_at(5, 500);
		EXPECT_EQ(container.size(), 6);
		EXPECT_EQ(container.at(5), 500);
		EXPECT_TRUE(container.has_value(5));
		EXPECT_FALSE(container.has_value(2));
	}

	TEST_F(StableVectorTest, EmplaceBackWorks) {
		struct Dummy {
			int a;
			std::string b;

			Dummy(int x, std::string y) : a(x), b(y) {}
		};

		mstd::stable_vector<Dummy> c_dummy;
		c_dummy.emplace_back(42, "hello");
		EXPECT_EQ(c_dummy.at(0).a, 42);
		EXPECT_EQ(c_dummy.at(0).b, "hello");
	}

	TEST_F(StableVectorTest, EraseRemovesElementAndSwaps) {
		container			= { 0, 1, 2, 3, 4 };
		size_t initial_size = container.size();

		container.erase(1);

		EXPECT_EQ(container.size(), initial_size);
		EXPECT_FALSE(container.has_value(1));

		EXPECT_TRUE(container.has_value(0));
		EXPECT_TRUE(container.has_value(4));
	}

	TEST_F(StableVectorTest, FrontAndBackAccess) {
		container = { 1, 2, 3 };
		EXPECT_EQ(container.front(), 1);
		EXPECT_EQ(container.back(), 3);
	}

	TEST_F(StableVectorTest, IterationTest) {
		container = { 10, 20, 30 };
		int sum	  = 0;
			for (auto it = container.begin(); it != container.end(); ++it) { sum += *it; }
		EXPECT_EQ(sum, 60);
	}

	TEST_F(StableVectorTest, MappingConsistency) {
		container.push_back(100); // ID 0
		container.push_back(200); // ID 1

		auto it = container.begin();
		EXPECT_EQ(container.get_id(it), 0);

		auto it2 = std::next(it);
		EXPECT_EQ(container.get_id(it2), 1);
	}

	TEST_F(StableVectorTest, EqualityOperator) {
		mstd::stable_vector<int> c1 = { 1, 2 };
		mstd::stable_vector<int> c2 = { 1, 2 };
		EXPECT_TRUE(c1 == c2);

		c2.push_back(3);
		EXPECT_TRUE(c1 != c2);
	}

	TEST_F(StableVectorTest, GetIdFromIterator) {
		container.push_back(10);	  // ID 0
		container.insert_at(10, 100); // ID 10
		container.push_back(20);	  // ID 11

		auto it = container.begin();
		EXPECT_EQ(container.get_id(it), 0);

		auto it_last = std::prev(container.end());
		EXPECT_EQ(*it_last, 20);
		EXPECT_EQ(container.get_id(it_last), 11);

		container.insert(22); // ID 3

		it_last = std::prev(container.end());
		EXPECT_EQ(*it_last, 22);
		EXPECT_EQ(container.get_id(it_last), 3);
	}

	TEST_F(StableVectorTest, HasValueViaIterator) {
		container = { 10, 20, 30 }; // ID: 0, 1, 2

		auto it	  = container.begin();
		EXPECT_TRUE(container.has_value(it));

		container.erase(0); // Usuwamy element pod ID 0

		auto it_new = container.begin();
		EXPECT_TRUE(container.has_value(it_new));
		EXPECT_EQ(*it_new, 30);
	}

	TEST_F(StableVectorTest, HasValueViaConstIterator) {
		const mstd::stable_vector<int> const_container = { 100, 200 };

		auto cit									   = const_container.cbegin();
		EXPECT_TRUE(const_container.has_value(cit));

		EXPECT_EQ(const_container.get_id(cit), 0);
	}

	TEST_F(StableVectorTest, GetIdThrowsOrAssertsOnEndIterator) {
#ifdef _DEBUG
		EXPECT_DEATH(std::ignore = container.get_id(container.end()), "Pos out of bounds");
#endif
	}

	TEST_F(StableVectorTest, EraseInLoopUsingIds) {
			for (int i = 0; i < 6; ++i) { container.push_back(i * 10); }

			for (size_t id = 0; id < 6; id += 2) { container.erase(id); }

		EXPECT_FALSE(container.has_value(0));
		EXPECT_TRUE(container.has_value(1));
		EXPECT_FALSE(container.has_value(2));
		EXPECT_TRUE(container.has_value(3));

		EXPECT_EQ(container.at(1), 10);
		EXPECT_EQ(container.at(3), 30);
		EXPECT_EQ(container.at(5), 50);
	}

	TEST_F(StableVectorTest, TryAtReturnsPointerOrNull) {
		container.insert_at(5, 500);

		int* ptr_found = container.try_at(5);
		ASSERT_NE(ptr_found, nullptr);
		EXPECT_EQ(*ptr_found, 500);

		int* ptr_empty = container.try_at(2);
		EXPECT_EQ(ptr_empty, nullptr);

		const auto& c_container = container;
		const int* c_ptr		= c_container.try_at(5);
		EXPECT_EQ(*c_ptr, 500);
	}

	TEST_F(StableVectorTest, GetReturnsValidIterator) {
		container.push_back(10); // ID 0
		container.push_back(20); // ID 1
		container.push_back(30); // ID 2

		auto it = container.get(1);
		ASSERT_NE(it, container.end());
		EXPECT_EQ(*it, 20);
		EXPECT_EQ(container.get_id(it), 1);

		const auto& c_container = container;
		auto cit				= c_container.get(2);
		EXPECT_EQ(*cit, 30);
	}

	TEST_F(StableVectorTest, TryGetReturnsIteratorOrEnd) {
		container.insert_at(10, 1000);

		auto it_found = container.try_get(10);
		EXPECT_NE(it_found, container.end());
		EXPECT_EQ(*it_found, 1000);

		auto it_missing = container.try_get(5);
		EXPECT_EQ(it_missing, container.end());
	}

	TEST_F(StableVectorTest, ConstTryGetReturnsConstIterator) {
		const mstd::stable_vector<int> c_container = { 10, 20 };

		mstd::stable_vector<int> temp			   = { 1, 2, 3 };
		temp.erase(1);
		const auto& c_temp = temp;

		auto it			   = c_temp.try_get(1);
		EXPECT_EQ(it, c_temp.cend());

		auto it_valid = c_temp.try_get(0);
		EXPECT_EQ(*it_valid, 1);
	}

	TEST_F(StableVectorTest, SlotCountingLogic) {
		EXPECT_EQ(container.active_slots(), 0);
		EXPECT_EQ(container.empty_slots(), 0);

		container.push_back(10);
		container.push_back(20);
		container.push_back(30);

		EXPECT_EQ(container.active_slots(), 3);
		EXPECT_EQ(container.empty_slots(), 0);
		EXPECT_EQ(container.size(), 3);

		container.erase(1);

		EXPECT_EQ(container.active_slots(), 2);
		EXPECT_EQ(container.empty_slots(), 1);
		EXPECT_EQ(container.size(), 3);

		container.insert_at(10, 100);
		EXPECT_EQ(container.active_slots(), 3);
		EXPECT_EQ(container.empty_slots(), 8);
		EXPECT_EQ(container.size(), 11);
	}

	TEST_F(StableVectorTest, EmptySlotsAfterClear) {
		container = { 1, 2, 3, 4, 5 };
		container.erase(0);
		container.erase(1);

		size_t current_active = container.active_slots();
		size_t current_empty  = container.empty_slots();

		EXPECT_EQ(current_active, 3);
		EXPECT_EQ(current_empty, 2);

		EXPECT_EQ(container.active_slots() + container.empty_slots(), container.size());
	}

	TEST_F(StableVectorTest, ActiveSlotsConsistency) {
		container.emplace_back(42);
		EXPECT_EQ(container.active_slots(), 1);

		container.push_back(24);
		EXPECT_EQ(container.active_slots(), 2);

		container.erase(0);
		EXPECT_EQ(container.active_slots(), 1);
	}
} // namespace mstd::test