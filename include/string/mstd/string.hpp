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
#include "isstrnum.hpp"
#include "strtonum.hpp"

namespace mstd {	
	static inline std::string trim(const std::string_view& str) {
		const std::string_view::const_iterator & start = 
			std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
				return std::isspace(ch);
			});
		
		const std::string_view::const_iterator& end = 
			std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
				return std::isspace(ch);
			}).base();

		return (start < end) ? std::string(start, end) : std::string();
	}

	template<class... _Strings>
	static inline std::string& concat_to(std::string& out, _Strings&&... strs) {
		out.reserve(out.size() + (string_size(strs) + ...));
		(out += ... += strs);
		return out;
	}

	template<class... _Strings>
	static inline std::string concat(_Strings&&... strs) {
		std::string str;
		return concat_to(str, strs...);
	}
}

#endif