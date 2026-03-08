#include <mstd/is_callable.hpp>
#include <gtest/gtest.h>

namespace mstd::test {
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

    struct Functor {
        void operator()(int) {}
    };

    static_assert(is_callable_v<void(*)()> == true);
    static_assert(is_callable_v<std::function<void()>> == true);
    static_assert(is_callable_v<Functor> == true);
#if _MSTD_HAS_CXX20
    static_assert(is_callable_v<decltype([]() -> void {}) > == true);
#else
    inline auto voidLambda = []() -> void {};
    static_assert(is_callable_v<decltype(voidLambda)> == true);
#endif
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