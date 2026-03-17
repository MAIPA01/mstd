#include <mstd/functions_types.hpp>
#include <gtest/gtest.h>
#include <string>
#include <type_traits>

namespace mstd::test {
    struct TestClass {
        void void_method() {}
        int int_method(double) { return 42; }
        static void static_method() {}
    };

    void global_action(int) {}
    int global_func(const std::string&) { return 0; }

    // --- TESTY CPP_FUNCTIONS ---

    TEST(FUNCTIONS_CoreTest, CppFunctionAliases) {
        // func_t
        static_assert(std::is_same_v<func_t<int(double)>, std::function<int(double)>>);

        // action_t
        static_assert(std::is_same_v<action_t<int, float>, std::function<void(int, float)>>);

        // method_t
        static_assert(std::is_same_v<method_t, std::function<void()>>);
    }

    // --- TESTY C_FUNCTIONS ---

    TEST(FUNCTIONS_CoreTest, CFunctionAliases) {
        using RawFunc = c_func_t<int(double)>;
        static_assert(std::is_same_v<RawFunc, int(*)(double)>);

        using RawMember = c_member_func_t<TestClass, void()>;
        static_assert(std::is_same_v<RawMember, void(TestClass::*)()>);

        static_assert(std::is_same_v<c_action_t<int>, void(*)(int)>);
        static_assert(std::is_same_v<c_method_t, void(*)()>);
    }

    // --- TESTY FUNCTIONS_CHECKS ---

    TEST(FUNCTIONS_CoreTest, FunctionPredicates) {
    	auto lambda = [](int) -> void {};

        using GlobalFuncPtr = decltype(&global_func);
        using MemberFuncPtr = decltype(&TestClass::int_method);
        using LambdaType = decltype(lambda);
        using StdFuncType = std::function<void()>;

        EXPECT_TRUE(is_function_v<GlobalFuncPtr>);
        EXPECT_TRUE(is_function_v<MemberFuncPtr>);

        EXPECT_FALSE(is_function_v<LambdaType>);
        EXPECT_FALSE(is_function_v<StdFuncType>);
        EXPECT_FALSE(is_function_v<int>);

        EXPECT_TRUE(is_action_v<decltype(&global_action)>);
        EXPECT_TRUE(is_action_v<decltype(&TestClass::void_method)>);
        EXPECT_FALSE(is_action_v<GlobalFuncPtr>);

        EXPECT_TRUE(is_method_v<decltype(&TestClass::void_method)>);
        EXPECT_TRUE(is_method_v<decltype(&TestClass::static_method)>);
        EXPECT_FALSE(is_method_v<decltype(&global_action)>);
    }

	TEST(FUNCTIONS_UpdateTest, MemberFunctionPointers) {
    	struct Player {
    		void jump() {}
    		void move(int, int) {}
    	};

    	using JumpPtr = c_member_method_t<Player>;
    	static_assert(std::is_same_v<JumpPtr, void(Player::*)()>);

    	using MovePtr = c_member_action_t<Player, int, int>;
    	static_assert(std::is_same_v<MovePtr, void(Player::*)(int, int)>);

    	EXPECT_TRUE(is_method_v<JumpPtr>);
    	EXPECT_TRUE(is_action_v<MovePtr>);
    }

	TEST(FUNCTIONS_UpdateTest, DefaultBehavior) {
    	static_assert(std::is_same_v<c_method_t, void(*)()>);
    	static_assert(std::is_same_v<c_action_t<float>, void(*)(float)>);
    }

    // --- TESTY C++20 CONCEPTS ---

#if _MSTD_HAS_CXX20
    TEST(FUNCTIONS_CoreTest, ConceptsCheck) {
        static_assert(func<int(*)(char)>);
        static_assert(action<void(*)(int)>);
        static_assert(method<void(*)()>);

        static_assert(!func<std::function<void()>>);
        static_assert(!method<void(*)(int)>);
    }
#endif

}