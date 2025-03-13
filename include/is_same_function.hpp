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
	// Checking If functions are same
	template<class Fa, class Fb, class = void, class = void>
	struct is_same_function : std::false_type {};

	template<class F>
	struct is_same_function<F, F> : std::true_type {};

	template<class Fa, class Fb>
	struct is_same_function<Fa, Fb, std::void_t<as_function_t<Fa>>, std::void_t<as_function_t<Fb>>>
		: std::is_same<as_function_t<Fa>, as_function_t<Fb>> {
	};

	template<class Fa, class Fb>
	constexpr bool is_same_function_v = is_same_function<Fa, Fb>::value;
}