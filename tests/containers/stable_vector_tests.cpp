#include <gtest/gtest.h>
#include <pch.hpp>
#include <mstd/stable_vector.hpp>

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
		EXPECT_EQ(c.get_next_id(), 3);
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

			Dummy(const int x, const std::string& y) : a(x), b(y) {}
		};

		mstd::stable_vector<Dummy> c_dummy;
		c_dummy.emplace_back(42, "hello");
		EXPECT_EQ(c_dummy.at(0).a, 42);
		EXPECT_EQ(c_dummy.at(0).b, "hello");
	}

	TEST_F(StableVectorTest, EraseRemovesElementAndSwaps) {
		container			= { 0, 1, 2, 3, 4 };
		const size_t initial_size = container.size();

		container.erase(1);

		EXPECT_EQ(container.size(), initial_size);
		EXPECT_FALSE(container.has_value(1));

		EXPECT_TRUE(container.has_value(0));
		EXPECT_TRUE(container.has_value(4));
	}

	TEST_F(StableVectorTest, FrontAndBackAccess) {
		container = { 1, 2, 3 };
		EXPECT_EQ(container.front(), 1);
		EXPECT_EQ(container.front_id(), 0);
		EXPECT_EQ(container.back(), 3);
		EXPECT_EQ(container.back_id(), 2);
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

		const auto it = container.begin();
		EXPECT_EQ(container.get_id(it), 0);

		const auto it2 = std::next(it);
		EXPECT_EQ(container.get_id(it2), 1);
	}

	TEST_F(StableVectorTest, EqualityOperator) {
		const mstd::stable_vector<int> c1 = { 1, 2 };
		mstd::stable_vector<int> c2 = { 1, 2 };
		EXPECT_TRUE(c1 == c2);

		c2.push_back(3);
		EXPECT_TRUE(c1 != c2);
	}

	TEST_F(StableVectorTest, GetIdFromIterator) {
		EXPECT_EQ(container.get_next_id(), 0);
		container.push_back(10);	  // ID 0
		container.insert_at(10, 100); // ID 10
		EXPECT_EQ(container.get_next_id(), 2);
		container.push_back(20);	  // ID 2

		const auto it = container.begin();
		EXPECT_EQ(container.get_id(it), 0);

		auto it_last = std::prev(container.end());
		EXPECT_EQ(*it_last, 20);
		EXPECT_EQ(container.get_id(2), 2);
		EXPECT_EQ(container.get_id(it_last), 2);

		EXPECT_EQ(container.get_next_id(), 3);
		container.insert(22); // ID 3

		it_last = std::prev(container.end());
		EXPECT_EQ(*it_last, 22);
		EXPECT_EQ(container.get_id(it_last), 3);
		EXPECT_EQ(container.get_id(3), 3);
	}

	TEST_F(StableVectorTest, HasValueViaIterator) {
		container = { 10, 20, 30 }; // ID: 0, 1, 2

		const auto it	  = container.begin();
		EXPECT_TRUE(container.has_value(it));

		container.erase(0); // Usuwamy element pod ID 0

		const auto it_new = container.begin();
		EXPECT_TRUE(container.has_value(it_new));
		EXPECT_EQ(*it_new, 30);
	}

	TEST_F(StableVectorTest, HasValueViaConstIterator) {
		const mstd::stable_vector<int> const_container = { 100, 200 };

		const auto cit									   = const_container.cbegin();
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

	TEST_F(StableVectorTest, EmplaceBackAndErase) {
		EXPECT_EQ(container.size(), 0);

		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(0));
		EXPECT_EQ(container.get_next_id(), 1);
		EXPECT_EQ(container.size(), 1);

		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(1));
		EXPECT_EQ(container.get_next_id(), 2);
		EXPECT_EQ(container.size(), 2);

		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(2));
		EXPECT_EQ(container.get_next_id(), 3);
		EXPECT_EQ(container.size(), 3);

		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(3));
		EXPECT_EQ(container.get_next_id(), 4);
		EXPECT_EQ(container.size(), 4);

		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(4));
		EXPECT_EQ(container.get_next_id(), 5);
		EXPECT_EQ(container.size(), 5);

		// ADD 5
		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(5));
		EXPECT_FALSE(container.has_value(6));
		EXPECT_FALSE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 6);
		EXPECT_EQ(container.size(), 6);

		// ADD 6
		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(5));
		EXPECT_TRUE(container.has_value(6));
		EXPECT_FALSE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 7);
		EXPECT_EQ(container.size(), 7);

		// ADD 7
		container.emplace_back(0);
		EXPECT_TRUE(container.has_value(5));
		EXPECT_TRUE(container.has_value(6));
		EXPECT_TRUE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 8);
		EXPECT_EQ(container.size(), 8);

		// REMOVE 5
		container.erase(5);
		EXPECT_FALSE(container.has_value(5));
		EXPECT_TRUE(container.has_value(6));
		EXPECT_TRUE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 5);
		EXPECT_EQ(container.size(), 8);

		// REMOVE 6
		container.erase(6);
		EXPECT_FALSE(container.has_value(5));
		EXPECT_FALSE(container.has_value(6));
		EXPECT_TRUE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 6);
		EXPECT_EQ(container.size(), 8);

		// REMOVE 7
		container.erase(7);
		EXPECT_FALSE(container.has_value(5));
		EXPECT_FALSE(container.has_value(6));
		EXPECT_FALSE(container.has_value(7));
		EXPECT_EQ(container.get_next_id(), 5);
		EXPECT_EQ(container.size(), 5);
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

		const auto it = container.get(1);
		ASSERT_NE(it, container.end());
		EXPECT_EQ(*it, 20);
		EXPECT_EQ(container.get_id(it), 1);

		const auto& c_container = container;
		const auto cit				= c_container.get(2);
		EXPECT_EQ(*cit, 30);
	}

	TEST_F(StableVectorTest, TryGetReturnsIteratorOrEnd) {
		container.insert_at(10, 1000);

		const auto it_found = container.try_get(10);
		EXPECT_NE(it_found, container.end());
		EXPECT_EQ(*it_found, 1000);

		const auto it_missing = container.try_get(5);
		EXPECT_EQ(it_missing, container.end());
	}

	TEST_F(StableVectorTest, ConstTryGetReturnsConstIterator) {
		const mstd::stable_vector<int> c_container = { 10, 20 };

		mstd::stable_vector<int> temp			   = { 1, 2, 3 };
		temp.erase(1);
		const auto& c_temp = temp;

		const auto it			   = c_temp.try_get(1);
		EXPECT_EQ(it, c_temp.cend());

		const auto it_valid = c_temp.try_get(0);
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

		const size_t current_active = container.active_slots();
		const size_t current_empty  = container.empty_slots();

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

	class NonCopyableValue {
	public:
		NonCopyableValue()									 = default;
		NonCopyableValue(const NonCopyableValue&)			 = delete;
		NonCopyableValue(NonCopyableValue&&)				 = default;
		~NonCopyableValue()									 = default;

		NonCopyableValue& operator=(const NonCopyableValue&) = delete;
		NonCopyableValue& operator=(NonCopyableValue&&)		 = default;
	};

	TEST_F(StableVectorTest, NonCopyableValue) {
		mstd::stable_vector<NonCopyableValue> valueContainer;

		// INSERT ON FREE SPOT
		{
			auto test = NonCopyableValue();
			valueContainer.insert(NonCopyableValue());
			// valueContainer.insert(test);						// is not copyable
			valueContainer.insert(std::move(test));

			// valueContainer.insert(NonCopyableValue(), 2);	// is not copyable
			// valueContainer.insert(test, 2);					// is not copyable
			// valueContainer.insert(std::move(test), 2);		// is not copyable

			// valueContainer.insert({ NonCopyableValue() });	// is not copyable
			// valueContainer.insert({ test });					// is not copyable
			// valueContainer.insert({ std::move(test) });		// is not copyable

			ASSERT_TRUE(true);
		}

		// INSERT AT IDX
		{
			auto test = NonCopyableValue();
			valueContainer.insert_at(0, NonCopyableValue());
			// valueContainer.insert_at(1, test);						// is not copyable
			valueContainer.insert_at(2, std::move(test));

			// valueContainer.insert_at(3, NonCopyableValue(), 2);		// is not copyable
			// valueContainer.insert_at(4, test, 2);					// is not copyable
			// valueContainer.insert_at(5, std::move(test), 2);			// is not copyable

			// valueContainer.insert_at(6, { NonCopyableValue() });		// is not copyable
			// valueContainer.insert_at(7, { test });					// is not copyable
			// valueContainer.insert_at(8, { std::move(test) });		// is not copyable

			ASSERT_TRUE(true);
		}

		// EMPLACE ON FREE SPOT
		{
			valueContainer.emplace();

			ASSERT_TRUE(true);
		}

		// EMPLACE AT IDX
		{
			valueContainer.emplace_at(0);

			ASSERT_TRUE(true);
		}

		// EMPLACE BACK
		{
			valueContainer.emplace_back();

			ASSERT_TRUE(true);
		}

		// PUSH BACK
		{
			auto test = NonCopyableValue();
			valueContainer.push_back(NonCopyableValue());
			// valueContainer.push_back(test);					// is not copyable
			valueContainer.push_back(std::move(test));

			ASSERT_TRUE(true);
		}
	}

	class NonMovableValue {
	public:
		NonMovableValue()								   = default;
		NonMovableValue(const NonMovableValue&)			   = default;
		NonMovableValue(NonMovableValue&&)				   = delete;
		~NonMovableValue()								   = default;

		NonMovableValue& operator=(const NonMovableValue&) = default;
		NonMovableValue& operator=(NonMovableValue&&)	   = delete;
	};

	TEST_F(StableVectorTest, NonMovableValue) {
		mstd::stable_vector<NonMovableValue> valueContainer;

		// INSERT ON FREE SPOT
		{
			// auto test = NonMovableValue();
			// valueContainer.insert(NonMovableValue());			// is not movable
			// valueContainer.insert(test);							// is not movable
			// valueContainer.insert(std::move(test));				// is not movable

			// valueContainer.insert(NonMovableValue(), 2);			// is not movable
			// valueContainer.insert(test, 2);						// is not movable
			// valueContainer.insert(std::move(test), 2);			// is not movable

			// valueContainer.insert({ NonMovableValue() });		// is not movable
			// valueContainer.insert({ test });						// is not movable
			// valueContainer.insert({ std::move(test) });			// is not movable

			ASSERT_TRUE(true);
		}

		// INSERT AT IDX
		{
			// auto test = NonMovableValue();
			// valueContainer.insert_at(0, NonMovableValue());			// is not movable
			// valueContainer.insert_at(1, test);						// is not movable
			// valueContainer.insert_at(2, std::move(test));			// is not movable

			// valueContainer.insert_at(3, NonMovableValue(), 2);		// is not movable
			// valueContainer.insert_at(4, test, 2);					// is not movable
			// valueContainer.insert_at(5, std::move(test), 2);			// is not movable

			// valueContainer.insert_at(6, { NonMovableValue() });		// is not movable
			// valueContainer.insert_at(7, { test });					// is not movable
			// valueContainer.insert_at(8, { std::move(test) });		// is not movable

			ASSERT_TRUE(true);
		}

		// EMPLACE ON FREE SPOT
		{
			// valueContainer.emplace(); // is not movable

			ASSERT_TRUE(true);
		}

		// EMPLACE AT IDX
		{
			// valueContainer.emplace_at(0); // is not movable

			ASSERT_TRUE(true);
		}

		// EMPLACE BACK
		{
			// valueContainer.emplace_back(); // is not movable

			ASSERT_TRUE(true);
		}

		// PUSH BACK
		{
			// auto test = NonMovableValue();
			// valueContainer.push_back(NonMovableValue());	// is not movable
			// valueContainer.push_back(test);				// is not movable
			// valueContainer.push_back(std::move(test));	// is not movable

			ASSERT_TRUE(true);
		}
	}
} // namespace mstd::test