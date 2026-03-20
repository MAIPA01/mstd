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
	inline void hash_append(size_t& hashValue, const T& value, const Ts&... values) {
		static _MSTD_CONSTEXPR17 const size_t left_shift   = 6;
		static _MSTD_CONSTEXPR17 const size_t right_shift  = 2;
		static _MSTD_CONSTEXPR17 const size_t magic_number = 0x9E37'79B9;

		hashValue ^= std::hash<T>()(value) + magic_number + (hashValue << left_shift) + (hashValue >> right_shift);

			if _MSTD_CONSTEXPR17 (sizeof...(Ts) != 0) { hash_append(hashValue, values...); }
	}

	template<class T0, class T1, class... Ts>
	inline size_t hash_combine(const T0& value0, const T1& value1, const Ts&... values) {
		size_t hashValue = 0;
		hash_append(hashValue, value0, value1, values...);
		return hashValue;
	}

	template<class Iter>
	inline void hash_range(size_t& seed, const Iter& begin, const Iter& end) {
			for (Iter i = begin; i != end; ++i) { hash_append(seed, *i); }
	}
} // namespace mstd

	#endif
#endif