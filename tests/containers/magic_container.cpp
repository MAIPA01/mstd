#include <gtest/gtest.h>
#include <mstd/magic_container.hpp>

namespace mstd::test {

    class MagicContainerTest : public ::testing::Test {
    protected:
        mstd::magic_container<int> container;
    };

    TEST_F(MagicContainerTest, DefaultConstructorIsEmpty) {
        EXPECT_TRUE(container.empty());
        EXPECT_EQ(container.size(), 0);
    }

    TEST_F(MagicContainerTest, InitializerListConstructor) {
        mstd::magic_container<int> c = { 10, 20, 30 };
        EXPECT_EQ(c.size(), 3);
        EXPECT_EQ(c.at(0), 10);
        EXPECT_EQ(c.at(2), 30);
    }

    TEST_F(MagicContainerTest, ResizeIncreasesSize) {
        container.resize(5, 100);
        EXPECT_EQ(container.size(), 5);
        EXPECT_EQ(container[4], 100);
        EXPECT_TRUE(container.has_value(4));
    }

    TEST_F(MagicContainerTest, PushBackAddsElements) {
        container.push_back(1);
        container.push_back(2);
        EXPECT_EQ(container.size(), 2);
        EXPECT_EQ(container.at(0), 1);
        EXPECT_EQ(container.at(1), 2);
    }

    TEST_F(MagicContainerTest, InsertAtSpecificId) {
        container.insert_at(5, 500);
        EXPECT_EQ(container.size(), 6);
        EXPECT_EQ(container.at(5), 500);
        EXPECT_TRUE(container.has_value(5));
        EXPECT_FALSE(container.has_value(2));
    }

    TEST_F(MagicContainerTest, EmplaceBackWorks)
    {
        struct Dummy {
            int a; std::string b;
            Dummy(int x, std::string y) : a(x), b(y) {}
        };
        mstd::magic_container<Dummy> c_dummy;
        c_dummy.emplace_back(42, "hello");
        EXPECT_EQ(c_dummy.at(0).a, 42);
        EXPECT_EQ(c_dummy.at(0).b, "hello");
    }

    TEST_F(MagicContainerTest, EraseRemovesElementAndSwaps) {
        container = { 0, 1, 2, 3, 4 };
        size_t initial_size = container.size();

        container.erase(1);

        EXPECT_EQ(container.size(), initial_size);
        EXPECT_FALSE(container.has_value(1));

        EXPECT_TRUE(container.has_value(0));
        EXPECT_TRUE(container.has_value(4));
    }

    TEST_F(MagicContainerTest, FrontAndBackAccess) {
        container = { 1, 2, 3 };
        EXPECT_EQ(container.front(), 1);
        EXPECT_EQ(container.back(), 3);
    }

    TEST_F(MagicContainerTest, IterationTest) {
        container = { 10, 20, 30 };
        int sum = 0;
        for (auto it = container.begin(); it != container.end(); ++it) {
            sum += *it;
        }
        EXPECT_EQ(sum, 60);
    }

    TEST_F(MagicContainerTest, MappingConsistency) {
        container.push_back(100); // ID 0
        container.push_back(200); // ID 1

        auto it = container.begin();
        EXPECT_EQ(container.get_id(it), 0);

        auto it2 = std::next(it);
        EXPECT_EQ(container.get_id(it2), 1);
    }

    TEST_F(MagicContainerTest, EqualityOperator) {
        mstd::magic_container<int> c1 = { 1, 2 };
        mstd::magic_container<int> c2 = { 1, 2 };
        EXPECT_TRUE(c1 == c2);

        c2.push_back(3);
        EXPECT_TRUE(c1 != c2);
    }

} // namespace mstd::test