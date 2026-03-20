/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_STRING_UTILS_HPP_
	#define _MSTD_STRING_UTILS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/string_libs.hpp>

namespace mstd::utils {
	template<class T, class = void>
	struct string_type_info;

	template<class T>
	struct string_type_info<T,
	  std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, char> || std::is_same_v<std::remove_cv_t<T>, signed char> ||
					   std::is_same_v<std::remove_cv_t<T>, unsigned char> > > {
		static _MSTD_CONSTEXPR17 size_t size(const T&) { return 1; }
	};

	template<class T, size_t N>
	struct string_type_info<T[N],
	  std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, char> || std::is_same_v<std::remove_cv_t<T>, signed char> ||
					   std::is_same_v<std::remove_cv_t<T>, unsigned char> > > {
		static _MSTD_CONSTEXPR17 size_t size(const T (&)[N]) { return N - 1; }
	};

	template<class T>
	struct string_type_info<T,
	  std::enable_if_t<std::is_pointer_v<T> && (std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T> >, char> ||
												 std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T> >, signed char> ||
												 std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T> >, unsigned char>)> > {
		static _MSTD_CONSTEXPR17 size_t size(T s) { return s ? std::strlen(reinterpret_cast<const char*>(s)) : 0; }
	};

	template<class T>
	struct string_type_info<T, std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, std::string> > > {
		static _MSTD_CONSTEXPR17 size_t size(const std::string& s) { return s.size(); }
	};

	template<class T>
	struct string_type_info<T, std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, std::string_view> > > {
		static _MSTD_CONSTEXPR17 size_t size(const std::string_view& s) { return s.size(); }
	};

	template<class String>
	_MSTD_CONSTEXPR17 size_t string_size(String&& s) {
		using string_t = std::remove_reference_t<String>;
		return string_type_info<string_t>::size(s);
	}
} // namespace mstd::utils

	#endif
#endif