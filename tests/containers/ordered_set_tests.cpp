#include <mstd/ordered_set.hpp>
#include <gtest/gtest.h>

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
        size_t i = 0;
        for (const auto& val : s) {
            EXPECT_EQ(val, expected[i++]);
        }
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
        size_t i = 0;
        for (const auto& val : set) {
            EXPECT_EQ(val, expected[i++]);
        }
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
        set = { 1, 2, 3 };
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
}