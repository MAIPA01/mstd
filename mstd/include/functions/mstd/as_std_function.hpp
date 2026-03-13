/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include "functions_libs.hpp"
#include "function_traits.hpp"

namespace mstd {
	template<class F, class = void>
	struct as_std_function;

	template<class F>
	struct as_std_function<F, std::void_t<typename function_traits<F>::std_function_type>> {
		using type = typename function_traits<F>::std_function_type;
	};

	template<class F>
	using as_std_function_t = typename as_std_function<F>::type;
}

#endif