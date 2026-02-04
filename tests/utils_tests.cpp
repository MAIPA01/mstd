#include <mstd/utils.hpp>
#include <gtest/gtest.h>
#include <unordered_set>

namespace mstd::test {
    TEST(HashTest, Consistency) {
        std::string s = "test";
        int i = 42;

        size_t h1 = hash_combine(s, i);
        size_t h2 = hash_combine(s, i);

        EXPECT_EQ(h1, h2);
        EXPECT_NE(h1, 0);
    }

    TEST(HashTest, OrderMatters) {
        size_t h1 = hash_combine(std::string("A"), std::string("B"));
        size_t h2 = hash_combine(std::string("B"), std::string("A"));

        EXPECT_NE(h1, h2);
    }

    TEST(HashTest, VariadicArguments) {
        size_t h2 = hash_combine(1, 2);
        size_t h3 = hash_combine(1, 2, 3);
        size_t h4 = hash_combine(1, 2, 3, 4);

        EXPECT_NE(h2, h3);
        EXPECT_NE(h3, h4);
    }

    TEST(HashTest, RangeHashing) {
        std::vector<int> vec = { 1, 2, 3, 4, 5 };
        size_t seed1 = 0;
        hash_range(seed1, vec.begin(), vec.end());

        size_t seed2 = 0;
        hash_append(seed2, 1, 2, 3, 4, 5);

        EXPECT_EQ(seed1, seed2);
    }

    TEST(HashTest, EmptyRange) {
        std::vector<int> empty_vec;
        size_t seed = 12345;
        hash_range(seed, empty_vec.begin(), empty_vec.end());

        EXPECT_EQ(seed, 12345);
    }

    TEST(HashTest, CollisionResistanceBasic) {
        std::unordered_set<size_t> hashes;

        hashes.insert(hash_combine(1, 2));
        hashes.insert(hash_combine(2, 1));
        hashes.insert(hash_combine(1, 1));
        hashes.insert(hash_combine(0, 0));

        EXPECT_EQ(hashes.size(), 4);
    }

    TEST(TraitsTest, ArithmeticEqualityCorrected) {
        constexpr bool should_be_false = is_eq_arithmetic_v<10.5, 10.0, 0.1>;
        EXPECT_FALSE(should_be_false);

        constexpr bool should_be_true = is_eq_arithmetic_v<10.05, 10.0, 0.1>;
        EXPECT_TRUE(should_be_true);
    }

    TEST(TraitsTest, IsInEmptyList) {
        constexpr bool in_empty = is_same_type_in_v<int, float>;
        EXPECT_FALSE(in_empty);

        constexpr bool val_in_empty = is_eq_value_in_v<10, 1>;
        EXPECT_FALSE(val_in_empty);
    }

    TEST(TraitsTest, UniqueTypesFinal) {
        using TestList = unique_types<int, float, int, char, float, int>;

        static_assert(TestList::types_num == 3, "Powinny zostaæ 3 unikalne typy");

        bool is_valid = std::is_same_v<TestList, types_holder<int, float, char>>;
        EXPECT_TRUE(is_valid);
        is_valid = std::is_same_v<TestList::as_tuple, std::tuple<int, float, char>>;
        EXPECT_TRUE(is_valid);
    }

    static_assert(is_in_range_v<5, 0, 10>, "5 jest w [0, 10]");
    static_assert(is_in_range_v<0, 0, 10>, "Granica dolna");
    static_assert(is_in_range_v<10, 0, 10>, "Granica górna");
    static_assert(!is_in_range_v<-1, 0, 10>, "Poza zakresem");

#if _MSTD_HAS_CXX20
    TEST(TraitsTest, Concepts) {
        EXPECT_TRUE(integral<size_t>);
        EXPECT_TRUE(signed_integral<int>);
        EXPECT_FALSE(signed_integral<unsigned int>);
        EXPECT_TRUE(floating_point<long double>);
    }
#endif

    class IdManagerTest : public ::testing::Test {
    protected:
        id32_manager manager;
    };

    TEST_F(IdManagerTest, SmallestIdFirstReuse) {
        std::ignore = manager.get_next_id(); // 0
        std::ignore = manager.get_next_id(); // 1
        std::ignore = manager.get_next_id(); // 2
        std::ignore = manager.get_next_id(); // 3

        manager.return_id(2);
        manager.return_id(0);
        manager.return_id(1);

        EXPECT_EQ(manager.get_next_id(), 0);
        EXPECT_EQ(manager.get_next_id(), 1);
        EXPECT_EQ(manager.get_next_id(), 2);
        EXPECT_EQ(manager.get_next_id(), 4);
    }

    TEST_F(IdManagerTest, ComplexUpdateRemovedIds) {
        for (int i = 0; i < 5; ++i) std::ignore = manager.get_next_id();

        manager.return_id(1);
        manager.return_id(3);

        manager.return_id(4);

        EXPECT_EQ(manager.get_next_id(), 1);
        EXPECT_EQ(manager.get_next_id(), 3);
        EXPECT_EQ(manager.get_next_id(), 4);
    }

    TEST_F(IdManagerTest, BoundaryReturns) {
        std::ignore = manager.get_next_id(); // 0

        EXPECT_FALSE(manager.return_id(1));

        EXPECT_FALSE(manager.return_id(100));
    }

    TEST(IdManagerStaticTest, BadIdDefinition) {
        EXPECT_EQ(id8_manager::bad_id(), 255);
        EXPECT_EQ(id16_manager::bad_id(), 65535);
    }

    TEST_F(IdManagerTest, ResetAndPersistence) {
        for (int i = 0; i < 10; ++i) std::ignore = manager.get_next_id();
        manager.return_id(5);

        manager.reset();

        EXPECT_EQ(manager.get_next_id(), 0);
        EXPECT_EQ(manager.get_next_id(), 1);
    }
}