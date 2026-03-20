#include <gtest/gtest.h>
#include <mstd/is_same_function.hpp>

namespace mstd::test {
	void func_v_v() {}

	int func_i_i(int a) { return a; }

	void func_v_i(int) {}

	struct Member {
		void method() {}

		void const_method() const {}

		int const& get_val(float) const {
			static int i = 0;
			return i;
		}
	};

	struct Functor {
		void operator()(int) {}
	};

	static_assert(is_same_function_v<void (*)(), void (*)()> == true);
	static_assert(is_same_function_v<std::function<void()>, std::function<void()>> == true);

	static_assert(is_same_function_v<void (*)(), std::function<void()>> == true);

#if _MSTD_HAS_CXX20
	using LambdaSimple = decltype([](int) -> void {});
#else
	inline auto lambdaSimple = [](int) -> void {};
	using LambdaSimple		 = decltype(lambdaSimple);
#endif
	static_assert(is_same_function_v<LambdaSimple, int (*)(int)> == false);
	static_assert(is_same_function_v<LambdaSimple, void (*)(int)> == true);

	static_assert(is_same_function_v<decltype(&Member::method), void (*)()> == true);
	static_assert(is_same_function_v<decltype(&Member::const_method), Functor> == false);

	TEST(FUNCTIONS_IsSameFunctionTest, SameSignaturesDifferentCallables) {
		auto lambda		  = []() { return 1; };
		using LambdaType  = decltype(lambda);
		using FuncPtrType = int (*)();
		using StdFuncType = std::function<int()>;

		EXPECT_TRUE((is_same_function_v<LambdaType, FuncPtrType>));
		EXPECT_TRUE((is_same_function_v<LambdaType, StdFuncType>));
		EXPECT_TRUE((is_same_function_v<FuncPtrType, StdFuncType>));
	}

	TEST(FUNCTIONS_IsSameFunctionTest, DifferentSignatures) {
		auto lambda_v_v = []() {};
		auto lambda_i_v = [](int) {};

		EXPECT_FALSE((is_same_function_v<decltype(lambda_v_v), decltype(lambda_i_v)>));
		EXPECT_FALSE((is_same_function_v<decltype(lambda_v_v), int (*)()>));
	}

	TEST(FUNCTIONS_IsSameFunctionTest, HandleNoexcept) {
		auto normal_func   = []() {};
		auto noexcept_func = []() noexcept {};

		EXPECT_TRUE((is_same_function_v<decltype(normal_func), decltype(noexcept_func)>));
	}

	TEST(FUNCTIONS_IsSameFunctionTest, NonFunctionTypes) {
		EXPECT_FALSE((is_same_function_v<int, float>));
		EXPECT_FALSE((is_same_function_v<int, void (*)()>));
	}
} // namespace mstd::test