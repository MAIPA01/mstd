/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "functions_libs.hpp"
#include "as_function.hpp"

namespace mstd {
	// main template - default false
	template<class F, class = void>
	struct is_callable : std::false_type {};

	// for std::function
	template<class R, class... Args>
	struct is_callable<std::function<R(Args...)>> : std::true_type {};

	// for other types
	template<class F>
	struct is_callable<F, std::void_t<as_function_t<F>>> : is_callable<as_function_t<F>> {};

	// helper alias
	template<class F>
	constexpr bool is_callable_v = is_callable<F>::value;
}