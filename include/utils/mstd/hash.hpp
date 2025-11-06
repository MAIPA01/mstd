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
	template<class T, class... Ts>
	void hash_append(size_t& hash_value, const T& value, const Ts&... values) {
		hash_value ^= std::hash<T>()(value) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);

		if constexpr (sizeof...(Ts) != 0) {
			hash_append(hash_value, values...);
		}
	}

	template<class T0, class T1, class... Ts>
	size_t hash_combine(const T0& value0, const T1& value1, const Ts&... values) {
		size_t hash_value = 0;
		hash_append(hash_value, value0, value1, values...);
		return hash_value;
	}

	template<class Iter>
	void hash_range(size_t& seed, const Iter& begin, const Iter& end) {
		for (Iter i = begin; i != end; ++i) {
			hash_append(seed, *i);
		}
	}
}