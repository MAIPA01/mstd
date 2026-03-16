#include <gtest/gtest.h>
#include <mstd/function_view.hpp>

using namespace mstd;

int free_func(int x) { return x * 2; }
int free_func_noexcept(int x) noexcept { return x * 2; }

struct Mock {
    int val = 10;
    int mem_const(int x) const { return val + x; }
    int mem_noexcept(int x) noexcept { return val + x; }
    int mem_volatile(int x) volatile { return val + x; }
    int mem_ref(int x) & { return val + x; }
};

TEST(FunctionViewTest, FreeFunction) {
    function_view<int(int)> fv(free_func);
    EXPECT_EQ(fv(5), 10);
}

TEST(FunctionViewTest, LambdaSupport) {
    auto lambda = [](int x) { return x + 1; };
    function_view<int(int)> fv(lambda);
    EXPECT_EQ(fv(10), 11);
}

TEST(FunctionViewTest, NoexceptConstraint) {
    using fv_noexcept = function_view<int(int) noexcept>;

    fv_noexcept v1(free_func_noexcept);
    EXPECT_EQ(v1(5), 10);

    static_assert(!std::is_constructible_v<fv_noexcept, decltype(free_func)>,
        "Should not allow non-noexcept function in noexcept view");
}

TEST(FunctionViewTest, ConstMemberFunction) {
    Mock m;

    using f_const_sig = int(int) const;
    static_assert(is_same_function_v<decltype(&Mock::mem_const), f_const_sig>);
}

TEST(FunctionViewTest, RefQualifierConstraint) {
    using f_ref_sig = int(int) &;
    using f_rvalue_sig = int(int) &&;

    static_assert(std::is_invocable_v<f_ref_sig, Mock, int>);
}

TEST(FunctionViewTest, StatefulFunctor) {
    int offset = 100;
    auto stateful = [offset](int x) { return x + offset; };

    function_view<int(int)> fv(stateful);
    EXPECT_EQ(fv(5), 105);
}

TEST(FunctionViewTest, VolatileSupport) {
    volatile Mock m;
    using f_volatile_sig = int(int) volatile;

    static_assert(is_same_function_v<decltype(&Mock::mem_volatile), f_volatile_sig>);
}