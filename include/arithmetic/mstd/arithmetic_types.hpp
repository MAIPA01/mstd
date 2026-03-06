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

#include "arithmetic_libs.hpp"

namespace mstd {
#if _MSTD_HAS_CXX20
	template<size_t N, arithmetic T>
	requires (N > 0)
	class vec;

	template<arithmetic T = float>
	class quat;

	template<size_t C, size_t R, arithmetic T>
	requires (C > 0 && R > 0)
	class mat;
#else
	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool> = true>
	class vec;

	template<class T = float, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	class quat;

	template<size_t C, size_t R, class T,
		std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool> = true>
	class mat;
#endif
}
#endif