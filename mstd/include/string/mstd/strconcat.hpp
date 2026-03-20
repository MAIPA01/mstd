/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_STRCONCAT_HPP_
	#define _MSTD_STRCONCAT_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/string_types.hpp>

namespace mstd {
	template<class... Strings>
	inline std::string& concat_to(std::string& out, Strings&&... strs) {
		out.reserve(out.size() + (utils::string_size(strs) + ...));
		(out += ... += strs);
		return out;
	}

	template<class... Strings>
	inline std::string concat(Strings&&... strs) {
		std::string str;
		return concat_to(str, std::forward<Strings>(strs)...);
	}
} // namespace mstd

	#endif
#endif