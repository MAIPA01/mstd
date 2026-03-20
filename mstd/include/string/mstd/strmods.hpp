/*
* mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_STRMODS_HPP_
	#define _MSTD_STRMODS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/string_types.hpp>

namespace mstd {
	inline std::string trim(std::string_view const& str) {
		std::string_view::const_iterator const& start =
		  std::find_if_not(str.begin(), str.end(), [](unsigned char ch) { return std::isspace(ch); });

		std::string_view::const_iterator const& end = std::find_if_not(
		  str.rbegin(), str.rend(), [](unsigned char ch) { return std::isspace(ch); }
		).base();

		return (start < end) ? std::string(start, end) : std::string();
	}
} // namespace mstd

	#endif
#endif