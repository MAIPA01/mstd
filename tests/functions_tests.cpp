#include <mstd/functions.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
    void void_func_no_args() {}
    int int_func_int_arg(int x) { return x; }
    void void_func_noexcept() noexcept {}

    struct Functor {
        void operator()(int) {}
    };

    struct ConstFunctor {
        void operator()(int) const {}
    };

    struct MemberTester {
        void method(int) {}
        int const_method(double) const { return 0; }
        void noexcept_method() noexcept {}
        void volatile_method() volatile {}
    };

    TEST(FUNCTIONS_AsFunctionTest, FunctionPointers) {
        using T1 = as_function_t<decltype(&void_func_no_args)>;
        static_assert(std::is_same_v<T1, std::function<void()>>);

        using T2 = as_function_t<decltype(&int_func_int_arg)>;
        static_assert(std::is_same_v<T2, std::function<int(int)>>);

        using T3 = as_function_t<decltype(&void_func_noexcept)>;
        static_assert(std::is_same_v<T3, std::function<void()>>);
    }

    TEST(FUNCTIONS_AsFunctionTest, Lambdas) {
        auto lambda = [](int a, double) { return a; };
        using T1 = as_function_t<decltype(lambda)>;
        static_assert(std::is_same_v<T1, std::function<int(int, double)>>);

        int x = 10;
        auto stateful_lambda = [x](int y) { return x + y; };
        using T2 = as_function_t<decltype(stateful_lambda)>;
        static_assert(std::is_same_v<T2, std::function<int(int)>>);

        auto mutable_lambda = [x]() mutable { return ++x; };
        using T3 = as_function_t<decltype(mutable_lambda)>;
        static_assert(std::is_same_v<T3, std::function<int()>>);
    }

    TEST(FUNCTIONS_AsFunctionTest, MemberFunctionPointers) {
        using T1 = as_function_t<decltype(&MemberTester::method)>;
        static_assert(std::is_same_v<T1, std::function<void(int)>>);

        using T2 = as_function_t<decltype(&MemberTester::const_method)>;
        static_assert(std::is_same_v<T2, std::function<int(double)>>);

        using T3 = as_function_t<decltype(&MemberTester::noexcept_method)>;
        static_assert(std::is_same_v<T3, std::function<void()>>);

        using T4 = as_function_t<decltype(&MemberTester::volatile_method)>;
        static_assert(std::is_same_v<T4, std::function<void()>>);
    }

    TEST(FUNCTIONS_AsFunctionTest, Functors) {
        using T1 = as_function_t<Functor>;
        static_assert(std::is_same_v<T1, std::function<void(int)>>);

        using T2 = as_function_t<ConstFunctor>;
        static_assert(std::is_same_v<T2, std::function<void(int)>>);
    }

    TEST(FUNCTIONS_AsFunctionTest, StdFunctionIdentity) {
        using Orig = std::function<void(float)>;
        using Res = as_function_t<Orig>;
        static_assert(std::is_same_v<Orig, Res>);
    }

    TEST(FUNCTIONS_AsFunctionTest, CompileTimeValidation) {
        SUCCEED();
    }

    struct OverloadedFunctor {
        void operator()(int) {}
        int operator()(double, double) { return 0; }
        void operator()() const {}
    };

    TEST(FUNCTIONS_AsFunctionOverloadTest, ManualDisambiguation) {
        using Sig1 = void(OverloadedFunctor::*)(int);
        using Sig2 = int(OverloadedFunctor::*)(double, double);
        using Sig3 = void(OverloadedFunctor::*)() const;

        using T1 = as_function_t<decltype(static_cast<Sig1>(&OverloadedFunctor::operator()))>;
        using T2 = as_function_t<decltype(static_cast<Sig2>(&OverloadedFunctor::operator()))>;
        using T3 = as_function_t<decltype(static_cast<Sig3>(&OverloadedFunctor::operator()))>;

        static_assert(std::is_same_v<T1, std::function<void(int)>>);
        static_assert(std::is_same_v<T2, std::function<int(double, double)>>);
        static_assert(std::is_same_v<T3, std::function<void()>>);
    }

    void func_v_v() {}
    int func_i_i(int a) { return a; }
    void func_v_i(int) {}

    struct Member {
        void method() {}
        void const_method() const {}
        const int& get_val(float) const { static int i = 0; return i; }
    };

    static_assert(is_same_function_v<void(*)(), void(*)()> == true);
    static_assert(is_same_function_v<std::function<void()>, std::function<void()>> == true);

    static_assert(is_same_function_v<void(*)(), std::function<void()>> == true);

    using LambdaSimple = decltype([](int) {});
    static_assert(is_same_function_v<LambdaSimple, int(*)(int)> == false);
    static_assert(is_same_function_v<LambdaSimple, void(*)(int)> == true);

    static_assert(is_same_function_v<decltype(&Member::method), void(*)()> == true);
    static_assert(is_same_function_v<decltype(&Member::const_method), Functor> == false);

    TEST(FUNCTIONS_IsSameFunctionTest, SameSignaturesDifferentCallables) {
        auto lambda = []() { return 1; };
        using LambdaType = decltype(lambda);
        using FuncPtrType = int(*)();
        using StdFuncType = std::function<int()>;

        EXPECT_TRUE((is_same_function_v<LambdaType, FuncPtrType>));
        EXPECT_TRUE((is_same_function_v<LambdaType, StdFuncType>));
        EXPECT_TRUE((is_same_function_v<FuncPtrType, StdFuncType>));
    }

    TEST(FUNCTIONS_IsSameFunctionTest, DifferentSignatures) {
        auto lambda_v_v = []() {};
        auto lambda_i_v = [](int) {};

        EXPECT_FALSE((is_same_function_v<decltype(lambda_v_v), decltype(lambda_i_v)>));
        EXPECT_FALSE((is_same_function_v<decltype(lambda_v_v), int(*)()>));
    }

    TEST(FUNCTIONS_IsSameFunctionTest, HandleNoexcept) {
        auto normal_func = []() {};
        auto noexcept_func = []() noexcept {};

        EXPECT_TRUE((is_same_function_v<decltype(normal_func), decltype(noexcept_func)>));
    }

    TEST(FUNCTIONS_IsSameFunctionTest, NonFunctionTypes) {
        EXPECT_FALSE((is_same_function_v<int, float>));
        EXPECT_FALSE((is_same_function_v<int, void(*)()>));
    }

    int test_func(const std::string&, int&&, double) { return 0; }

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

    void plain_function() {}

    struct NonCallable {
        int x;
    };

    struct MemberClass {
        void method() {}
    };

    struct Overloaded {
        void operator()(int) {}
        void operator()(double) {}
    };

    static_assert(is_callable_v<void(*)()> == true);
    static_assert(is_callable_v<std::function<void()>> == true);
    static_assert(is_callable_v<Functor> == true);
    static_assert(is_callable_v<decltype([]() {}) > == true);
    static_assert(is_callable_v<decltype(&MemberClass::method)> == true);

    static_assert(is_callable_v<int> == false);
    static_assert(is_callable_v<NonCallable> == false);
    static_assert(is_callable_v<std::string> == false);

    TEST(IsCallableTest, BasicCallables) {
        EXPECT_TRUE(is_callable_v<decltype(&plain_function)>);
        EXPECT_TRUE(is_callable_v<Functor>);

        auto lambda = [](int a) { return a * a; };
        EXPECT_TRUE(is_callable_v<decltype(lambda)>);
    }

    TEST(IsCallableTest, NonCallables) {
        EXPECT_FALSE(is_callable_v<int>);
        EXPECT_FALSE(is_callable_v<MemberClass>);
        EXPECT_FALSE(is_callable_v<void*>);
    }

    TEST(IsCallableTest, MemberFunctions) {
        using MemPtr = decltype(&MemberClass::method);
        EXPECT_TRUE(is_callable_v<MemPtr>);
    }

    TEST(IsCallableTest, StdFunctionVariants) {
        EXPECT_TRUE((is_callable_v<std::function<int(double, char)>>));

        using NoexceptFunc = void(*)() noexcept;
        EXPECT_TRUE(is_callable_v<NoexceptFunc>);
    }

    TEST(IsCallableTest, OverloadedFunctor) {
        EXPECT_FALSE(is_callable_v<Overloaded>);
    }
}