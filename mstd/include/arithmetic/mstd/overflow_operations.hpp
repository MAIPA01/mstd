/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_OVERFLOW_OPERATIONS_HPP_
#define _MSTD_OVERFLOW_OPERATIONS_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/arithmetic_types.hpp>
#include <mstd/types.hpp>

namespace mstd {
#if _MSTD_HAS_CXX20
	template<arithmetic N, arithmetic Na, arithmetic Nb>
#else
	template<class N, class Na, class Nb, std::enable_if_t<are_arithmetic_v<N, Na, Nb>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool add_overflow(const Na& a, const Nb& b, N& out) {
		using limits = std::numeric_limits<N>;

		if _MSTD_CONSTEXPR17 (are_unsigned_v<N, Na, Nb>) {
			if (a > limits::max() - b) {
				return true;
			}
		}
		else {
			if (b >= 0 && a > limits::max() - b) {
				return true;
			}

			if (b <= 0 && a < limits::min() - b) {
				return true;
			}
		}

		out = static_cast<N>(a + b);
		return false;
	}

#if _MSTD_HAS_CXX20
	template<arithmetic N, arithmetic Na, arithmetic Nb>
#else
	template<class N, class Na, class Nb, std::enable_if_t<are_arithmetic_v<N, Na, Nb>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool sub_overflow(const Na& a, const Nb& b, N& out) {
		using limits = std::numeric_limits<N>;

		if _MSTD_CONSTEXPR17(are_unsigned_v<N, Na, Nb>) {
			if (a < limits::min() + b) {
				return true;
			}
		}
		else {
			if (b <= 0 && a > limits::max() + b) {
				return true;
			}

			if (b >= 0 && a < limits::min() + b) {
				return true;
			}
		}

		out = static_cast<N>(a - b);
		return false;
	}

#if _MSTD_HAS_CXX20
	template<arithmetic N, arithmetic Na, arithmetic Nb>
#else
	template<class N, class Na, class Nb, std::enable_if_t<are_arithmetic_v<N, Na, Nb>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool mul_overflow(const Na& a, const Nb& b, N& out) {
		using limits = std::numeric_limits<N>;

		if _MSTD_CONSTEXPR17 (are_unsigned_v<N, Na, Nb>) {
			if (a != 0 && b != 0 && a > limits::max() / b) {
				return true;
			}
		}
		else {
			if (a != 0 && b != 0) {
				if (a == -1 && b == static_cast<Nb>(limits::min())) {
					return true;
				}

				if (a == limits::min() && b == static_cast<Nb>(-1)) {
					return true;
				}

				if (a > static_cast<Na>(limits::max() / b)) {
					return true;
				}

				if (a < static_cast<Na>(limits::min() / b)) {
					return true;
				}
			}
		}

		out = static_cast<N>(a * b);
		return false;
	}

#if _MSTD_HAS_CXX20
	template<arithmetic N, arithmetic Na, arithmetic Nb>
#else
	template<class N, class Na, class Nb, std::enable_if_t<are_arithmetic_v<N, Na, Nb>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool div_overflow(const Na& a, const Nb& b, N& out) {
		using limits = std::numeric_limits<N>;

		if _MSTD_CONSTEXPR17 (are_unsigned_v<N, Na, Nb>) {
			out = static_cast<N>(b == 0 ? limits::max() : a / b);
		}
		else {
			if (a == limits::min() && b == -1) {
				return true;
			}

			N badValue = a < 0 ? limits::min() : limits::max();
			out = static_cast<N>(b == 0 ? badValue : a / b);
		}
		return false;
	}
}
#endif
#endif