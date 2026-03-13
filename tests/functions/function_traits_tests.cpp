#include <mstd/function_traits.hpp>
#include <gtest/gtest.h>
#include <string>

namespace mstd::test {
    // Funkcje testowe
    int test_func(const std::string&, int&&, double) { return 0; }
    void test_noexcept() noexcept {}

    struct Member {
        void method() {}
        void const_method() const {}
        const int& get_val(float) const { static int i = 0; return i; }
        void volatile_method() volatile {}
    };

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, ReturnTypes) {
        using M = decltype(&Member::get_val);

        static_assert(std::is_same_v<function_return_t<M>, const int&>);
        static_assert(std::is_same_v<function_decayed_return_t<M>, int>);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, ArgumentTypes) {
        using F = decltype(&test_func);

        static_assert(function_args_num_v<F> == 3);
        static_assert(std::is_same_v<function_arg_t<F, 0>, const std::string&>);
        static_assert(std::is_same_v<function_decayed_arg_t<F, 0>, std::string>);

        static_assert(std::is_same_v<function_arg_t<F, 1>, int&&>);
        static_assert(std::is_same_v<function_decayed_arg_t<F, 1>, int>);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, TupleAliases) {
        using F = decltype(&test_func);

        using FullTuple = function_args_t<F>;
        using DecayedTuple = function_decayed_args_t<F>;

        EXPECT_TRUE((std::is_same_v<FullTuple, std::tuple<const std::string&, int&&, double>>));
        EXPECT_TRUE((std::is_same_v<DecayedTuple, std::tuple<std::string, int, double>>));
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, PropertyFlags) {
        using F_ptr = decltype(&test_func);
        using F_noexcept = decltype(&test_noexcept);
        using M_const = decltype(&Member::const_method);
        using M_vol = decltype(&Member::volatile_method);

        EXPECT_TRUE(is_function_ptr_v<F_ptr>);
        EXPECT_TRUE(is_free_function_v<F_ptr>);
        EXPECT_FALSE(is_noexcept_function_v<F_ptr>);

        EXPECT_TRUE(is_noexcept_function_v<F_noexcept>);

        EXPECT_TRUE(is_member_function_v<M_const>);
        EXPECT_TRUE(is_const_function_v<M_const>);
        EXPECT_FALSE(is_free_function_v<M_const>);

        EXPECT_TRUE(is_volatile_function_v<M_vol>);
        EXPECT_TRUE((std::is_same_v<function_parent_t<M_const>, Member>));
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, FunctorsAndLambdas) {
        auto lambda = [](int a, double b) { return a + static_cast<int>(b); };
        using L = decltype(lambda);

        EXPECT_TRUE(is_functor_v<L>);
        EXPECT_FALSE(is_function_ptr_v<L>);
        EXPECT_EQ(function_args_num_v<L>, 2);
        EXPECT_TRUE((std::is_same_v<function_arg_t<L, 1>, double>));

        EXPECT_TRUE(is_const_function_v<L>);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, StdFunctionSupport) {
        using StdFunc = std::function<void(int, char)>;

        EXPECT_TRUE(is_std_function_v<StdFunc>);
        EXPECT_TRUE(is_functor_v<StdFunc>);
        EXPECT_EQ(function_args_num_v<StdFunc>, 2);
        EXPECT_TRUE((std::is_same_v<function_return_t<StdFunc>, void>));
    }

#if _MSTD_HAS_CXX20
    TEST(FUNCTIONS_FunctionTraitsRevisedTest, ConceptsTest) {
        static_assert(member_function<decltype(&Member::method)>);
        static_assert(free_function<void(*)()>);
        static_assert(functor<decltype([](){})>);
    }
#endif
}