#include <mstd/id_manager.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
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