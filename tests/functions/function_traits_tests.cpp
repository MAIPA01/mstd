#include <mstd/function_traits.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    int test_func(const std::string&, int&&, double) { return 0; }

    struct Member {
        void method() {}
        void const_method() const {}
        const int& get_val(float) const { static int i = 0; return i; }
    };

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, OriginalVsDecayedReturn) {
        using M = decltype(&Member::get_val);

        static_assert(std::is_same_v<function_return_t<M>, const int&>);
        static_assert(std::is_same_v<function_decayed_return_t<M>, int>);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, OriginalVsDecayedArgs) {
        using F = decltype(&test_func);

        static_assert(std::is_same_v<function_arg_t<F, 0>, const std::string&>);
        static_assert(std::is_same_v<function_decayed_arg_t<F, 0>, std::string>);

        static_assert(std::is_same_v<function_arg_t<F, 1>, int&&>);
        static_assert(std::is_same_v<function_decayed_arg_t<F, 1>, int>);

        static_assert(std::is_same_v<function_arg_t<F, 2>, double>);
        static_assert(std::is_same_v<function_decayed_arg_t<F, 2>, double>);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, ArgsNumCorrectness) {
        auto lambda = [](int a, int b, int c) { return a + b + c; };
        EXPECT_EQ(function_args_num_v<decltype(lambda)>, 3);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, TupleAliases) {
        using F = decltype(&test_func);

        using FullTuple = function_args_t<F>;
        using DecayedTuple = function_decayed_args_t<F>;

        bool is_full_correct = std::is_same_v<FullTuple, std::tuple<const std::string&, int&&, double>>;
        bool is_decay_correct = std::is_same_v<DecayedTuple, std::tuple<std::string, int, double>>;

        EXPECT_TRUE(is_full_correct);
        EXPECT_TRUE(is_decay_correct);
    }

    TEST(FUNCTIONS_FunctionTraitsRevisedTest, LambdaWithAutoCapture) {
        std::string secret = "password";
        auto lambda = [secret](const std::string& input) { return input == secret; };

        using L = decltype(lambda);

        EXPECT_EQ(function_args_num_v<L>, 1);
        EXPECT_TRUE((std::is_same_v<function_arg_t<L, 0>, const std::string&>));
        EXPECT_TRUE((std::is_same_v<function_return_t<L>, bool>));
    }
}