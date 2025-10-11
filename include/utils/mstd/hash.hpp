/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "utils_libs.hpp"

namespace mstd {
	template<class T>
	void hash_combine(size_t& seed, const T& value) {
		seed ^= std::hash<T>()(value) + 0x9e3779b9
			+ (seed << 6) + (seed >> 2);
	}

	template<class... Ts>
	void hash_combine(size_t& seed, const Ts&... values) {
		(hash_combine(seed, values), ...);
	}

	template<class Iter>
	void hash_combine(size_t& seed, const Iter& begin, const Iter& end) {
		for (const Iter& i = begin; i != end; ++i) {
			hash_combine(seed, *i);
		}
	}
}