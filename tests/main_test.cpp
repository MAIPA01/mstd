#define USE_FOR_EACH_MACROS
#define USE_ENUMS_MACROS
#define USE_CLONE_FUNC_MACROS
#define USE_EXTRA_MACROS
#include <mstd/macros.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
#define TEST_TUPLE (A, B, C)
#define STRINGIFY(x) #x
#define TO_STR(x) STRINGIFY(x)

    TEST(MacroTuplesTest, BreakAndPop) {
        // Test BREAK_TUPLE
        EXPECT_STREQ(TO_STR(BREAK_TUPLE(MAKE_TUPLE(1))), "1");

        // Test POP_TUPLE_FRONT
        // (A, B, C) -> (B, C)
        EXPECT_STREQ(TO_STR(POP_TUPLE_FRONT(TEST_TUPLE)), "(B, C)");
    }

    TEST(MacroTuplesTest, AppendOperations) {
        // Test APPEND_TUPLE_FRONT: (B, C) + A -> (A, B, C)
        auto front_append = TO_STR(APPEND_TUPLE_FRONT((B, C), A));
        EXPECT_STREQ(front_append, "(A, B, C)");

        // Test APPEND_TUPLE_BACK: (A, B) + C -> (A, B, C)
        auto back_append = TO_STR(APPEND_TUPLE_BACK((A, B), C));
        EXPECT_STREQ(back_append, "(A, B, C)");
    }

    ENUM_CLASS(TaskStatus, (Pending), (Running), (Completed))

    ENUM_CLASS_BASE_VALUE_STRING(HttpCode, int,
        (Ok, 200, "OK"),
        (NotFound, 404, "Not Found"),
        (InternalError, 500, "Server Error")
    )

    TEST(MacroEnumTest, SizeAndValues) {
        EXPECT_EQ(size<TaskStatus>(), 3);
        EXPECT_EQ(size<HttpCode>(), 3);
        EXPECT_EQ(static_cast<int>(HttpCode::NotFound), 404);
    }

    TEST(MacroEnumTest, ToStringAndStream) {
        EXPECT_EQ(to_string(TaskStatus::Running), "Running");

        EXPECT_EQ(to_string(HttpCode::NotFound), "Not Found");

        std::stringstream ss;
        ss << HttpCode::Ok;
        EXPECT_EQ(ss.str(), "OK");
    }

    TEST(MacroEnumTest, FromStringSwitch) {
        EXPECT_EQ(from_string<TaskStatus>("Pending"), TaskStatus::Pending);
        EXPECT_EQ(from_string<HttpCode>("Server Error"), HttpCode::InternalError);

        EXPECT_THROW(from_string<HttpCode>("Invalid"), std::invalid_argument);
    }

    class Weapon {
    public:
        std::string name = "Sword";
        int damage = 10;

        virtual ~Weapon() = default;

        DECLARE_CLONE_FUNC_WITH_DEFINITION(Weapon, name, damage)
    };

    class EnchantedWeapon : public Weapon {
    public:
        int mana_cost = 5;

        DECLARE_CLONE_BASE_FUNC_WITH_DEFINITION_ADVANCED(EnchantedWeapon, Weapon,
            MAKE_TUPLE(mana_cost, mana_cost)
        )
    };

    TEST(MacroCloneTest, DeepCopyValidation) {
        EnchantedWeapon* original = new EnchantedWeapon();
        original->name = "FireStaff";
        original->mana_cost = 50;

        EnchantedWeapon* copy = original->Clone();

        ASSERT_NE(copy, nullptr);
        EXPECT_EQ(copy->name, "FireStaff");
        EXPECT_EQ(copy->mana_cost, 50);
        EXPECT_EQ(copy->damage, 10);

        original->mana_cost = 100;
        EXPECT_EQ(copy->mana_cost, 50);

        delete original;
        delete copy;
    }

    TEST(MacroForEachTest, ListIteration) {
        std::vector<std::string> v;
#define ADD_TO_VEC(x) v.push_back(#x);

        DO_FOR_EACH(ADD_TO_VEC, Item1, Item2, Item3)

            ASSERT_EQ(v.size(), 3);
        EXPECT_EQ(v[1], "Item2");
    }
}