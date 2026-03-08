#include <mstd/as_function.hpp>
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
}