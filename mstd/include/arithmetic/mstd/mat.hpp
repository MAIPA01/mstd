/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 * 
 * Tested and fixed by Muppetsg2 (https://github.com/Muppetsg2)
 */

#pragma once
#ifndef _MSTD_MAT_HPP_
#define _MSTD_MAT_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/vec.hpp>

#undef far
#undef near

namespace mstd {
#if _MSTD_HAS_CXX20
	template<size_t C, size_t R, arithmetic T>
	requires (C > 0 && R > 0)
#else
	template<size_t C, size_t R, class T,
		std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool>>
#endif
	class mat {
	public:
		static _MSTD_CONSTEXPR17 const size_t columns = C;
		static _MSTD_CONSTEXPR17 const size_t rows = R;
		static _MSTD_CONSTEXPR17 const size_t size = R * C;
		using column_type = vec<R, T>;
		using row_type = vec<C, T>;
		using value_type = T;

#pragma region COLUMN_CLASS
		class mat_column {
		private:
			using mat_type = mat<C, R, T>;

			mat_type* _parent;
			size_t _column;

		public:
			_MSTD_CONSTEXPR20 mat_column(mat_type* parent, size_t column) : _parent(parent), _column(column) {}
			_MSTD_CONSTEXPR20 mat_column(const mat_column& other) : _parent(other._parent), _column(other._column) {}

			_MSTD_CONSTEXPR20 mat_column& operator=(const mat_column& other) {
				for (size_t y = 0; y != R; ++y) {
					_parent->_values[_column][y] = other[y];
				}
				return *this;
			}
			_MSTD_CONSTEXPR20 mat_column& operator=(const column_type& other) {
				for (size_t y = 0; y != R; ++y) {
					_parent->_values[_column][y] = other[y];
				}
				return *this;
			}

			_MSTD_CONSTEXPR20 bool operator==(const mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			_MSTD_CONSTEXPR20 bool operator!=(const mat_column& other) const {
				return !this->operator==(other);
			}

			_MSTD_CONSTEXPR20 T& operator[](const size_t& idx) {
				return _parent->_values[_column][idx];
			}
			_MSTD_CONSTEXPR20 T operator[](const size_t& idx) const {
				return _parent->_values[_column][idx];
			}
				
			_MSTD_CONSTEXPR20 operator T* () {
				return _parent->_values[_column];
			}
			_MSTD_CONSTEXPR20 operator const T* () const {
				return _parent->_values[_column];
			}
			_MSTD_CONSTEXPR20 operator column_type() const {
				column_type res;
				for (size_t y = 0; y != R; ++y) {
					res[y] = _parent->_values[_column][y];
				}
				return res;
			}
		};

		class const_mat_column {
		private:
			using mat_type = mat<C, R, T>;

			const mat_type* _parent;
			size_t _column;

		public:
			_MSTD_CONSTEXPR20 const_mat_column(const mat_type* parent, size_t column) : _parent(parent), _column(column) {}
			_MSTD_CONSTEXPR20 const_mat_column(const mat_column& other) : _parent(other._parent), _column(other._column) {}
			_MSTD_CONSTEXPR20 const_mat_column(const const_mat_column& other) : _parent(other._parent), _column(other._column) {}

			_MSTD_CONSTEXPR20 bool operator==(const mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			_MSTD_CONSTEXPR20 bool operator!=(const mat_column& other) const {
				return !this->operator==(other);
			}
			_MSTD_CONSTEXPR20 bool operator==(const const_mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			_MSTD_CONSTEXPR20 bool operator!=(const const_mat_column& other) const {
				return !this->operator==(other);
			}

			_MSTD_CONSTEXPR20 T operator[](const size_t& idx) const {
				return _parent->_values[_column][idx];
			}

			_MSTD_CONSTEXPR20 operator const T* () const {
				return _parent->_values[_column];
			}
			_MSTD_CONSTEXPR20 operator column_type() const {
				column_type res;
				for (size_t y = 0; y != R; ++y) {
					res[y] = _parent->_values[_column][y];
				}
				return res;
			}
		};
#pragma endregion // COLUMN_CLASS
	private:
		T _values[C][R] = {};

#pragma region PRIVATE_METHODS
#if _MSTD_HAS_CXX20
		template<arithmetic... Ts, size_t... Idxs>
#else
		template<class... Ts, size_t... Idxs, std::enable_if_t<are_arithmetic_v<Ts...>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs / R][Idxs % R] = static_cast<T>(values)), ...);
		}

#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic VT>
#else
		template<size_t VN, class VT, std::enable_if_t<std::is_arithmetic_v<VT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _set_column(const size_t idx, const vec<VN, VT>& column) {
			size_t maxSize = std::min(VN, R);
			if _MSTD_CONSTEXPR17 (std::is_same_v<T, VT>) {
				std::copy_n(static_cast<const T*>(column), maxSize, &_values[idx]);
			}
			else {
				for (size_t y = 0; y != maxSize; ++y) {
					_values[idx][y] = (T)column[y];
				}
			}
			_fill_column_from(maxSize, idx, T(0));
		}

#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic... Ts, size_t... Idxs>
#else
		template<size_t VN, class... Ts, size_t... Idxs, std::enable_if_t<are_arithmetic_v<Ts...>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _set_values(const std::index_sequence<Idxs...>&, const vec<VN, Ts>&... columns) {
			(_set_column(Idxs, columns), ...);
		}

		_MSTD_CONSTEXPR20 void _fill_column(size_t colIdx, const T& value) {
			if (colIdx >= C) return;
			std::fill_n(&_values[colIdx], C, value);
		}

		_MSTD_CONSTEXPR20 void _fill_column_from(size_t firstIdx, size_t colIdx, const T& value) {
			if (colIdx >= C) return;
			if (firstIdx >= R) return;
			std::fill_n(&_values[colIdx][firstIdx], R - firstIdx, value);
		}

		_MSTD_CONSTEXPR20 void _fill_values(const T& value) {
			std::fill_n(&_values[0][0], R * C, value);
		}

		_MSTD_CONSTEXPR20 void _fill_values_from(size_t firstIdx, const T& value) {
			if (firstIdx >= size) return;
			std::fill_n(&_values[0][0] + firstIdx, size - firstIdx, value);
		}

		_MSTD_CONSTEXPR20 void _set_identity_values(const T& value) {
			std::fill_n(&_values[0][0], size, T(0));
			size_t minSize = std::min(C, R);
			for (size_t i = 0; i != minSize; ++i) {
				_values[i][i] = value;
			}
		}

#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT* values, size_t size) {
			if _MSTD_CONSTEXPR17 (std::is_same_v<T, OT>) {
				std::copy_n(values, std::min(mat<C, R, T>::size, size), &_values[0][0]);
			}
			else {
				size_t sizeLeft = size;
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != std::min(sizeLeft, R); ++y) {
						_values[x][y] = static_cast<T>(values[x][y]);
					}
					if (sizeLeft <= R) {
						break;
					}
					sizeLeft -= R;
				}
			}
		}

#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT* values, size_t columns, size_t rows) {
			size_t colSize = std::min(columns, C);
			size_t rowSize = std::min(rows, R);
			if _MSTD_CONSTEXPR17(std::is_same_v<T, OT>) {
				for (size_t x = 0; x != colSize; ++x) {
					std::copy_n(&values[x * rowSize], rowSize, &_values[x]);
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
			else {
				for (size_t x = 0; x != colSize; ++x) {
					for (size_t y = 0; y != rowSize; ++y) {
						_values[x][y] = static_cast<T>(values[x][y]);
					}
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
		}

#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT(&values)[ON]) {
			_copy_values_from(&values, ON);
		}

#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT(&values)[OC][OR]) {
			_copy_values_from(&values, OC, OR);
		}

#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic OT>
#else
		template<size_t VN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const vec<VN, OT>* columns, size_t size) {
			size_t colSize = std::min(C, size);
			size_t rowSize = std::min(VN, R);
			if _MSTD_CONSTEXPR17(std::is_same_v<T, OT>) {
				for (size_t x = 0; x != colSize; ++x) {
					std::copy_n(static_cast<const T*>(columns[x]), rowSize, &_values[x][0]);
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
			else {
				for (size_t x = 0; x != colSize; ++x) {
					for (size_t y = 0; y != rowSize; ++y) {
						_values[x][y] = static_cast<T>(columns[x][y]);
					}
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
		}

#if _MSTD_HAS_CXX20
		template<size_t VN, size_t N, arithmetic OT>
#else
		template<size_t VN, size_t N, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const vec<VN, OT> (&columns)[N]) {
			_copy_values_from(&columns, N);
		}

#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const mat<OC, OR, OT>& other) {
			size_t colSize = std::min(OC, C);
			size_t rowSize = std::min(OR, R);
			if _MSTD_CONSTEXPR17(std::is_same_v<T, OT>) {
				for (size_t x = 0; x != colSize; ++x) {
					std::copy_n(static_cast<const T*>(other[x]), rowSize, &_values[x][0]);
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
			else {
				for (size_t x = 0; x != colSize; ++x) {
					for (size_t y = 0; y != rowSize; ++y) {
						_values[x][y] = static_cast<T>(other[x][y]);
					}
					_fill_column_from(rowSize, x, static_cast<T>(0));
				}
			}
		}
#pragma endregion // PRIVATE_METHOD

	public:
#pragma region CONSTRUCTORS
		_MSTD_CONSTEXPR20 mat() {
			_fill_values(0);
		}

#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const OT* values, size_t size) {
			_copy_values_from(values, size);
			_fill_values_from(size, static_cast<T>(0));
		}

#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const OT* values, size_t columns, size_t rows) {
			_copy_values_from(values, columns, rows);
			_fill_values_from(columns * R, static_cast<T>(0));
		}
#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const OT(&values)[ON]) : mat(&values, ON) {}
#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const OT(&values)[OC][OR]) : mat(&values, OC, OR) {}
#if _MSTD_HAS_CXX20
		template<arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= size)
#else
		template<class... Ts, std::enable_if_t<((sizeof...(Ts) > 0) && (sizeof...(Ts) <= size) &&
			mstd::are_arithmetic_v<Ts...>), bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const Ts&... values) {
			_set_values(std::index_sequence_for<Ts...>(), values...);
			_fill_values_from(sizeof...(Ts), static_cast<T>(0));
		}
#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic OT>
#else
		template<size_t VN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const vec<VN, OT>* columns, size_t size) {
			_copy_values_from(columns, size);
			_fill_values_from(size * R, static_cast<T>(0));
		}
#if _MSTD_HAS_CXX20
		template<size_t N, size_t VN, arithmetic OT>
#else
		template<size_t N, size_t VN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const vec<VN, OT>(&columns)[N]) : mat(&columns, N) {}
#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= C)
#else
		template<size_t VN, class... Ts, std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= C), bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const vec<VN, Ts>&... columns) {
			_set_values(std::index_sequence_for<Ts...>(), columns...);
			_fill_values_from(sizeof...(Ts) * R, static_cast<T>(0));
		}
#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat(const mat<OC, OR, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(OC * R, static_cast<T>(0));
		}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		_MSTD_CONSTEXPR20 ~mat() noexcept = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator=(const OT(&values)[ON]) {
			_copy_values_from(values);
			_fill_values_from(ON, static_cast<T>(0));
			return *this;
		}
#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator=(const OT(&values)[OC][OR]) {
			_copy_values_from(values);
			_fill_values_from(OC * R, static_cast<T>(0));
			return *this;
		}
#if _MSTD_HAS_CXX20
		template<size_t VN, arithmetic OT, size_t N>
#else
		template<size_t VN, class OT, size_t N, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator=(const vec<VN, OT>(&columns)[N]) {
			_copy_values_from(columns);
			_fill_values_from(N * R, static_cast<T>(0));
			return *this;
		}
#if _MSTD_HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator=(const mat<OC, OR, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(OC * R, static_cast<T>(0));
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static _MSTD_CONSTEXPR20 mat<C, R, T> zero() {
			return mat<C, R, T>();
		}
		static _MSTD_CONSTEXPR20 mat<C, R, T> one() {
			return fill(static_cast<T>(1));
		}
		static _MSTD_CONSTEXPR20 mat<C, R, T> fill(const T& value) {
			mat<C, R, T> res;
			res._fill_values(value);
			return res;
		}

#pragma region PREDEFINED_SQUARE_MATRIX
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> identity() _MSTD_REQUIRES(C == R) {
			return fill_identity(static_cast<T>(1));
		}
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> fill_identity(const T& value) _MSTD_REQUIRES(C == R) {
			mat<C, R, T> res;
			res._set_identity_values(value);
			return res;
		}
		
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && R > 1 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> translation(const vec<R - 1, T>& transVec)
			_MSTD_REQUIRES(C == R && R > 1) {
			mat<C, R, T> res = mat<C, R, T>::identity();
			for (size_t y = 0; y != R - 1; ++y) {
				res[C - 1][y] = transVec[y];
			}
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && R > 1 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> scale(const vec<R - 1, T>& scaleVec)
			_MSTD_REQUIRES(C == R && R > 1) {
			mat<C, R, T> res;
			for (size_t i = 0; i != R - 1; ++i) {
				res[i][i] = scaleVec[i];
			}
			res[C - 1][R - 1] = T(1);
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> scale(const T& scaleFactor) _MSTD_REQUIRES(C == R) {
			return mat<C, R, T>::fill_identity(scaleFactor);
		}

#pragma region PREDEFINED_MATRIX_3x3
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 3 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			screen(const T& left, const T& right, const T& bottom, const T& top, 
			const T& width, const T& height) _MSTD_REQUIRES(C == R && C == 3) {
			const T& invBt = 1.0 / (bottom - top);
			const T& invRl = 1.0 / (right - left);

			mat<C, R, T> res = mat<C, R, T>::zero();
			res[0][0] = width * invRl;
			res[2][0] = -width * left * invRl;
			res[1][1] = height * invBt;
			res[2][1] = -height * top * invBt;
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 3 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			symetric_screen(const T& right, const T& top, const T& width, const T& height) 
			_MSTD_REQUIRES(C == R && C == 3) {
			return screen(-right, right, -top, top, width, height);
		}

#pragma endregion // PREDEFINED_MATRIX_3x3

#pragma region PREDEFINED_MATRIX_4x4
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			rot_x(const T& radians) _MSTD_REQUIRES(C == R && C == 4) {
			T cosA = static_cast<T>(std::cos(radians));
			T sinA = static_cast<T>(std::sin(radians));

			// 4x4
			mat<C, R, T> res = mat<C, R, T>::identity();
			res[1][1] = cosA;
			res[2][1] = -sinA;
			res[1][2] = sinA;
			res[2][2] = cosA;
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> rot_y(const T& radians) 
			_MSTD_REQUIRES(C == R && C == 4) {
			T cosA = static_cast<T>(std::cos(radians));
			T sinA = static_cast<T>(std::sin(radians));

			// 4x4
			mat<C, R, T> res = mat<C, R, T>::identity();
			res[0][0] = cosA;
			res[2][0] = sinA;
			res[0][2] = -sinA;
			res[2][2] = cosA;
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> rot_z(const T& radians) 
			_MSTD_REQUIRES(C == R && C == 4) {
			T cosA = static_cast<T>(std::cos(radians));
			T sinA = static_cast<T>(std::sin(radians));

			// 4x4
			mat<C, R, T> res = mat<C, R, T>::identity();
			res[0][0] = cosA;
			res[1][0] = -sinA;
			res[0][1] = sinA;
			res[1][1] = cosA;
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			rot(const vec<R - 1, T>& axis, const T& radians) _MSTD_REQUIRES(C == R && C == 4) {
			const T& sinA = static_cast<T>(std::sin(radians));
			const T& cosA = static_cast<T>(std::cos(radians));
			const T& oneMinCosA = static_cast<T>(1) - cosA;

			vec<R - 1, T> normAxis = axis;
			if (!normAxis.is_zero()) normAxis.normalize();

			mat<C, R, T> res = mat<C, R, T>::identity();
			res[0][0] = (normAxis[0] * normAxis[0]) + (cosA * (1 - (normAxis[0] * normAxis[0])));
			res[0][1] = ((normAxis[0] * normAxis[1]) * oneMinCosA) - (sinA * normAxis[2]);
			res[0][2] = ((normAxis[0] * normAxis[2]) * oneMinCosA) - (sinA * normAxis[1]);

			res[1][0] = ((normAxis[0] * normAxis[1]) * oneMinCosA) + (sinA * normAxis[2]);
			res[1][1] = (normAxis[1] * normAxis[1]) + (cosA * (1 - (normAxis[1] * normAxis[1])));
			res[1][2] = ((normAxis[1] * normAxis[2]) * oneMinCosA) - (sinA * normAxis[0]);

			res[2][0] = ((normAxis[0] * normAxis[2]) * oneMinCosA) - (sinA * normAxis[1]);
			res[2][1] = ((normAxis[1] * normAxis[2]) * oneMinCosA) + (sinA * normAxis[0]);
			res[2][2] = (normAxis[2] * normAxis[2]) + (cosA * (1 - (normAxis[2] * normAxis[2])));

			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T> rot(const quat<T>& quaternion) 
			_MSTD_REQUIRES(C == R && C == 4) {
			_MSTD_CONSTEXPR17 const float two = 2.f;

			const T& x2 = quaternion.v[0] * quaternion.v[0];
			const T& y2 = quaternion.v[1] * quaternion.v[1];
			const T& z2 = quaternion.v[2] * quaternion.v[2];

			const T& sx = quaternion.s * quaternion.v[0];
			const T& sy = quaternion.s * quaternion.v[1];
			const T& sz = quaternion.s * quaternion.v[2];
			const T& xy = quaternion.v[0] * quaternion.v[1];
			const T& xz = quaternion.v[0] * quaternion.v[2];
			const T& yz = quaternion.v[1] * quaternion.v[2];

			mat<C, R, T> res = mat<C, R, T>::identity();
			res[0][0] = 1.f - (two * (y2 + z2));
			res[1][0] = two * (xy - sz);
			res[2][0] = two * (xz + sy);

			res[0][1] = two * (xy + sz);
			res[1][1] = 1.f - (two * (x2 + z2));
			res[2][1] = two * (yz - sx);

			res[0][2] = two * (xz - sy);
			res[1][2] = two * (yz + sx);
			res[2][2] = 1.f - (two * (x2 + y2));
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			frustrum(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far,
			const T& resLeft = static_cast<T>(-1), const T& resRight = static_cast<T>(1), 
				const T& resBottom = static_cast<T>(-1), const T& resTop = static_cast<T>(1),
			const T& resNear = static_cast<T>(-1), const T& resFar = static_cast<T>(1)) _MSTD_REQUIRES(C == R && C == 4) {

			const T& absNear = std::abs(near);
			const T& absFar = std::abs(far);

			const T& xDir = right > left ? static_cast<T>(1) : static_cast<T>(-1);
			const T& yDir = top > bottom ? static_cast<T>(1) : static_cast<T>(-1);
			const T& zDir = -(xDir * yDir);

			const T& invRl = right == left ? 0.f : (1.f / (right - left));
			const T& invTb = top == bottom ? 0.f : (1.f / (top - bottom));
			const T& invFn = absFar == absNear ? 0.f : (1.f / (absFar - absNear));

			mat<C, R, T> res;
			res[0][0] = (resRight - resLeft) * absNear * invRl;
			res[2][0] = ((resLeft * right) - (resRight * left)) * zDir * invRl;
			res[1][1] = (resTop - resBottom) * absNear * invTb;
			res[2][1] = ((resBottom * top) - (resTop * bottom)) * zDir * invTb;
			res[2][2] = ((resFar * absFar) - (resNear * absNear)) * zDir * invFn;
			res[3][2] = (resNear - resFar) * absNear * absFar * invFn;
			res[2][3] = zDir;
			return res;
		}

		// left = -right, bottom = -top
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			symetric_frustrum(const T& right, const T& top, const T& near, const T& far, const T& resRight = static_cast<T>(1),
			const T& resTop = static_cast<T>(1), const T& resNear = static_cast<T>(-1), const T& resFar = static_cast<T>(1)) 
			_MSTD_REQUIRES(C == R && C == 4) {
			return frustrum(-right, right, -top, top, near, far, -resRight, resRight, -resTop, resTop, resNear, resFar);
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			perspective(const T& fov, const T& aspect, const T& near, const T& far, bool rightPosX = true,
			bool topPosY = true, bool horizontalFov = true, const T& resRight = static_cast<T>(1), 
				const T& resTop = static_cast<T>(1), const T& resNear = static_cast<T>(-1), 
				const T& resFar = static_cast<T>(1)) _MSTD_REQUIRES(C == R && C == 4) {

			_MSTD_CONSTEXPR17 const double half = 0.5;

			const T& absNear = std::abs(near);
			const T& absFar = std::abs(far);

			T right;
			T top;
			if (horizontalFov) {
				right = static_cast<T>(std::tan(fov * half) * absNear);
				top = aspect == static_cast<T>(0) ? static_cast<T>(0) : (right / aspect);
			}
			else {
				top = static_cast<T>(std::tan(fov * half) * absNear);
				right = top * aspect;
			}

			return symetric_frustrum((rightPosX ? right : -right), (topPosY ? top : -top), absNear, absFar,
				resRight, resTop, resNear, resFar);
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			ortographic(const T& left, const T& right, const T& bottom, const T& top, const T& near,
			const T& far, const T& resLeft = static_cast<T>(-1), const T& resRight = static_cast<T>(1), 
				const T& resBottom = static_cast<T>(-1), const T& resTop = static_cast<T>(1), 
				const T& resNear = static_cast<T>(-1), const T& resFar = static_cast<T>(1)) 
			_MSTD_REQUIRES(C == R && C == 4) {

			const T& absNear = std::abs(near);
			const T& absFar = std::abs(far);

			const T& xDir = right > left ? static_cast<T>(1) : static_cast<T>(-1);
			const T& yDir = top > bottom ? static_cast<T>(1) : static_cast<T>(-1);
			const T& zDir = -(xDir * yDir);

			const T& invRl = right == left ? 0.0 : (1.0 / (right - left));
			const T& invTb = top == bottom ? 0.0 : (1.0 / (top - bottom));
			const T& invFn = absFar == absNear ? 0.0 : (1.0 / (absFar - absNear));

			mat<C, R, T> res;
			res[0][0] = (resRight - resLeft) * invRl;
			res[3][0] = ((resLeft * right) - (resRight * left)) * invRl;
			res[1][1] = (resTop - resBottom) * invTb;
			res[3][1] = ((resBottom * top) - (resTop * bottom)) * invTb;
			res[2][2] = (resFar - resNear) * zDir * invFn;
			res[3][2] = ((resNear * absFar) - (resFar * absNear)) * invFn;
			res[3][3] = 1;
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			symetric_ortographic(const T& right, const T& top, const T& near,
			const T& far, const T& resRight = static_cast<T>(1), const T& resTop = static_cast<T>(1), 
				const T& resNear = static_cast<T>(-1), const T& resFar = static_cast<T>(1)) _MSTD_REQUIRES(C == R && C == 4) {
			return ortographic(-right, right, -top, top, near, far, -resRight, resRight, -resTop, resTop, resNear, resFar);
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			view(const vec<3ull, T>& pos, const vec<3ull, T>& right, const vec<3ull, T>& forward, 
				const vec<3ull, T>& up) _MSTD_REQUIRES(C == R && C == 4) {
			mat<C, R, T> res;
			res[0] = vec4(right[0], up[0], -forward[0], T(0));
			res[1] = vec4(right[1], up[1], -forward[1], T(0));
			res[2] = vec4(right[2], up[2], -forward[2], T(0));
			res[3] = vec4(-pos[0], -pos[1], -pos[2], T(1));
			return res;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && C == 4 && std::is_same_v<Type, value_type>))
		static _MSTD_CONSTEXPR20 mat<C, R, T>
			look_at(const vec<3ull, T>& eyePos, const vec<3ull, T>& lookAtPos, 
				const vec<3ull, T>& worldUp) _MSTD_REQUIRES(C == R && C == 4) {
			using vec3_type = vec<3ull, T>;

			vec3_type forward = (lookAtPos - eyePos).normalize();
			vec3_type normWorldUp = worldUp.normalized();
			vec3_type right = forward.cross(worldUp);
			vec3_type up = right.cross(forward);

			return view(eyePos, right, forward, up);
		}
#pragma endregion // PREDEFINED_MATRIX_4x4
#pragma endregion // PREDEFINED_SQUARE_MATRIX
#pragma endregion // PREDEFINED

#pragma region PREDEFINED_CHECKS
		_MSTD_CONSTEXPR20 bool is_zero() const {
			return is_filled_with(static_cast<T>(0));
		}

		_MSTD_CONSTEXPR20 bool is_one() const {
			return is_filled_with(T(1));
		}

		_MSTD_CONSTEXPR20 bool is_filled_with(const T& value) const {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					if (_values[x][y] != value) {
						return false;
					}
				}
			}
			return true;
		}

#pragma region PREDEFINED_SQUARE_MATRIX_CHECKS
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 bool is_identity() const _MSTD_REQUIRES(C == R) {
			return is_identity_filled_with(1);
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 bool is_identity_filled_with(const T& value) const 
			_MSTD_REQUIRES(C == R) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					if ((x == y && mstd::is_not_equal(_values[x][y], value)) || 
						(x != y && mstd::is_not_equal(_values[x][y], static_cast<T>(0)))) {
						return false;
					}
				}
			}
			return true;
		}

#pragma endregion // PREDEFINED_SQUARE_MATRIX_CHECKS
#pragma endregion // PREDEFINED_CHECKS

#pragma region MATRIX_OPERATIONS
		_MSTD_CONSTEXPR20 mat<R, C, T> transposed() const {
			mat<R, C, T> res;
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					res[y][x] = _values[x][y];
				}
			}
			return res;
		}

		_MSTD_CONSTEXPR20 _MSTD_RETURN_VALUE_IF(C > 1 && R > 1, mat<C - 1, R - 1, T>) 
			get_sub_matrix(size_t rowIdx, size_t colIdx) const _MSTD_REQUIRES(C > 1 && R > 1) {
			mat<C - 1, R - 1, T> res;
			for (size_t x = 0, subX = 0; x != C; ++x) {
				if (x == colIdx) continue;
				size_t subY = 0;

				// kopiuje wartości kolumny od 0 do row_idx - 1
				if (rowIdx != 0) {
					std::copy_n(_values[x], std::min(rowIdx, R - 1), static_cast<T*>(res[subX]));
					subY += rowIdx;
				}
				// kopiuje wartości kolumny od row_idx + 1 do R - 1
				if (rowIdx != R - 1) {
					std::copy_n(_values[x] + rowIdx + 1, R - rowIdx - 1, static_cast<T*>(res[subX]) + subY);
				}
				++subX;
			}
			return res;
		}

		_MSTD_CONSTEXPR20 _MSTD_RETURN_VALUE_IF(R > 1, mat<C, R - 1, T>) get_sub_row_matrix(size_t rowIdx) const
		_MSTD_REQUIRES(R > 1) {
			mat<C, R - 1, T> res;
			for (size_t x = 0; x != C; ++x) {
				size_t subY = 0;

				// kopiuje wartości kolumny od 0 do row_idx - 1
				if (rowIdx != 0) {
					std::copy_n(_values[x], std::min(rowIdx, R - 1), static_cast<T*>(res[x]));
					subY += rowIdx;
				}
				// kopiuje wartości kolumny od row_idx + 1 do R - 1
				if (rowIdx != R - 1) {
					std::copy_n(_values[x] + rowIdx + 1, R - 1 - rowIdx, static_cast<T*>(res[x]) + subY);
				}
			}
			return res;
		}

		_MSTD_CONSTEXPR20 _MSTD_RETURN_VALUE_IF(C > 1, mat<C - 1, R, T>) get_sub_col_matrix(size_t colIdx) const
		_MSTD_REQUIRES(C > 1) {
			mat<C - 1, R, T> res;
			for (size_t x = 0, subX = 0; x != C; ++x) {
				if (x == colIdx) continue;

				// kopiuje wartości kolumny
				std::copy_n(_values[x], R, static_cast<T*>(res[subX]));
				++subX;
			}
			return res;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T>& clamp(const T& minVal, const T& maxVal) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] = std::clamp(_values[x][y], minVal, maxVal);
				}
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T> clampped(const T& minVal, const T& maxVal) const {
			mat<C, R, T> res = *this;
			return res.clamp(minVal, maxVal);
		}

		_MSTD_CONSTEXPR20 mat<C, R, T>& clamp(const mat<C, R, T>& minVal, const mat<C, R, T>& maxVal) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] = std::clamp(_values[x][y], minVal[x][y], maxVal[x][y]);
				}
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T> clampped(const mat<C, R, T>& minVal, const mat<C, R, T>& maxVal) const {
			mat<C, R, T> res = *this;
			return res.clamp(minVal, maxVal);
		}

#pragma region SQUARE_MATRIX_OPERATIONS
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<R, C, T>& transpose() _MSTD_REQUIRES(R == C) {
			for (size_t y = 0; y != R; ++y) {
				for (size_t x = 0; x != C; ++x) {
					if (x == y) break;

					T temp = _values[x][y];
					_values[x][y] = _values[y][x];
					_values[y][x] = temp;
				}
			}
			return *this;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 T determinant() const _MSTD_REQUIRES(R == C) {
			if _MSTD_CONSTEXPR17 (R == 1) { 
				return _values[0][0];
			}
			else if _MSTD_CONSTEXPR17 (R == 2) {
				return (_values[0][0] * _values[1][1]) - (_values[0][1] * _values[1][0]);
			}
			else if _MSTD_CONSTEXPR17 (R == 3) {
				T det = 0;
				if (_values[0][0] != static_cast<T>(0)) {
					det += _values[0][0] * ((_values[1][1] * _values[2][2]) - (_values[2][1] * _values[1][2]));
				}
				if (_values[1][0] != static_cast<T>(0)) {
					det += _values[1][0] * ((_values[2][1] * _values[0][2]) - (_values[0][1] * _values[2][2]));
				}
				if (_values[2][0] != static_cast<T>(0)) {
					det += _values[2][0] * ((_values[0][1] * _values[1][2]) - (_values[1][1] * _values[0][2]));
				}
				return det;
			}
			else if _MSTD_CONSTEXPR17 (R == 4) {
				T det = 0;
				if (_values[0][0] != static_cast<T>(0)) {
					det += _values[0][0] *
						((_values[1][1] * ((_values[2][2] * _values[3][3]) - (_values[3][2] * _values[2][3]))) +
							(_values[2][1] * ((_values[3][2] * _values[1][3]) - (_values[1][2] * _values[3][3]))) +
							(_values[3][1] * ((_values[1][2] * _values[2][3]) - (_values[2][2] * _values[1][3]))));
				}
				if (_values[0][1] != static_cast<T>(0)) {
					det -= _values[1][0] *
						((_values[0][1] * ((_values[2][2] * _values[3][3]) - (_values[3][2] * _values[2][3]))) +
							(_values[2][1] * ((_values[3][2] * _values[0][3]) - (_values[0][2] * _values[3][3]))) +
							(_values[3][1] * ((_values[0][2] * _values[2][3]) - (_values[2][2] * _values[0][3]))));
				}
				if (_values[2][0] != static_cast<T>(0)) {
					det += _values[2][0] *
						((_values[0][1] * ((_values[1][2] * _values[3][3]) - (_values[3][2] * _values[1][3]))) +
							(_values[1][1] * ((_values[3][2] * _values[0][3]) - (_values[0][2] * _values[3][3]))) +
							(_values[3][1] * ((_values[0][2] * _values[1][3]) - (_values[1][2] * _values[0][3]))));
				}
				if (_values[3][0] != static_cast<T>(0)) {
					det -= _values[3][0] *
						((_values[0][1] * ((_values[1][2] * _values[2][3]) - (_values[2][2] * _values[1][3]))) +
							(_values[1][1] * ((_values[2][2] * _values[0][3]) - (_values[0][2] * _values[2][3]))) +
							(_values[2][1] * ((_values[0][2] * _values[1][3]) - (_values[1][2] * _values[0][3]))));
				}
				return det;
			}
			else {
				T det = 0;
				int sign = 1;
				for (size_t x = 0; x != C; ++x) {
					if (_values[x][0] != static_cast<T>(0)) {
						// get sub matrix
						mat<C - 1, R - 1, T> subMat = get_sub_matrix(0, x);

						// get sub matrixes det
						T subDet = subMat.determinant();

						// add sub det
						det += sign * _values[x][0] * subDet;
					}

					// change sign
					sign *= -1;
				}
				return det;
			}
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<C, R, T>& invert() _MSTD_REQUIRES(R == C) {
			*this = inverted();
			return *this;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<C, R, T> inverted() const _MSTD_REQUIRES(R == C) {
			// calculate det
			T det = determinant();

			T invD = static_cast<T>(det == static_cast<T>(0) ? 0.0 : (1.0 / det));

			if _MSTD_CONSTEXPR17 (R == 1) {
				return mat<C, R, T>(invD);
			}
			else {
				mat<C, R, T> res;
				if _MSTD_CONSTEXPR17 (R == 2) {
					res[0][0] = _values[1][1] * invD;
					res[1][0] = -_values[1][0] * invD;
					res[0][1] = -_values[0][1] * invD;
					res[1][1] = _values[0][0] * invD;
				}
				else if _MSTD_CONSTEXPR17 (R == 3) {
					res[0][0] = ((_values[1][1] * _values[2][2]) - (_values[2][1] * _values[1][2])) * invD;
					res[1][0] = ((_values[2][0] * _values[1][2]) - (_values[1][0] * _values[2][2])) * invD;
					res[2][0] = ((_values[1][0] * _values[2][1]) - (_values[2][0] * _values[1][1])) * invD;
					res[0][1] = ((_values[0][2] * _values[2][1]) - (_values[0][1] * _values[2][2])) * invD;
					res[1][1] = ((_values[0][0] * _values[2][2]) - (_values[2][0] * _values[0][2])) * invD;
					res[2][1] = ((_values[2][0] * _values[0][1]) - (_values[0][0] * _values[2][1])) * invD;
					res[0][2] = ((_values[0][1] * _values[1][2]) - (_values[1][1] * _values[0][2])) * invD;
					res[1][2] = ((_values[1][0] * _values[0][2]) - (_values[0][0] * _values[1][2])) * invD;
					res[2][2] = ((_values[0][0] * _values[1][1]) - (_values[0][1] * _values[1][0])) * invD;
				}
				else {
					// transponowana (z niej tworzymy mniejsze macierze, usuwając kolumne (x) i wiersz (y), których obliczamy det
					// det staje się wartością elementu na pozycji (x, y) ze znakiem w zależności ((x + y) % 2 == 0) -> 1 else -1 
					// na koniec mnożymy wartość elementu na pozycji (x, y) razy invD
					const mat<R, C, T>& trans = transposed();
					for (size_t x = 0; x != C; ++x) {
						for (size_t y = 0; y != R; ++y) {
							// utworzyć mniejszą macierz
							mat<R - 1, C - 1, T> subMat = get_sub_matrix(y, x);

							// transponujemy sub_mat
							subMat.transpose();

							// obliczyć det mniejszej macierzy
							T subDet = subMat.determinant();

							// jeśli sub_det != 0
							if (subDet != static_cast<T>(0)) {
								// ustawiamy wartość elementu x, y
								res[x][y] = ((x + y) % 2 == 0 ? 1 : -1) * subDet * invD;
							}
						}
					}
					res.transpose();
				}
				return res;
			}
		}
#pragma endregion // SQUARE_MATRIX_OPERATIONS
#pragma endregion // MATRIX_OPERATIONS

#pragma region OPERATORS
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator+=(const mat<C, R, T>& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] += other[x][y];
				}
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator-=(const mat<C, R, T>& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] -= other[x][y];
				}
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T>& operator+=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] += other;
				}
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator-=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] -= other;
				}
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator*=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] *= other;
				}
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator/=(const T& other) {
			if (other == static_cast<T>(0)) {
				return *this;
			}
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] /= other;
				}
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T> operator+(const mat<C, R, T>& other) const {
			mat<C, R, T> res = *this;
			res += other;
			return res;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T> operator-(const mat<C, R, T>& other) const {
			mat<C, R, T> res = *this;
			res -= other;
			return res;
		}
		template<size_t OC>
		_MSTD_CONSTEXPR20 mat<OC, R, T> operator*(const mat<OC, C, T>& other) const {
			mat<OC, R, T> res;
			for (size_t x = 0; x != OC; ++x) {
				for (size_t y = 0; y != R; ++y) {
					for (size_t i = 0; i != C; ++i) {
						res[x][y] += _values[i][y] * other[x][i];
					}
				}
			}
			return res;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T> operator+(const T& other) const {
			mat<C, R, T> res = *this;
			res += other;
			return res;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T> operator-(const T& other) const {
			mat<C, R, T> res = *this;
			res -= other;
			return res;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T> operator*(const T& other) const {
			mat<C, R, T> res = *this;
			res *= other;
			return res;
		}
		friend _MSTD_CONSTEXPR20 mat<C, R, T> operator*(const T& other, const mat<C, R, T>& matrix) {
			return matrix * other;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T> operator/(const T& other) const {
			mat<C, R, T> res = *this;
			res /= other;
			return res;
		}

		_MSTD_CONSTEXPR20 vec<R, T> operator*(const vec<C, T>& other) const {
			vec<R, T> res;
			for (size_t y = 0; y != R; ++y) {
				for (size_t x = 0; x != C; ++x) {
					res[y] += _values[x][y] * other[x];
				}
			}
			return res;
		}

		_MSTD_CONSTEXPR20 mat<C, R, T> operator+() const {
			return mat<C, R, T>(*this);
		}
		_MSTD_CONSTEXPR20 mat<C, R, T> operator-() const {
			return *this * -1;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator++() {
			return *this += 1;
		}
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator--() {
			return *this -= 1;
		}

		template<size_t OC, size_t OR>
		_MSTD_CONSTEXPR20 bool operator==(const mat<OC, OR, T>& other) const {
			if _MSTD_CONSTEXPR17 (OC != C || OR != R) {
				return false;
			}
			else {
				for (size_t x = 0; x != C; ++x) {
					if (std::memcmp(_values[x], other[x], R * sizeof(T)) != 0) {
						return false;
					}
				}
				return true;
			}
		}
		template<size_t OC, size_t OR>
		_MSTD_CONSTEXPR20 bool operator!=(const mat<OC, OR, T>& other) const {
			return !(*this == other);
		}

		_MSTD_CONSTEXPR20 operator const T* () const {
			return _values;
		}

		_MSTD_CONSTEXPR20 mat_column operator[](size_t idx) {
			return mat_column(this, idx);
		}
		_MSTD_CONSTEXPR20 const_mat_column operator[](size_t idx) const {
			return const_mat_column(this, idx);
		}

		friend std::ostream& operator<<(std::ostream& str, const mat<C, R, T>& matrix) {
			static _MSTD_CONSTEXPR17 const char left_top_corner = static_cast<char>(0xda);
			static _MSTD_CONSTEXPR17 const char left_bottom_corner = static_cast<char>(0xc0);
			static _MSTD_CONSTEXPR17 const char side = static_cast<char>(0xb3);
			static _MSTD_CONSTEXPR17 const char right_top_corner = static_cast<char>(0xbf);
			static _MSTD_CONSTEXPR17 const char right_bottom_corner = static_cast<char>(0xd9);

			size_t cellWidth = 0;

			for (size_t y = 0; y != R; ++y) {
				for (size_t x = 0; x != C; ++x) {
					std::ostringstream oss;
					oss << matrix[y][x];
					cellWidth = std::max(cellWidth, oss.str().size());
				}
			}

			for (size_t y = 0; y != R; ++y) {
				if _MSTD_CONSTEXPR17 (R > 1) {
					if (y == 0) str << left_top_corner;
					else if (y == R - 1) str << left_bottom_corner;
					else str << side;
				}
				else {
					str << side;
				}
				str << ' ';

				for (size_t x = 0; x != C; ++x) {
					str << std::setw(cellWidth) << matrix[x][y];
					str << ' ';
				}

				if _MSTD_CONSTEXPR17 (R > 1) {
					if (y == 0) str << right_top_corner;
					else if (y == R - 1) str << right_bottom_corner;
					else str << side;
				}
				else {
					str << side;
				}

				if (y != R - 1) {
					str << std::endl;
				}
			}
			return str;
		}

#pragma region SQUARE_MATRIX_OPERATORS
		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator*=(const mat<C, R, T>& other) _MSTD_REQUIRES(R == C) {
			*this = *this * other;
			return *this;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<C, R, T>& operator/=(const mat<C, R, T>& other) _MSTD_REQUIRES(R == C) {
			*this *= other.inverted();
			return *this;
		}

		_MSTD_ENABLE_IF_TEMPLATE(Type = value_type, (C == R && std::is_same_v<Type, value_type>))
		_MSTD_CONSTEXPR20 mat<C, R, T> operator/(const mat<C, R, T>& other) const _MSTD_REQUIRES(R == C) {
			mat<C, R, T> res = *this;
			res /= other;
			return res;
		}
#pragma endregion // SQUARE_MATRIX_OPERATORS
#pragma endregion // OPERATORS
	};

#pragma region EXTRA_OPERATIONS
#if _MSTD_HAS_CXX20
	template<size_t C, size_t R, arithmetic T>
#else
	template<size_t C, size_t R, class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 mat<C, R, T> clamp(const mat<C, R, T>& a, const T& minVal, const T& maxVal) {
		return a.clampped(minVal, maxVal);
	}

#if _MSTD_HAS_CXX20
	template<size_t C, size_t R, arithmetic T>
#else
	template<size_t C, size_t R, class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 mat<C, R, T> clamp(const mat<C, R, T>& a, const mat<C, R, T>& minVal, const mat<C, R, T>& maxVal) {
		return a.clampped(minVal, maxVal);
	}
#pragma endregion // EXTRA_OPERATIONS
}
#endif
#endif