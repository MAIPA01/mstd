/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_ARITHMETIC_TYPES_HPP_
	#define _MSTD_ARITHMETIC_TYPES_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/arithmetic_utils.hpp>
		#include <mstd/management_types.hpp>

namespace mstd {
		#if _MSTD_HAS_CXX20
	template<size_t N, arithmetic T>
	requires (N > 0)
	class vec;

	template<arithmetic T = float>
	class quat;

	template<size_t C, size_t R, arithmetic T>
	requires (C > 0 && R > 0)
	class mat;
		#else
	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool> = true>
	class vec;

	template<class T = float, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	class quat;

	template<size_t C, size_t R, class T,
	  std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool> = true>
	class mat;
		#endif

		#pragma region PREDEFINED_QUATS
	using fquat	  = quat<float>;
	using dquat	  = quat<double>;
	using ldquat  = quat<long double>;
	using iquat	  = quat<int>;
	using uquat	  = quat<unsigned int>;
	using bquat	  = quat<bool>;
	using cquat	  = quat<char>;
	using ucquat  = quat<unsigned char>;
	using scquat  = quat<signed char>;
	using lquat	  = quat<long>;
	using ulquat  = quat<unsigned long>;
	using llquat  = quat<long long>;
	using ullquat = quat<unsigned long long>;
		#pragma endregion

		#pragma region PREDEFINED_VECS
		#define _MSTD_MAKE_VECS_TYPES(len)                   \
			using vec##len	  = vec<len, float>;             \
			using fvec##len	  = vec<len, float>;             \
			using dvec##len	  = vec<len, double>;            \
			using ldvec##len  = vec<len, long double>;       \
			using ivec##len	  = vec<len, int>;               \
			using uvec##len	  = vec<len, unsigned int>;      \
			using bvec##len	  = vec<len, bool>;              \
			using cvec##len	  = vec<len, char>;              \
			using ucvec##len  = vec<len, unsigned char>;     \
			using scvec##len  = vec<len, signed char>;       \
			using lvec##len	  = vec<len, long>;              \
			using ulvec##len  = vec<len, unsigned long>;     \
			using llvec##len  = vec<len, long long>;         \
			using ullvec##len = vec<len, unsigned long long>

	_MSTD_MAKE_VECS_TYPES(2);
	_MSTD_MAKE_VECS_TYPES(3);
	_MSTD_MAKE_VECS_TYPES(4);
		#pragma endregion

		#pragma region PREDEFINED_MATS
		#define _MSTD_MAKE_MAT_NOT_SQR_TYPES(col, row)                    \
			using mat##col##x##row	  = mat<col, row, float>;             \
			using fmat##col##x##row	  = mat<col, row, float>;             \
			using dmat##col##x##row	  = mat<col, row, double>;            \
			using ldmat##col##x##row  = mat<col, row, long double>;       \
			using imat##col##x##row	  = mat<col, row, int>;               \
			using umat##col##x##row	  = mat<col, row, unsigned int>;      \
			using bmat##col##x##row	  = mat<col, row, bool>;              \
			using cmat##col##x##row	  = mat<col, row, char>;              \
			using ucmat##col##x##row  = mat<col, row, unsigned char>;     \
			using scmat##col##x##row  = mat<col, row, signed char>;       \
			using lmat##col##x##row	  = mat<col, row, long>;              \
			using ulmat##col##x##row  = mat<col, row, unsigned long>;     \
			using llmat##col##x##row  = mat<col, row, long long>;         \
			using ullmat##col##x##row = mat<col, row, unsigned long long>

		#define _MSTD_MAKE_TWO_MAT_NOT_SQR_TYPES(col, row) \
			_MSTD_MAKE_MAT_NOT_SQR_TYPES(col, row);        \
			_MSTD_MAKE_MAT_NOT_SQR_TYPES(row, col)

	_MSTD_MAKE_TWO_MAT_NOT_SQR_TYPES(2, 3);
	_MSTD_MAKE_TWO_MAT_NOT_SQR_TYPES(2, 4);
	_MSTD_MAKE_TWO_MAT_NOT_SQR_TYPES(3, 4);

	template<size_t N, class T>
	using mat_sqr = mat<N, N, T>;

		#define _MSTD_MAKE_MAT_SQR_TYPES(len)                    \
			using mat##len	  = mat_sqr<len, float>;             \
			using fmat##len	  = mat_sqr<len, float>;             \
			using dmat##len	  = mat_sqr<len, double>;            \
			using ldmat##len  = mat_sqr<len, long double>;       \
			using imat##len	  = mat_sqr<len, int>;               \
			using umat##len	  = mat_sqr<len, unsigned int>;      \
			using bmat##len	  = mat_sqr<len, bool>;              \
			using cmat##len	  = mat_sqr<len, char>;              \
			using ucmat##len  = mat_sqr<len, unsigned char>;     \
			using scmat##len  = mat_sqr<len, signed char>;       \
			using lmat##len	  = mat_sqr<len, long>;              \
			using ulmat##len  = mat_sqr<len, unsigned long>;     \
			using llmat##len  = mat_sqr<len, long long>;         \
			using ullmat##len = mat_sqr<len, unsigned long long>

	_MSTD_MAKE_MAT_SQR_TYPES(2);
	_MSTD_MAKE_MAT_SQR_TYPES(3);
	_MSTD_MAKE_MAT_SQR_TYPES(4);
		#pragma endregion
} // namespace mstd
	#endif
#endif