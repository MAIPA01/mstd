/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_STRTONUM_HPP
	#define _MSTD_STRTONUM_HPP

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/overflow_operations.hpp>
		#include <mstd/string_types.hpp>

namespace mstd {
		// 0x0F...
		#if _MSTD_HAS_CXX20
	template<mstd::integral N>
		#else
	template<class N, std::enable_if_t<std::is_integral_v<N>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strxtonum(const std::string_view hexStr, N& num) {
		_MSTD_CONSTEXPR17 const size_t hex_divider	 = 16u;
		_MSTD_CONSTEXPR17 const size_t bits_in_hex	 = 8u;
		_MSTD_CONSTEXPR17 const size_t a_decimal_rep = 10;

			if (hexStr.size() <= 2) { return false; }

			if (hexStr[0] != '0' || hexStr[1] != 'x') { return false; }
		size_t i				  = 2;

		using limits			  = std::numeric_limits<N>;
		const uint32_t maxBitsNum = std::max<uint32_t>((limits::digits + limits::is_signed) / bits_in_hex, 1);

		using UN				  = std::make_unsigned_t<N>;
		UN unum					  = 0;
			while (((hexStr[i] >= '0' && hexStr[i] <= '9') || (hexStr[i] >= 'A' && hexStr[i] <= 'F') ||
					 (hexStr[i] >= 'a' && hexStr[i] <= 'f')) &&
				   i - 2 < maxBitsNum) {
					if (mul_overflow(unum, hex_divider, unum)) { return false; }

					if (hexStr[i] >= '0' && hexStr[i] <= '9' && add_overflow(unum, hexStr[i] - '0', unum)) { return false; }

					if (hexStr[i] >= 'A' && hexStr[i] <= 'F' && add_overflow(unum, (hexStr[i] - 'A') + a_decimal_rep, unum)) {
						return false;
					}

					if (hexStr[i] >= 'a' && hexStr[i] <= 'f' && add_overflow(unum, (hexStr[i] - 'a') + a_decimal_rep, unum)) {
						return false;
					}

				++i;
					if (i == hexStr.size()) {
						num = static_cast<N>(unum);
						return true;
					}
			}

		return false;
	}

		// 0c07...
		#if _MSTD_HAS_CXX20
	template<mstd::integral N>
		#else
	template<class N, std::enable_if_t<std::is_integral_v<N>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strctonum(const std::string_view octStr, N& num) {
		_MSTD_CONSTEXPR17 const size_t oct_divider = 8u;

			if (octStr.size() <= 2) { return false; }

			if (octStr[0] != '0' || octStr[1] != 'c') { return false; }
		size_t i					 = 2;

		using limits				 = std::numeric_limits<N>;
		const uint32_t& maxOctalsNum = std::max((limits::digits + limits::is_signed) / 4, 1);

		using UN					 = std::make_unsigned_t<N>;
		UN unum						 = 0;
			while (octStr[i] >= '0' && octStr[i] <= '7' && i - 2 < maxOctalsNum) {
					if (mul_overflow(unum, oct_divider, unum)) { return false; }

					if (add_overflow(unum, octStr[i] - '0', unum)) { return false; }

				++i;
					if (i == octStr.size()) {
						num = static_cast<N>(unum);
						return true;
					}
			}

		return false;
	}

		// 0b01...
		#if _MSTD_HAS_CXX20
	template<mstd::integral N>
		#else
	template<class N, std::enable_if_t<std::is_integral_v<N>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strbtonum(const std::string_view binStr, N& num) {
			if (binStr.size() <= 2) { return false; }

			if (binStr[0] != '0' || binStr[1] != 'b') { return false; }
		size_t i					 = 2;

		using limits				 = std::numeric_limits<N>;
		const uint32_t& maxOctalsNum = limits::digits + limits::is_signed;

		using UN					 = std::make_unsigned_t<N>;
		UN unum						 = 0;
			while (binStr[i] >= '0' && binStr[i] <= '1' && i - 2 < maxOctalsNum) {
					if (mul_overflow(unum, 2u, unum)) { return false; }

					if (add_overflow(unum, binStr[i] - '0', unum)) { return false; }

				++i;
					if (i == binStr.size()) {
						num = static_cast<N>(unum);
						return true;
					}
			}

		return false;
	}

		// ((+|-)* 12) | (0b00...) | (0c00...) | (0x00...)
		#if _MSTD_HAS_CXX20
	template<mstd::signed_integral SN>
		#else
	template<class SN, std::enable_if_t<mstd::is_signed_integral_v<SN>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strtonum(const std::string_view str, SN& num) {
		_MSTD_CONSTEXPR17 const size_t decimal_base = 10;

			if (str.empty()) { return false; }

			if (str.size() > 2 && str[0] == '0') {
					if (str[1] == 'b') { return strbtonum(str, num); }

					if (str[1] == 'c') { return strctonum(str, num); }

					if (str[1] == 'x') { return strxtonum(str, num); }
			}

		size_t i = 0;

		SN sign	 = 1;
			while (str[i] == '-' || str[i] == '+') {
					if (str[i] == '-') { sign *= static_cast<SN>(-1); }

				++i;
					if (i == str.size()) { return false; }
			}

		num = 0;
			while (str[i] >= '0' && str[i] <= '9') {
					if (mul_overflow(num, decimal_base, num)) { return false; }
					if (add_overflow(num, sign * (str[i] - '0'), num)) { return false; }

				++i;
					if (i == str.size()) { return true; }
			}

		return false;
	}

		// (+* 12) | (0b00...) | (0c00...) | (0x00...)
		#if _MSTD_HAS_CXX20
	template<mstd::unsigned_integral UN>
		#else
	template<class UN, std::enable_if_t<mstd::is_unsigned_integral_v<UN>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strtounum(const std::string_view str, UN& num) {
		_MSTD_CONSTEXPR17 const size_t decimal_base = 10;

			if (str.empty()) { return false; }

			if (str.size() > 2 && str[0] == '0') {
					if (str[1] == 'b') { return strbtonum(str, num); }

					if (str[1] == 'c') { return strctonum(str, num); }

					if (str[1] == 'x') { return strxtonum(str, num); }
			}

		size_t i = 0;
			while (str[i] == '+') {
				++i;
					if (i == str.size()) { return false; }
			}

		num = 0;
			while (str[i] >= '0' && str[i] <= '9') {
					if (mul_overflow(num, decimal_base, num)) { return false; }
					if (add_overflow(num, str[i] - '0', num)) { return false; }

				++i;
					if (i == str.size()) { return true; }
			}

		return false;
	}

		// ((+|-)* 12.22)
		#if _MSTD_HAS_CXX20
	template<mstd::floating_point FP>
		#else
	template<class FP, std::enable_if_t<std::is_floating_point_v<FP>, bool> = true>
		#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 bool strtofp(const std::string_view str, FP& num) {
		_MSTD_CONSTEXPR17 const double decimal_mul	= 0.1;
		_MSTD_CONSTEXPR17 const size_t decimal_base = 10;

			if (str.empty()) { return false; }

		size_t i = 0;

		FP sign	 = 1;
			while (str[i] == '-' || str[i] == '+') {
					if (str[i] == '-') { sign *= static_cast<FP>(-1); }

				++i;
					if (i == str.size()) { return false; }
			}

		num = 0;

			while (str[i] >= '0' && str[i] <= '9') {
				num *= decimal_base;
				num += str[i] - '0';

				++i;
					if (i == str.size()) {
						num *= sign;
						return true;
					}
			}

			if (str[i] == '.') {
				++i;
					if (i == str.size()) { return false; }

				FP decimal = static_cast<FP>(decimal_mul);
					while (str[i] >= '0' && str[i] <= '9') {
						num += decimal * (str[i] - '0');
						decimal *= static_cast<FP>(decimal_mul);

						++i;
							if (i == str.size()) {
								num *= sign;
								return true;
							}
					}
			}

		return false;
	}
} // namespace mstd

	#endif
#endif