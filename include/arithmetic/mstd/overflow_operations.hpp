/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "arithmetic_types.hpp"

namespace mstd {
#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<arithmetic _N, arithmetic _Na, arithmetic _Nb>
#else
	template<class _N, class _Na, class _Nb, std::enable_if_t<are_arithmetic_v<_N, _Na, _Nb>, bool> = true>
#endif
	static constexpr bool add_overflow(const _Na& a, const _Nb& b, _N& out) {
		if constexpr (are_unsigned_v<_N, _Na, _Nb>) {
			if (a > std::numeric_limits<_N>::max() - b) {
				return true;
			}
		}
		else {
			if (b >= 0 && a > std::numeric_limits<_N>::max() - b) {
				return true;
			}

			if (b <= 0 && a < std::numeric_limits<_N>::min() - b) {
				return true;
			}
		}

		out = a + b;
		return false;
	}

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<arithmetic _N, arithmetic _Na, arithmetic _Nb>
#else
	template<class _N, class _Na, class _Nb, std::enable_if_t<are_arithmetic_v<_N, _Na, _Nb>, bool> = true>
#endif
	static constexpr bool sub_overflow(const _Na& a, const _Nb& b, _N& out) {
		if constexpr (are_unsigned_v<_N, _Na, _Nb>) {
			if (a < std::numeric_limits<_N>::min() + b) {
				return true;
			}
		}
		else {
			if (b <= 0 && a > std::numeric_limits<_N>::max() + b) {
				return true;
			}

			if (b >= 0 && a < std::numeric_limits<_N>::min() + b) {
				return true;
			}
		}

		out = a - b;
		return false;
	}

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<arithmetic _N, arithmetic _Na, arithmetic _Nb>
#else
	template<class _N, class _Na, class _Nb, std::enable_if_t<are_arithmetic_v<_N, _Na, _Nb>, bool> = true>
#endif
	static constexpr bool mul_overflow(const _Na& a, const _Nb& b, _N& out) {
		if constexpr (are_unsigned_v<_N, _Na, _Nb>) {
			if (a != 0 && b != 0 && a > std::numeric_limits<_N>::max() / b) {
				return true;
			}
		}
		else {
			if (a != 0 && b != 0) {
				if (a == -1 && b == std::numeric_limits<_N>::min()) {
					return true;
				}

				if (a == std::numeric_limits<_N>::min() && b == -1) {
					return true;
				}

				if (a > std::numeric_limits<_N>::max() / b) {
					return true;
				}

				if (a < std::numeric_limits<_N>::min() / b) {
					return true;
				}
			}
		}

		out = a * b;
		return false;
	}

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<arithmetic _N, arithmetic _Na, arithmetic _Nb>
#else
	template<class _N, class _Na, class _Nb, std::enable_if_t<are_arithmetic_v<_N, _Na, _Nb>, bool> = true>
#endif
	static constexpr bool div_overflow(const _Na& a, const _Nb& b, _N& out) {
		if constexpr (are_unsigned_v<_N, _Na, _Nb>) {
			out = b == 0 ? std::numeric_limits<_N>::max() : a / b;
		}
		else {
			if (a == std::numeric_limits<_N>::min() && b == -1) {
				return true;
			}

			out = b == 0 ? (a < 0 ? std::numeric_limits<_N>::min() : std::numeric_limits<_N>::max()) : a / b;
		}
		return false;
	}
}