/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include "string_types.hpp"

namespace mstd {
	inline bool isstrhex(const std::string_view& str) {
		if (str.size() <= 2) return false;
		if (str[0] != '0' || str[1] != 'x') return false;

		size_t i = 2;
		for (; i != str.size(); ++i) {
			if ((str[i] < '0' || str[i] > '9') &&
				(str[i] < 'a' || str[i] > 'f') &&
				(str[i] < 'A' || str[i] > 'F')) {
				return false;
			}
		}

		return true;
	}

	inline bool isstroct(const std::string_view& str) {
		if (str.size() <= 2) return false;
		if (str[0] != '0' || str[1] != 'c') return false;

		size_t i = 2;
		for (; i != str.size(); ++i) {
			if (str[i] < '0' || str[i] > '7') {
				return false;
			}
		}

		return true;
	}

	inline bool isstrbin(const std::string_view& str) {
		if (str.size() <= 2) return false;
		if (str[0] != '0' || str[1] != 'b') return false;

		size_t i = 2;
		for (; i != str.size(); ++i) {
			if (str[i] < '0' || str[i] > '1') {
				return false;
			}
		}

		return true;
	}

	inline bool isstrnum(const std::string_view& str) {
		if (str.size() == 0) return false;

		if (str.size() > 2) {
			if (str[0] == '0') {
				if (str[1] == 'b') {
					return isstrbin(str);
				}
				else if (str[1] == 'c') {
					return isstroct(str);
				}
				else if (str[1] == 'x') {
					return isstrhex(str);
				}
			}
		}

		size_t i = 0;
		while (str[i] == '+' || str[i] == '-') {
			++i;
			if (i == str.size()) return false;
		}

		for (; i != str.size(); ++i) {
			if (str[i] < '0' || str[i] > '9') return false;
		}

		return true;
	}

	inline bool isstrunum(const std::string_view& str) {
		if (str.size() == 0) return false;

		if (str.size() > 2) {
			if (str[0] == '0') {
				if (str[1] == 'b') {
					return isstrbin(str);
				}
				else if (str[1] == 'c') {
					return isstroct(str);
				}
				else if (str[1] == 'x') {
					return isstrhex(str);
				}
			}
		}

		size_t i = 0;
		while (str[i] == '+') {
			++i;
			if (i == str.size()) return false;
		}

		for (; i != str.size(); ++i) {
			if (str[i] < '0' || str[i] > '9') return false;
		}

		return true;
	}

	inline bool isstrfp(const std::string_view& str) {
		if (str.size() == 0) return false;

		size_t i = 0;
		while (str[i] == '+' || str[i] == '-') {
			++i;
			if (i == str.size()) return false;
		}

		while (str[i] >= '0' && str[i] <= '9') {
			++i;
			if (i == str.size()) return true;
		}

		if (str[i] == '.') {
			++i;
			if (i == str.size()) return false;

			while (str[i] >= '0' && str[i] <= '9') {
				++i;
				if (i == str.size()) return true;
			}
		}

		return false;
	}
}

#endif