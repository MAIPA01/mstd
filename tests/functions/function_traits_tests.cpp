#include <gtest/gtest.h>
#include <mstd/function_traits.hpp>
#include <string>

namespace mstd::test {
	// Funkcje testowe
	int test_func(std::string const&, int&&, double) { return 0; }

	void test_noexcept() noexcept {}

	struct Member {
		void method() {}

		void const_method() const {}

		int const& get_val(float) const {
			static int i = 0;
			return i;
		}

		void volatile_method() volatile {}

		void ref_method() & {}

		void rvalue_method() && {}
	};

	TEST(FUNCTIONS_FunctionTraitsRevisedTest, ReturnTypes) {
		using M = decltype(&Member::get_val);

		static_assert(std::is_same_v<function_return_t<M>, int const&>);
		static_assert(std::is_same_v<function_decayed_return_t<M>, int>);
	}

	TEST(FUNCTIONS_FunctionTraitsRevisedTest, ArgumentTypes) {
		using F = decltype(&test_func);

		static_assert(function_args_num_v<F> == 3);
		static_assert(std::is_same_v<function_arg_t<F, 0>, std::string const&>);
		static_assert(std::is_same_v<function_decayed_arg_t<F, 0>, std::string>);

		static_assert(std::is_same_v<function_arg_t<F, 1>, int&&>);
		static_assert(std::is_same_v<function_decayed_arg_t<F, 1>, int>);
	}

	TEST(FUNCTIONS_FunctionTraitsRevisedTest, TupleAliases) {
		using F			   = decltype(&test_func);

		using FullTuple	   = function_args_t<F>;
		using DecayedTuple = function_decayed_args_t<F>;

		EXPECT_TRUE((std::is_same_v<FullTuple, std::tuple<std::string const&, int&&, double>>));
		EXPECT_TRUE((std::is_same_v<DecayedTuple, std::tuple<std::string, int, double>>));
	}

	TEST(FUNCTIONS_FunctionTraitsRevisedTest, PropertyFlags) {
		using F_ptr		 = decltype(&test_func);
		using F_noexcept = decltype(&test_noexcept);
		using M_const	 = decltype(&Member::const_method);
		using M_vol		 = decltype(&Member::volatile_method);

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
		using L		= decltype(lambda);

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
		static_assert(free_function<void (*)()>);
		static_assert(functor<decltype([]() {})>);
	}
#endif

	TEST(FunctionTraitsTest, FreeFunctions) {
		using Sig = void(int) noexcept;

		static_assert(std::is_same_v<function_type_t<Sig>, void(int) noexcept>);

		static_assert(std::is_same_v<core_function_type_t<Sig>, void(int)>);
	}

	TEST(FunctionTraitsTest, MemberFunctionsQualifiers) {
		using ConstMem = decltype(&Member::get_val);
		static_assert(std::is_same_v<function_type_t<ConstMem>, int const&(float) const>);

		static_assert(std::is_same_v<core_function_type_t<ConstMem>, int const&(float)>);
	}

	TEST(FunctionTraitsTest, VolatileAndRefQualifiers) {
		using VolatileMem = decltype(&Member::volatile_method);
		static_assert(std::is_same_v<function_type_t<VolatileMem>, void() volatile>);
		static_assert(std::is_same_v<core_function_type_t<VolatileMem>, void()>);

		using RefMem = decltype(&Member::ref_method);
		static_assert(std::is_same_v<function_type_t<RefMem>, void() &>);
		static_assert(std::is_same_v<core_function_type_t<RefMem>, void()>);

		using RvalueMem = decltype(&Member::rvalue_method);
		static_assert(std::is_same_v<function_type_t<RvalueMem>, void() &&>);
		static_assert(std::is_same_v<core_function_type_t<RvalueMem>, void()>);
	}

	TEST(FunctionTraitsTest, ComplexCombinedQualifiers) {
		using ComplexMem = void (Member::*)(int) const volatile noexcept;

		static_assert(std::is_same_v<function_type_t<ComplexMem>, void(int) const volatile noexcept>);
		static_assert(std::is_same_v<core_function_type_t<ComplexMem>, void(int)>);
	}

	TEST(FunctionTraitsTest, FunctorsAndLambdas) {
		auto lambda		 = [](int x) mutable noexcept -> double { return x * 2.0; };
		using LambdaType = decltype(lambda);

		static_assert(std::is_same_v<function_type_t<LambdaType>, double(int) noexcept>);
		static_assert(std::is_same_v<core_function_type_t<LambdaType>, double(int)>);
	}

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
		using T1 = as_std_function_t<decltype(&void_func_no_args)>;
		static_assert(std::is_same_v<T1, std::function<void()>>);

		using T2 = as_std_function_t<decltype(&int_func_int_arg)>;
		static_assert(std::is_same_v<T2, std::function<int(int)>>);

		using T3 = as_std_function_t<decltype(&void_func_noexcept)>;
		static_assert(std::is_same_v<T3, std::function<void()>>);
	}

	TEST(FUNCTIONS_AsFunctionTest, Lambdas) {
		auto lambda = [](int a, double) { return a; };
		using T1	= as_std_function_t<decltype(lambda)>;
		static_assert(std::is_same_v<T1, std::function<int(int, double)>>);

		int x				 = 10;
		auto stateful_lambda = [x](int y) { return x + y; };
		using T2			 = as_std_function_t<decltype(stateful_lambda)>;
		static_assert(std::is_same_v<T2, std::function<int(int)>>);

		auto mutable_lambda = [x]() mutable { return ++x; };
		using T3			= as_std_function_t<decltype(mutable_lambda)>;
		static_assert(std::is_same_v<T3, std::function<int()>>);
	}

	TEST(FUNCTIONS_AsFunctionTest, MemberFunctionPointers) {
		using T1 = as_std_function_t<decltype(&MemberTester::method)>;
		static_assert(std::is_same_v<T1, std::function<void(int)>>);

		using T2 = as_std_function_t<decltype(&MemberTester::const_method)>;
		static_assert(std::is_same_v<T2, std::function<int(double)>>);

		using T3 = as_std_function_t<decltype(&MemberTester::noexcept_method)>;
		static_assert(std::is_same_v<T3, std::function<void()>>);

		using T4 = as_std_function_t<decltype(&MemberTester::volatile_method)>;
		static_assert(std::is_same_v<T4, std::function<void()>>);
	}

	TEST(FUNCTIONS_AsFunctionTest, Functors) {
		using T1 = as_std_function_t<Functor>;
		static_assert(std::is_same_v<T1, std::function<void(int)>>);

		using T2 = as_std_function_t<ConstFunctor>;
		static_assert(std::is_same_v<T2, std::function<void(int)>>);
	}

	TEST(FUNCTIONS_AsFunctionTest, StdFunctionIdentity) {
		using Orig = std::function<void(float)>;
		using Res  = as_std_function_t<Orig>;
		static_assert(std::is_same_v<Orig, Res>);
	}

	struct OverloadedFunctor {
		void operator()(int) {}

		int operator()(double, double) { return 0; }

		void operator()() const {}
	};

	TEST(FUNCTIONS_AsFunctionOverloadTest, ManualDisambiguation) {
		using Sig1 = void (OverloadedFunctor::*)(int);
		using Sig2 = int (OverloadedFunctor::*)(double, double);
		using Sig3 = void (OverloadedFunctor::*)() const;

		using T1   = as_std_function_t<decltype(static_cast<Sig1>(&OverloadedFunctor::operator()))>;
		using T2   = as_std_function_t<decltype(static_cast<Sig2>(&OverloadedFunctor::operator()))>;
		using T3   = as_std_function_t<decltype(static_cast<Sig3>(&OverloadedFunctor::operator()))>;

		static_assert(std::is_same_v<T1, std::function<void(int)>>);
		static_assert(std::is_same_v<T2, std::function<int(double, double)>>);
		static_assert(std::is_same_v<T3, std::function<void()>>);
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

	static_assert(is_callable_v<void (*)()>);
	static_assert(is_callable_v<std::function<void()>>);
	static_assert(is_callable_v<Functor>);
#if _MSTD_HAS_CXX20
	static_assert(is_callable_v<decltype([]() -> void {})>);
#else
	inline auto voidLambda = []() -> void {};
	static_assert(is_callable_v<decltype(voidLambda)>);
#endif
	static_assert(is_callable_v<decltype(&MemberClass::method)>);

	static_assert(!is_callable_v<int>);
	static_assert(!is_callable_v<NonCallable>);
	static_assert(!is_callable_v<std::string>);

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
		EXPECT_TRUE(is_callable_v<std::function<int(double, char)>>);

		using NoexceptFunc = void (*)() noexcept;
		EXPECT_TRUE(is_callable_v<NoexceptFunc>);
	}

	TEST(IsCallableTest, OverloadedFunctor) { EXPECT_FALSE(is_callable_v<Overloaded>); }
} // namespace mstd::test