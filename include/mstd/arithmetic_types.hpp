/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "arithmetic_libs.hpp"

namespace mstd {
	template<class... Ns>
	constexpr bool are_unsigned_v = (std::is_unsigned_v<Ns> && ...);

	template<class... Ns>
	constexpr bool are_arithmetic_v = (std::is_arithmetic_v<Ns> && ...);

	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool> = true>
		class vec;

	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	class quat;

	template<size_t C, size_t R, class T,
		std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool> = true>
		class mat;
}