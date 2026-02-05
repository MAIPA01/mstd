/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "string_types.hpp"
#include <mstd/overflow_operations.hpp>

namespace mstd {
	// 0x0F...
#if _MSTD_HAS_CXX20
	template<mstd::integral _N>
#else
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strxtonum(const std::string& hex_str, _N& num) {
		if (hex_str.size() <= 2) return false;

		if (hex_str[0] != '0' || hex_str[1] != 'x') {
			return false;
		}
		size_t i = 2;

		const std::numeric_limits<_N>& limits = std::numeric_limits<_N>();
		const uint32_t& max_bits_num = std::max((limits.digits + limits.is_signed) / 8, 1);

		using _UN = std::make_unsigned_t<_N>;
		_UN unum = 0;
		while (((hex_str[i] >= '0' && hex_str[i] <= '9') ||
			(hex_str[i] >= 'A' && hex_str[i] <= 'F') ||
			(hex_str[i] >= 'a' && hex_str[i] <= 'f')) && i - 2 < max_bits_num) {
			
			if (mul_overflow(unum, 16u, unum)) {
				return false;
			}

			if (hex_str[i] >= '0' && hex_str[i] <= '9') {
				if (add_overflow(unum, hex_str[i] - '0', unum)) {
					return false;
				}
			}
			else if (hex_str[i] >= 'A' && hex_str[i] <= 'F') {
				if (add_overflow(unum, (hex_str[i] - 'A') + 10, unum)) {
					return false;
				}
			}
			else if (hex_str[i] >= 'a' && hex_str[i] <= 'f') {
				if (add_overflow(unum, (hex_str[i] - 'a') + 10, unum)) {
					return false;
				}
			}

			++i;
			if (i == hex_str.size()) {
				num = static_cast<_N>(unum);
				return true;
			}
		}
		
		return false;
	}

	// 0c07...
#if _MSTD_HAS_CXX20
	template<mstd::integral _N>
#else
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strctonum(const std::string& oct_str, _N& num) {
		if (oct_str.size() <= 2) return false;

		if (oct_str[0] != '0' || oct_str[1] != 'c') {
			return false;
		}
		size_t i = 2;

		const std::numeric_limits<_N>& limits = std::numeric_limits<_N>();
		const uint32_t& max_octals_num = std::max((limits.digits + limits.is_signed) / 4, 1);

		using _UN = std::make_unsigned_t<_N>;
		_UN unum = 0;
		while (oct_str[i] >= '0' && oct_str[i] <= '7' && i - 2 < max_octals_num) {
			if (mul_overflow(unum, 8u, unum)) {
				return false;
			}

			if (add_overflow(unum, oct_str[i] - '0', unum)) {
				return false;
			}

			++i;
			if (i == oct_str.size()) {
				num = static_cast<_N>(unum);
				return true;
			}
		}

		return false;
	}

	// 0b01...
#if _MSTD_HAS_CXX20
	template<mstd::integral _N>
#else
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strbtonum(const std::string& bin_str, _N& num) {
		if (bin_str.size() <= 2) return false;

		if (bin_str[0] != '0' || bin_str[1] != 'b') {
			return false;
		}
		size_t i = 2;

		const std::numeric_limits<_N>& limits = std::numeric_limits<_N>();
		const uint32_t& max_bits_num = limits.digits + limits.is_signed;

		using _UN = std::make_unsigned_t<_N>;
		_UN unum = 0;
		while (bin_str[i] >= '0' && bin_str[i] <= '1' && i - 2 < max_bits_num) {
			if (mul_overflow(unum, 2u, unum)) {
				return false;
			}

			if (add_overflow(unum, bin_str[i] - '0', unum)) {
				return false;
			}

			++i;
			if (i == bin_str.size()) {
				num = static_cast<_N>(unum);
				return true;
			}
		}

		return false;
	}

	// ((+|-)* 12) | (0b00...) | (0c00...) | (0x00...)
#if _MSTD_HAS_CXX20
	template<mstd::signed_integral _SN>
#else
	template<class _SN, std::enable_if_t<mstd::is_signed_integral_v<_SN>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strtonum(const std::string& str, _SN& num) {
		if (str.size() == 0) return false;

		if (str.size() > 2) {
			if (str[0] == '0') {
				if (str[1] == 'b') {
					return strbtonum(str, num);
				}
				else if (str[1] == 'c') {
					return strctonum(str, num);
				}
				else if (str[1] == 'x') {
					return strxtonum(str, num);
				}
			}
		}

		size_t i = 0;

		_SN sign = 1;
		while (str[i] == '-' || str[i] == '+') {
			if (str[i] == '-') sign *= static_cast<_SN>(-1);
			
			++i;
			if (i == str.size()) return false;
		}

		num = 0;
		while (str[i] >= '0' && str[i] <= '9') {
			if (mul_overflow(num, 10, num)) {
				return false;
			}
			if (add_overflow(num, sign * (str[i] - '0'), num)) {
				return false;
			}

			++i;
			if (i == str.size()) {
				return true;
			}
		}

		return false;
	}

	// (+* 12) | (0b00...) | (0c00...) | (0x00...)
#if _MSTD_HAS_CXX20
	template<mstd::unsigned_integral _UN>
#else
	template<class _UN, std::enable_if_t<mstd::is_unsigned_integral_v<_UN>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strtounum(const std::string& str, _UN& num) {
		if (str.size() == 0) return false;

		if (str.size() > 2) {
			if (str[0] == '0') {
				if (str[1] == 'b') {
					return strbtonum(str, num);
				}
				else if (str[1] == 'c') {
					return strctonum(str, num);
				}
				else if (str[1] == 'x') {
					return strxtonum(str, num);
				}
			}
		}

		size_t i = 0;
		while (str[i] == '+') {
			++i;
			if (i == str.size()) return false;
		}

		num = 0;
		while (str[i] >= '0' && str[i] <= '9') {
			if (mul_overflow(num, 10u, num)) {
				return false;
			}
			if (add_overflow(num, str[i] - '0', num)) {
				return false;
			}

			++i;
			if (i == str.size()) {
				return true;
			}
		}

		return false;
	}

	// ((+|-)* 12.22)
#if _MSTD_HAS_CXX20
	template<mstd::floating_point _FP>
#else
	template<class _FP, std::enable_if_t<std::is_floating_point_v<_FP>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool strtofp(const std::string& str, _FP& num) {
		if (str.size() == 0) return false;

		size_t i = 0;

		_FP sign = 1;
		while (str[i] == '-' || str[i] == '+') {
			if (str[i] == '-') sign *= static_cast<_FP>(-1);
			
			++i;
			if (i == str.size()) return false;
		}

		num = 0;
		
		while (str[i] >= '0' && str[i] <= '9') {
			num *= 10;
			num += str[i] - '0';

			++i;
			if (i == str.size()) {
				num *= sign;
				return true;
			}
		}

		if (str[i] == '.') {
			++i;
			if (i == str.size()) return false;

			_FP decimal = static_cast<_FP>(0.1);
			while (str[i] >= '0' && str[i] <= '9') {
				num += decimal * (str[i] - '0');
				decimal *= static_cast<_FP>(0.1);

				++i;
				if (i == str.size()) {
					num *= sign;
					return true;
				}
			}
		}

		return false;
	}
}