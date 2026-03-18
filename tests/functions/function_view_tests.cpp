#include <gtest/gtest.h>
#include <mstd/function_view.hpp>

namespace mstd::tests {
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

		function_view<int(int)> fv2 = free_func;
		EXPECT_EQ(fv2(5), 10);
	}

	TEST(FunctionViewTest, LambdaSupport) {
		auto lambda = [](int x) { return x + 1; };
		function_view<int(int)> fv(lambda);
		EXPECT_EQ(fv(10), 11);

		auto other_lambda = [](int x) { return x + 2; };
		fv = other_lambda;
		EXPECT_EQ(fv(10), 12);

		int offset = 100;
		auto stateful = [offset](int x) { return x + offset; };

		function_view<int(int)> fv_stateful(stateful);
		EXPECT_EQ(fv_stateful(5), 105);
	}

	TEST(FunctionViewTest, NoexceptConstraint) {
		using fv_noexcept = function_view<int(int) noexcept>;

		fv_noexcept v1(free_func_noexcept);
		EXPECT_EQ(v1(5), 10);

		static_assert(!std::is_constructible_v<fv_noexcept, decltype(free_func)>,
			"Should not allow non-noexcept function in noexcept view");

		static_assert(utils::is_valid_free_function_v<decltype(free_func_noexcept), int(int) noexcept, int, int>);
		static_assert(!utils::is_valid_free_function_v<decltype(free_func), int(int) noexcept, int, int>);
	}

	TEST(FunctionViewTest, ConstMemberFunction) {
		Mock m;

		using f_const_sig = int(int) const;
		static_assert(is_same_function_v<decltype(&Mock::mem_const), f_const_sig>);

		m.val = 50;

		using Sig = int(int);
		auto pair = std::make_pair(&m, &Mock::mem_const);

		function_view<Sig> fv(pair);
		EXPECT_EQ(fv(10), 60);

		m.val = 100;
		EXPECT_EQ(fv(10), 110);
	}

	TEST(FunctionViewTest, CopyAndMoveSemantics) {
		function_view<int(int)> v1 = free_func;

		// Copy
		function_view<int(int)> v2 = v1;
		EXPECT_EQ(v2(5), 10);

		// Move
		function_view<int(int)> v3 = std::move(v2);
		EXPECT_EQ(v3(10), 20);

		EXPECT_EQ(v1(5), 10);
	}

	TEST(FunctionViewTest, Reassignment) {
		function_view<int(int)> fv = free_func;
		EXPECT_EQ(fv(5), 10);

		auto lambda = [](int x) { return x * x; };

		fv = lambda;
		EXPECT_EQ(fv(5), 25);
	}

	TEST(FunctionViewTest, ConstAndVolatileValidation) {
		using ValidSig = int(int);
		using InvalidSig = int(int) const;
		using VolatileSig = int(int) volatile;

		static_assert(utils::is_valid_function_view_function_v<ValidSig>);
		static_assert(!utils::is_valid_function_view_function_v<InvalidSig>,
			"function_view should not allow const-qualified function types as its template argument");
		static_assert(!utils::is_valid_function_view_function_v<VolatileSig>,
			"function_view should not allow volitale-qualified function types as its template argument");
	}

	TEST(FunctionViewTest, MemberFunctionNoexceptValidation) {
		using SigNoexcept = int(int) noexcept;

		static_assert(utils::is_valid_member_function_v<decltype(&Mock::mem_noexcept), SigNoexcept, int, int>);

		static_assert(!utils::is_valid_member_function_v<decltype(&Mock::mem_const), SigNoexcept, int, int>);
	}
}