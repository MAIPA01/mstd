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

		#include <mstd/management_utils.hpp>

namespace mstd {
		#pragma region IS_ITERATOR

	template<class T, class = void>
	struct is_iterator : std::false_type {};

	template<class T>
	struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category> > : std::true_type {};

	template<class T>
	inline _MSTD_CONSTEXPR17 bool is_iterator_v = is_iterator<T>::value;

		#if _MSTD_HAS_CXX20
	template<class T> concept iterator = is_iterator_v<T>;
		#endif
		#pragma endregion

		#pragma region IS_ITERATOR_OF

	template<class Iter, class T, class = void>
	struct is_iterator_of : std::false_type {};

	template<class Iter, class T>
	struct is_iterator_of<Iter, T,
	  std::void_t<std::enable_if_t<is_iterator_v<Iter> &&
								   (std::is_convertible_v<typename std::iterator_traits<Iter>::value_type, remove_cvref_t<T> > ||
									 std::is_same_v<typename std::iterator_traits<Iter>::value_type, remove_cvref_t<T> >)> > >
		: std::true_type {};

	template<class Iter, class T>
	inline _MSTD_CONSTEXPR17 bool is_iterator_of_v = is_iterator_of<Iter, T>::value;

		#if _MSTD_HAS_CXX20
	template<class Iter, class T> concept iterator_of = is_iterator_of_v<Iter, T>;
		#endif
		#pragma endregion

		#pragma region IS_SAME_WITHOUT_CVREF

	template<class T, class U>
	struct is_same_without_cvref : std::is_same<remove_cvref_t<T>, remove_cvref_t<U> > {};

	template<class T>
	struct is_same_without_cvref<T, T> : std::true_type {};

	template<class T, class U>
	inline _MSTD_CONSTEXPR17 bool is_same_without_cvref_v = is_same_without_cvref<T, U>::value;

		#if _MSTD_HAS_CXX20
	template<class T, class U> concept same_without_cvref_as = mstd::is_same_without_cvref_v<T, U>;
		#endif
		#pragma endregion

} // namespace mstd

	#endif
#endif