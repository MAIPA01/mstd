/*
 * mstd - Maipa's Standard Library
 *
 * BSD 3-Clause License with Attribution Requirement
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgment:
 *    - "This product includes software developed by Patryk Antosik (MAIPA01) as
 *      part of the mstd project."
 * 4. If you modify this software and distribute it, you must include a notice
 *    stating that the original work was created by Patryk Antosik (MAIPA01) as
 *    part of the mstd project.
 * 5. Neither the name of the author nor the name mstd may be used to endorse or
 *    promote products derived from this software without specific prior written
 *    permission.
 * 6. Sale of unmodified code: The unmodified version of this software may not be
 *    sold without the explicit permission of the author.
 * 7. Sale of modified code: Modified versions of the code may be sold, provided that:
 *    - Proper attribution to Patryk Antosik (MAIPA01) as the original author is maintained.
 *    - The modified code is clearly marked as a modified version.
 *    - The modified code must be accompanied by documentation that includes
 *      references to the original version and its author.
 * 8. Exception for large projects: If this code is used as part of a larger project
 *    (e.g., an application, framework, or other libraries), permission is not required
 *    for the sale of the unmodified version, as long as:
 *    - The project is more than just a wrapper for the original code.
 *    - The code is an integral part of the larger project, and the project does not
 *      merely involve selling the original code.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "string_types.hpp"
#include "overflow_operations.hpp"

namespace mstd {
	// 0x0F...
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
	static constexpr bool strxtonum(const std::string& hex_str, _N& num) {
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
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
	static constexpr bool strctonum(const std::string& oct_str, _N& num) {
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
	template<class _N, std::enable_if_t<std::is_integral_v<_N>, bool> = true>
	static constexpr bool strbtonum(const std::string& bin_str, _N& num) {
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
	template<class _SN, std::enable_if_t<std::is_integral_v<_SN>, bool> = true>
	static constexpr bool strtonum(const std::string& str, _SN& num) {
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
			if (str[i] == '-') sign *= -1;
			
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
	template<class _UN, std::enable_if_t<std::is_integral_v<_UN>, bool> = true>
	static constexpr bool strtounum(const std::string& str, _UN& num) {
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
	template<class _FP, std::enable_if_t<std::is_floating_point_v<_FP>, bool> = true>
	static constexpr bool strtofp(const std::string& str, _FP& num) {
		if (str.size() == 0) return false;

		size_t i = 0;

		_FP sign = 1;
		while (str[i] == '-' || str[i] == '+') {
			if (str[i] == '-') sign *= -1;
			
			++i;
			if (i == str.size()) return false;
		}

		num = 0;
		
		while (str[i] >= '0' && str[i] <= '9') {
			num *= 10;
			num += str[i] - '0';

			++i;
			if (i == str.size()) return true;
		}

		if (str[i] == '.') {
			++i;
			if (i == str.size()) return false;

			_FP decimal = 0.1;
			while (str[i] >= '0' && str[i] <= '9') {
				num += decimal * (str[i] - '0');
				decimal *= 0.1;

				++i;
				if (i == str.size()) return true;
			}
		}

		return false;
	}
}