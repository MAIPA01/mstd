/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_HASH_HPP_
#define _MSTD_HASH_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/types.hpp>

namespace mstd {
	template<class T, class... Ts>
	inline void hash_append(size_t& hash_value, const T& value, const Ts&... values) {
		hash_value ^= std::hash<T>()(value) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);

		if _MSTD_CONSTEXPR17 (sizeof...(Ts) != 0) {
			hash_append(hash_value, values...);
		}
	}

	template<class T0, class T1, class... Ts>
	inline size_t hash_combine(const T0& value0, const T1& value1, const Ts&... values) {
		size_t hash_value = 0;
		hash_append(hash_value, value0, value1, values...);
		return hash_value;
	}

	template<class Iter>
	inline void hash_range(size_t& seed, const Iter& begin, const Iter& end) {
		for (Iter i = begin; i != end; ++i) {
			hash_append(seed, *i);
		}
	}
}

#endif
#endif