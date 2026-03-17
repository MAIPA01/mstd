/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "is_callable.hpp"


#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include "as_std_function.hpp"

namespace mstd {
	#pragma region FUNCTIONS_CHECKS
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_function_v = is_callable_v<F> && (is_free_function_v<F> || is_member_function_v<F>) && !is_functor_v<F> && !is_std_function_v<F>;

	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_action_v = is_function_v<F> && std::is_void_v<function_return_t<F>>;

	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_method_v = is_action_v<F> && function_args_num_v<F> == 0;

	#if _MSTD_HAS_CXX20
	template<class F> concept func = is_function_v<F>;
	template<class F> concept action = is_action_v<F>;
	template<class F> concept method = is_method_v<F>;
	#endif
	#pragma endregion

	#pragma region CPP_FUNCTIONS
	template<class F>
	using func_t = as_std_function_t<F>;

	template<class... Args> 
	using action_t = func_t<void(Args...)>;

	using method_t = action_t<>;
	#pragma endregion

	#pragma region C_FUNCTIONS
	template<class F, class C, class = void>
	struct _c_func_impl {};

	template<class F, class C>
	struct _c_func_impl<F, C, std::void_t<std::enable_if_t<mstd::is_function_v<F>, bool>>> {
		using type = F C::*;
	};

	template<class F>
	struct _c_func_impl<F, void, std::void_t<std::enable_if_t<mstd::is_function_v<F>, bool>>> {
		using type = F*;
	};

	template<class F>
	using c_func_t = typename _c_func_impl<F, void>::type;

	template<class C, class F>
	using c_member_func_t = typename _c_func_impl<F, C>::type;

	template<class... Args>
	using c_action_t = c_func_t<void(Args...)>;

	template<class C, class... Args>
	using c_member_action_t = c_member_func_t<C, void(Args...)>;

	using c_method_t = c_action_t<>;

	template<class C>
	using c_member_method_t = c_member_action_t<C>;
	#pragma endregion

}
#endif