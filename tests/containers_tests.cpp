#include <mstd/containers.hpp>
#include <string>
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
            {"one", 1},
            {"two", 2},
            {"three", 3}
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
        map = { {"A", 1}, {"B", 2}, {"C", 3} };

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

    TEST_F(OrderedMapTest, ConstAtThrowsException) {
        const mstd::ordered_map<std::string, int> const_map = { {"fixed", 10} };
        EXPECT_EQ(const_map.at("fixed"), 10);
        EXPECT_THROW(const_map.at("missing"), std::out_of_range);
    }

    TEST_F(OrderedMapTest, EraseKey) {
        map = { {"X", 10}, {"Y", 20}, {"Z", 30} };
        map.erase("Y");

        EXPECT_EQ(map.size(), 2);
        EXPECT_FALSE(map.contains("Y"));

        EXPECT_EQ(map["Z"], 30);
        auto it = map.begin();
        EXPECT_EQ(it->first, "X");
        EXPECT_EQ((++it)->first, "Z");
    }

    TEST_F(OrderedMapTest, FindReturnsCorrectIterator) {
        map = { {"target", 99} };
        auto it = map.find("target");
        ASSERT_NE(it, map.end());
        EXPECT_EQ(it->second, 99);

        auto it_missing = map.find("ghost");
        EXPECT_EQ(it_missing, map.end());
    }

    TEST_F(OrderedMapTest, EqualityOperator) {
        mstd::ordered_map<int, int> m1 = { {1, 10}, {2, 20} };
        mstd::ordered_map<int, int> m2 = { {1, 10}, {2, 20} };
        mstd::ordered_map<int, int> m3 = { {2, 20}, {1, 10} };

        EXPECT_EQ(m1, m2);
        EXPECT_NE(m1, m3);
    }

    TEST_F(OrderedMapTest, ClearRemovesAll) {
        map = { {"A", 1}, {"B", 2} };
        map.clear();
        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0);
        EXPECT_FALSE(map.contains("A"));
    }

    class BimapTest : public ::testing::Test {
    protected:
        mstd::unordered_bimap<std::string, int> bm;
    };

    TEST_F(BimapTest, IteratorRangeBasedForLoop) {
        bm.insert({ "One", 1 });
        bm.insert({ "Two", 2 });
        bm.insert({ "Three", 3 });

        std::vector<std::pair<std::string, int>> expected = {
            {"One", 1}, {"Two", 2}, {"Three", 3}
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

        auto it = c_bm.begin();
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

    TEST_F(BimapTest, SetMethodsWithIterators) {
        bm.insert({ "User", 10 });
        auto it = bm.find("User");

        bm.set_value(it, 20);
        EXPECT_EQ(bm.at("User"), 20);
        EXPECT_FALSE(bm.contains_value(10));

        auto it2 = bm.find_value(20);
        bm.set_key(it2, "Admin");
        EXPECT_EQ(bm.at_value(20), "Admin");
        EXPECT_FALSE(bm.contains("User"));
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
        mstd::unordered_bimap<int, int> bm1 = { {1, 10}, {2, 20} };
        mstd::unordered_bimap<int, int> bm2 = { {1, 10}, {2, 20} };

        EXPECT_TRUE(bm1 == bm2);

        bm1.clear();
        EXPECT_NE(bm1, bm2);
    }

} // namespace mstd::test