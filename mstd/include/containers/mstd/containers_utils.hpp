/*
* mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_CONTAINERS_UTILS_HPP_
	#define _MSTD_CONTAINERS_UTILS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/containers_libs.hpp>

namespace mstd {
	template<class T, class = void>
	struct is_iterator : std::false_type {};

	template<class T>
	struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category> >
		: std::true_type {};

	template<class T>
	inline _MSTD_CONSTEXPR17 bool is_iterator_v = is_iterator<T>::value;

		#if _MSTD_HAS_CXX20
	template<class T> concept iterator = is_iterator_v<T>;
		#endif
} // namespace mstd

	#endif
#endif