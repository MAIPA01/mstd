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
#include "vec.hpp"

namespace mstd {
#if _HAS_CXX20
	template<size_t C, size_t R, arithmetic T>
	requires (C > 0 && R > 0)
#else
	template<size_t C, size_t R, class T,
		std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool>>
#endif
	class mat {
	public:
		static constexpr const size_t columns = C;
		static constexpr const size_t rows = R;
		static constexpr const size_t size = R * C;
		using column_type = vec<R, T>;
		using row_type = vec<C, T>;

#pragma region COLUMN_CLASS
		class mat_column {
		private:
			using mat_type = mat<C, R, T>;

			mat_type* _parent;
			size_t _column;

		public:
			mat_column(mat_type* parent, size_t column) : _parent(parent), _column(column) {}
			mat_column(const mat_column& other) : _parent(other._parent), _column(other._column) {}

			mat_column& operator=(const mat_column& other) {
				for (size_t y = 0; y != R; ++y) {
					_parent->_values[_column][y] = other[y];
				}
				return *this;
			}
			mat_column& operator=(const column_type& other) {
				for (size_t y = 0; y != R; ++y) {
					_parent->_values[_column][y] = other[y];
				}
				return *this;
			}

			bool operator==(const mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			bool operator!=(const mat_column& other) const {
				return !this->operator==(other);
			}

			T& operator[](const size_t& idx) {
				return _parent->_values[_column][idx];
			}
			T operator[](const size_t& idx) const {
				return _parent->_values[_column][idx];
			}
				
			operator T* () {
				return _parent->_values[_column];
			}
			operator const T* () const {
				return _parent->_values[_column];
			}
			operator column_type() const {
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
			const_mat_column(const mat_type* parent, size_t column) : _parent(parent), _column(column) {}
			const_mat_column(const mat_column& other) : _parent(other._parent), _column(other._column) {}
			const_mat_column(const const_mat_column& other) : _parent(other._parent), _column(other._column) {}

			bool operator==(const mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			bool operator!=(const mat_column& other) const {
				return !this->operator==(other);
			}
			bool operator==(const const_mat_column& other) const {
				for (size_t y = 0; y != R; ++y) {
					if (_parent->_values[_column][y] != other[y]) {
						return false;
					}
				}
				return true;
			}
			bool operator!=(const const_mat_column& other) const {
				return !this->operator==(other);
			}

			T operator[](const size_t& idx) const {
				return _parent->_values[_column][idx];
			}

			operator const T* () const {
				return _parent->_values[_column];
			}
			operator column_type() const {
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
#if _HAS_CXX20
		template<arithmetic... Ts, size_t... Idxs>
#else
		template<class... Ts, size_t... Idxs>
#endif
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs / R][Idxs % R] = (T)values), ...);
		}

		template<size_t VN, class VT>
		constexpr void _set_column(const size_t idx, const vec<VN, VT>& column) {
			size_t max_size = std::min(VN, R);
			if constexpr (std::is_same_v<T, VT>) {
				std::memcpy(&_values[idx], static_cast<const T*>(column), max_size * sizeof(T));
			}
			else {
				for (size_t y = 0; y != max_size; ++y) {
					_values[idx][y] = (T)column[y];
				}
			}
			_fill_column_from(max_size, idx, T(0));
		}

		template<size_t VN, class... Ts, size_t... Idxs>
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const vec<VN, Ts>&... columns) {
			(_set_column(Idxs, columns), ...);
		}

		constexpr void _fill_column(const size_t& col_idx, const T& value) {
			if (col_idx >= C) return;
			std::fill_n(&_values[col_idx], C, value);
		}

		constexpr void _fill_column_from(const size_t& first_idx, const size_t& col_idx, const T& value) {
			if (col_idx >= C) return;
			if (first_idx >= R) return;
			std::fill_n(&_values[col_idx][first_idx], R - first_idx, value);
		}

		constexpr void _fill_values(const T& value) {
			std::fill_n(&_values[0][0], R * C, value);
		}

		constexpr void _fill_values_from(const size_t& first_idx, const T& value) {
			if (first_idx >= size) return;
			std::fill_n(&_values[0][0] + first_idx, size - first_idx, value);
		}

		constexpr void _set_identity_values(const T& value) {
			std::fill_n(&_values[0][0], size, T(0));
			size_t size = std::min(C, R);
			for (size_t i = 0; i != size; ++i) {
				_values[i][i] = value;
			}
		}

#if _HAS_CXX20
			template<arithmetic OT>
#else
			template<class OT>
#endif
			constexpr void _copy_values_from(const OT* values, const size_t& size) {
				if constexpr (std::is_same_v<T, OT>) {
					std::memcpy(&_values[0][0], values, std::min(this->size, size) * sizeof(T));
				}
				else {
					size_t size_left = size;
					for (size_t x = 0; x != C; ++x) {
						for (size_t y = 0; y != std::min(size_left, R); ++y) {
							_values[x][y] = (T)values[x][y];
						}
						if (size_left <= R) {
							break;
						}
						size_left -= R;
					}
				}
			}

#if _HAS_CXX20
			template<arithmetic OT>
#else
			template<class OT>
#endif
			constexpr void _copy_values_from(const OT* values, const size_t& columns, const size_t& rows) {
				size_t col_size = std::min(columns, C);
				size_t row_size = std::min(rows, R);
				if constexpr (std::is_same_v<T, OT>) {
					for (size_t x = 0; x != col_size; ++x) {
						std::memcpy(&_values[x], &values[x * row_size], row_size * sizeof(T));
						_fill_column_from(row_size, x, T(0));
					}
				}
				else {
					for (size_t x = 0; x != col_size; ++x) {
						for (size_t y = 0; y != row_size; ++y) {
							_values[x][y] = (T)values[x][y];
						}
						_fill_column_from(row_size, x, T(0));
					}
				}
			}

#if _HAS_CXX20
			template<size_t ON, arithmetic OT>
#else
			template<size_t ON, class OT>
#endif
			constexpr void _copy_values_from(const OT(&values)[ON]) {
				if constexpr (std::is_same_v<T, OT>) {
					std::memcpy(&_values[0][0], &values[0], std::min(this->size, ON) * sizeof(T));
				}
				else {
					size_t size_left = ON;
					for (size_t x = 0; x != C; ++x) {
						for (size_t y = 0; y != std::min(size_left, R); ++y) {
							_values[x][y] = (T)values[x][y];
						}
						if (size_left <= R) {
							break;
						}
						size_left -= R;
					}
				}
			}

#if _HAS_CXX20
			template<size_t OC, size_t OR, arithmetic OT>
#else
			template<size_t OC, size_t OR, class OT>
#endif
			constexpr void _copy_values_from(const OT (&values)[OC][OR]) {
				if constexpr (std::is_same_v<T, OT>) {
					std::memcpy(&_values[0][0], &values[0], std::min(this->size, OC * OR) * sizeof(T));
				}
				else {
					size_t col_size = std::min(OC, C);
					size_t row_size = std::min(OR, R);
					for (size_t x = 0; x != col_size; ++x) {
						for (size_t y = 0; y != row_size; ++y) {
							_values[x][y] = (T)values[x][y];
						}
					}
				}
			}

			template<size_t VN, class OT>
			constexpr void _copy_values_from(const vec<VN, OT>* columns, const size_t& size) {
				size_t col_size = std::min(C, size);
				size_t row_size = std::min(VN, R);
				if constexpr (std::is_same_v<T, OT>) {
					for (size_t x = 0; x != col_size; ++x) {
						std::memcpy(&_values[x][0], static_cast<const T*>(columns[x]),
							row_size * sizeof(T));
						_fill_column_from(row_size, x, T(0));
					}
				}
				else {
					for (size_t x = 0; x != col_size; ++x) {
						for (size_t y = 0; y != row_size; ++y) {
							_values[x][y] = (T)columns[x][y];
						}
						_fill_column_from(row_size, x, T(0));
					}
				}
			}

			template<size_t VN, size_t N, class OT>
			constexpr void _copy_values_from(const vec<VN, OT> (&columns)[N]) {
				size_t col_size = std::min(C, N);
				size_t row_size = std::min(R, VN);
				if constexpr (std::is_same_v<T, OT>) {
					for (size_t x = 0; x != col_size; ++x) {
						std::memcpy(&_values[x][0], static_cast<const T*>(columns[x]),
							row_size * sizeof(T));
						_fill_column_from(row_size, x, T(0));
					}
				}
				else {
					for (size_t x = 0; x != col_size; ++x) {
						for (size_t y = 0; y != row_size; ++y) {
							_values[x][y] = (T)columns[x][y];
						}
						_fill_column_from(row_size, x, T(0));
					}
				}
			}

			template<size_t OC, size_t OR, class OT>
			constexpr void _copy_values_from(const mat<OC, OR, OT>& other) {
				size_t col_size = std::min(OC, C);
				size_t row_size = std::min(OR, R);
				if constexpr (std::is_same_v<T, OT>) {
					for (size_t x = 0; x != col_size; ++x) {
						std::memcpy(&_values[x][0], static_cast<const T*>(other[x]), row_size * sizeof(T));
						_fill_column_from(row_size, x, T(0));
					}
				}
				else {
					for (size_t x = 0; x != col_size; ++x) {
						for (size_t y = 0; y != row_size; ++y) {
							_values[x][y] = (T)other[x][y];
						}
						_fill_column_from(row_size, x, T(0));
					}
				}
			}
#pragma endregion // PRIVATE_METHOD

	public:
#pragma region CONSTRUCTORS
		mat() {
			_fill_values(0);
		}

#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat(const OT* values, const size_t& size) {
			_copy_values_from(values, size);
			_fill_values_from(size, T(0));
		}

#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat(const OT* values, const size_t& columns, const size_t& rows) {
			_copy_values_from(values, columns, rows);
			_fill_values_from(columns * R, T(0));
		}
#if _HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat(const OT(&values)[ON]) {
			_copy_values_from(values);
			_fill_values_from(ON, T(0));
		}
#if _HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat(const OT(&values)[OC][OR]) {
			_copy_values_from(values);
			_fill_values_from(OC * R, T(0));
		}
#if _HAS_CXX20
		template<arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= size)
#else
		template<class... Ts, std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= C * R &&
			are_all_v<std::is_arithmetic, Ts...>), bool> = true>
#endif
		mat(const Ts&... values) {
			_set_values(std::index_sequence_for<Ts...>(), values...);
			_fill_values_from(sizeof...(Ts), T(0));
		}
		template<size_t VN, class OT>
		mat(const vec<VN, OT>* columns, const size_t& size) {
			_copy_values_from(columns, size);
			_fill_values_from(size * R, T(0));
		}
		template<size_t N, size_t VN, class OT>
		mat(const vec<VN, OT>(&columns)[N]) {
			_copy_values_from(columns);
			_fill_values_from(N * R, T(0));
		}
#if _HAS_CXX20
		template<size_t VN, arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= C)
#else
		template<size_t VN, class... Ts, std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= C), bool> = true>
#endif
		mat(const vec<VN, Ts>&... columns) {
			_set_values(std::index_sequence_for<Ts...>(), columns...);
			_fill_values_from(sizeof...(Ts) * R, T(0));
		}
		template<size_t OC, size_t OR, class OT>
		mat(const mat<OC, OR, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(OC * R, T(0));
		}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~mat() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat<C, R, T>& operator=(const OT(&values)[ON]) {
			_copy_values_from(values);
			_fill_values_from(ON, T(0));
			return *this;
		}
#if _HAS_CXX20
		template<size_t OC, size_t OR, arithmetic OT>
#else
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		mat<C, R, T>& operator=(const OT(&values)[OC][OR]) {
			_copy_values_from(values);
			_fill_values_from(OC * R, T(0));
			return *this;
		}
		template<size_t VN, class OT, size_t N>
		mat<C, R, T>& operator=(const vec<VN, OT>(&columns)[N]) {
			_copy_values_from(columns);
			_fill_values_from(N * R, T(0));
			return *this;
		}
		template<size_t OC, size_t OR, class OT>
		mat<C, R, T>& operator=(const mat<OC, OR, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(OC * R, T(0));
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static mat<C, R, T> zero() {
			return mat<C, R, T>();
		}
		static mat<C, R, T> one() {
			return fill(T(1));
		}
		static mat<C, R, T> fill(const T& value) {
			mat<C, R, T> res;
			res._fill_values(value);
			return res;
		}

#pragma region PREDEFINED_SQUARE_MATRIX
#if _HAS_CXX20
			static mat<C, R, T> identity() requires (C == R) {
#else
			static typename std::enable_if_t<(C == R), mat<C, R, T>>
			identity() {
#endif
				return fill_identity(T(1));
			}
#if _HAS_CXX20
			static mat<C, R, T> fill_identity(const T& value) requires (C == R) {
#else
			static typename std::enable_if_t<(C == R), mat<C, R, T>>
			fill_identity(const T& value) {
#endif
				mat<C, R, T> res;
				res._set_identity_values(value);
				return res;
			}

#if _HAS_CXX20
			static mat<C, R, T> translation(const vec<R - 1, T>& trans_vec) requires (C == R && R > 1) {
#else
			static typename std::enable_if_t<(C == R && R > 1), mat<C, R, T>>
			translation(const vec<R - 1, T>& trans_vec) {
#endif
				mat<C, R, T> res = mat<C, R, T>::identity();
				for (size_t y = 0; y != R - 1; ++y) {
					res[C - 1][y] = trans_vec[y];
				}
				return res;
			}

#if _HAS_CXX20
			static mat<C, R, T> scale(const vec<R - 1, T>& scale_vec) requires (C == R && R > 1) {
#else
			static typename std::enable_if_t<(C == R && R > 1), mat<C, R, T>>
			scale(const vec<R - 1, T>& scale_vec) {
#endif
				mat<C, R, T> res;
				for (size_t i = 0; i != R - 1; ++i) {
					res[i][i] = scale_vec[i];
				}
				res[C - 1][R - 1] = T(1);
				return res;
			}

#if _HAS_CXX20
			static mat<C, R, T>	scale(const T& scale_factor) requires (C == R) {
#else
			static typename std::enable_if_t<(C == R), mat<C, R, T>>
			scale(const T& scale_factor) {
#endif
				return mat<C, R, T>::fill_identity(scale_factor);
			}

#pragma region PREDEFINED_MATRIX_3x3
			template<class = std::enable_if_t<(C == R && C == 3)>>
			static mat<C, R, T> screen(const T& left, const T& right, const T& bottom, const T& top, const T& width, const T& height) {
				const T& inv_bt = 1.0 / (bottom - top);
				const T& inv_rl = 1.0 / (right - left);

				mat<C, R, T> res = mat<C, R, T>::zero();
				res[0][0] = width * inv_rl;
				res[2][0] = -width * left * inv_rl;
				res[1][1] = height * inv_bt;
				res[2][1] = -height * top * inv_bt;
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 3)>>
			static mat<C, R, T> symetric_screen(const T& right, const T& top, const T& width, const T& height) {
				return screen(-right, right, -top, top, width, height);
			}

#pragma endregion // PREDEFINED_MATRIX_3x3

#pragma region PREDEFINED_MATRIX_4x4
			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> rot_x(const T& radians) {
				T cosA = (T)std::cos(radians);
				T sinA = (T)std::sin(radians);

				// 4x4
				mat<C, R, T> res = mat<C, R, T>::identity();
				res[1][1] = cosA;
				res[2][1] = -sinA;
				res[1][2] = sinA;
				res[2][2] = cosA;
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T>	rot_y(const T& radians) {
				T cosA = (T)std::cos(radians);
				T sinA = (T)std::sin(radians);

				// 4x4
				mat<C, R, T> res = mat<C, R, T>::identity();
				res[0][0] = cosA;
				res[2][0] = sinA;
				res[0][2] = -sinA;
				res[2][2] = cosA;
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T>	rot_z(const T& radians) {
				T cosA = (T)std::cos(radians);
				T sinA = (T)std::sin(radians);

				// 4x4
				mat<C, R, T> res = mat<C, R, T>::identity();
				res[0][0] = cosA;
				res[1][0] = -sinA;
				res[0][1] = sinA;
				res[1][1] = cosA;
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T>	rot(const vec<R - 1, T>& axis, const T& radians) {
				const T& sinA = (T)std::sin(radians);
				const T& cosA = (T)std::cos(radians);
				const T& oneMinCosA = T(1) - cosA;

				vec<R - 1, T> norm_axis = axis;
				if (!norm_axis.is_zero()) norm_axis.normalize();

				mat<C, R, T> res = mat<C, R, T>::identity();
				res[0][0] = (norm_axis[0] * norm_axis[0]) + cosA * (1 - (norm_axis[0] * norm_axis[0]));
				res[0][1] = (norm_axis[0] * norm_axis[1]) * oneMinCosA - sinA * norm_axis[2];
				res[0][2] = (norm_axis[0] * norm_axis[2]) * oneMinCosA - sinA * norm_axis[1];

				res[1][0] = (norm_axis[0] * norm_axis[1]) * oneMinCosA + sinA * norm_axis[2];
				res[1][1] = (norm_axis[1] * norm_axis[1]) + cosA * (1 - (norm_axis[1] * norm_axis[1]));
				res[1][2] = (norm_axis[1] * norm_axis[2]) * oneMinCosA - sinA * norm_axis[0];

				res[2][0] = (norm_axis[0] * norm_axis[2]) * oneMinCosA - sinA * norm_axis[1];
				res[2][1] = (norm_axis[1] * norm_axis[2]) * oneMinCosA + sinA * norm_axis[0];
				res[2][2] = (norm_axis[2] * norm_axis[2]) + cosA * (1 - (norm_axis[2] * norm_axis[2]));

				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> rot(const quat<T>& quaternion) {
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
				res[0][0] = 1.f - 2.f * (y2 + z2);
				res[1][0] = 2.f * (xy - sz);
				res[2][0] = 2.f * (xz + sy);

				res[0][1] = 2.f * (xy + sz);
				res[1][1] = 1.f - 2.f * (x2 + z2);
				res[2][1] = 2.f * (yz - sx);

				res[0][2] = 2.f * (xz - sy);
				res[1][2] = 2.f * (yz + sx);
				res[2][2] = 1.f - 2.f * (x2 + y2);
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> frustrum(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far,
				const T& res_left = T(-1), const T& res_right = T(1), const T& res_bottom = T(-1), const T& res_top = T(1),
				const T& res_near = T(-1), const T& res_far = T(1)) {

				if (right == left) throw std::runtime_error("right cannot be equal left");
				if (top == bottom) throw std::runtime_error("top cannot be equal bottom");

				const T& abs_near = std::abs(near);
				const T& abs_far = std::abs(far);
				if (abs_near == abs_far) throw std::runtime_error("absolute of near cannot be equal absolute of far");

				const T& x_dir = right > left ? T(1) : T(-1);
				const T& y_dir = top > bottom ? T(1) : T(-1);
				const T& z_dir = -(x_dir * y_dir);

				const T& inv_rl = 1.f / (right - left);
				const T& inv_tb = 1.f / (top - bottom);
				const T& inv_fn = 1.f / (abs_far - abs_near);

				mat<C, R, T> res;
				res[0][0] = (res_right - res_left) * abs_near * inv_rl;
				res[2][0] = (res_left * right - res_right * left) * z_dir * inv_rl;
				res[1][1] = (res_top - res_bottom) * abs_near * inv_tb;
				res[2][1] = (res_bottom * top - res_top * bottom) * z_dir * inv_tb;
				res[2][2] = (res_far * abs_far - res_near * abs_near) * z_dir * inv_fn;
				res[3][2] = (res_near - res_far) * abs_near * abs_far * inv_fn;
				res[2][3] = z_dir;
				return res;
			}

			// left = -right, bottom = -top
			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> symetric_frustrum(const T& right, const T& top, const T& near, const T& far, const T& res_right = T(1),
				const T& res_top = T(1), const T& res_near = T(-1), const T& res_far = T(1)) {
				return frustrum(-right, right, -top, top, near, far, -res_right, res_right, -res_top, res_top, res_near, res_far);
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> perspective(const T& fov, const T& aspect, const T& near, const T& far, bool right_pos_x = true,
				bool top_pos_y = true, bool horizontal_fov = true, const T& res_right = T(1), const T& res_top = T(1),
				const T& res_near = T(-1), const T& res_far = T(1)) {

				const T& abs_near = std::abs(near);
				const T& abs_far = std::abs(far);

				T right;
				T top;
				if (horizontal_fov) {
					if (aspect == T(0)) throw std::runtime_error("aspect was zero");

					right = (T)std::tan(fov * 0.5) * abs_near;
					top = right / aspect;
				}
				else {
					top = (T)std::tan(fov * 0.5) * abs_near;
					right = top * aspect;
				}

				return symetric_frustrum((right_pos_x ? right : -right), (top_pos_y ? top : -top), abs_near, abs_far,
					res_right, res_top, res_near, res_far);
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> ortographic(const T& left, const T& right, const T& bottom, const T& top, const T& near,
				const T& far, const T& res_left = T(-1), const T& res_right = T(1), const T& res_bottom = T(-1),
				const T& res_top = T(1), const T& res_near = T(-1), const T& res_far = T(1)) {

				if (right == left) throw std::runtime_error("right cannot be equal left");
				if (top == bottom) throw std::runtime_error("top cannot be equal bottom");

				const T& abs_near = std::abs(near);
				const T& abs_far = std::abs(far);
				if (abs_near == abs_far) throw std::runtime_error("absolute of near cannot be equal absolute of far");

				const T& x_dir = right > left ? T(1) : T(-1);
				const T& y_dir = top > bottom ? T(1) : T(-1);
				const T& z_dir = -(x_dir * y_dir);

				const T& inv_rl = 1.0 / (right - left);
				const T& inv_tb = 1.0 / (top - bottom);
				const T& inv_fn = 1.0 / (abs_far - abs_near);

				mat<C, R, T> res;
				res[0][0] = (res_right - res_left) * inv_rl;
				res[3][0] = (res_left * right - res_right * left) * inv_rl;
				res[1][1] = (res_top - res_bottom) * inv_tb;
				res[3][1] = (res_bottom * top - res_top * bottom) * inv_tb;
				res[2][2] = (res_far - res_near) * z_dir * inv_fn;
				res[3][2] = (res_near * abs_far - res_far * abs_near) * inv_fn;
				res[3][3] = 1;
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> symetric_ortographic(const T& right, const T& top, const T& near,
				const T& far, const T& res_right = T(1), const T& res_top = T(1), const T& res_near = T(-1),
				const T& res_far = T(1)) {
				return ortographic(-right, right, -top, top, near, far, -res_right, res_right, -res_top, res_top, res_near, res_far);
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> view(const vec<3ull, T>& pos, const vec<3ull, T>& right, const vec<3ull, T>& forward, const vec<3ull, T>& up) {
				mat<C, R, T> res;
				res[0] = vec4(right[0], up[0], -forward[0], T(0));
				res[1] = vec4(right[1], up[1], -forward[1], T(0));
				res[2] = vec4(right[2], up[2], -forward[2], T(0));
				res[3] = vec4(-pos[0], -pos[1], -pos[2], T(1));
				return res;
			}

			template<class = std::enable_if_t<(C == R && C == 4)>>
			static mat<C, R, T> lookAt(const vec<3ull, T>& eye_pos, const vec<3ull, T>& look_at_pos, const vec<3ull, T>& world_up) {
				using vec3_type = vec<3ull, T>;
				using vec4_type = vec<4ull, T>;

				vec3_type forward = (look_at_pos - eye_pos).normalize();
				vec3_type norm_world_up = world_up.normalized();
				vec3_type right = forward.cross(world_up);
				vec3_type up = right.cross(forward);

				return view(eye_pos, right, forward, up);
			}
#pragma endregion // PREDEFINED_MATRIX_4x4
#pragma endregion // PREDEFINED_SQUARE_MATRIX
#pragma endregion // PREDEFINED

#pragma region PREDEFINED_CHECKS
		bool is_zero() const {
			return is_filled_with(T(0));
		}

		bool is_one() const {
			return is_filled_with(T(1));
		}

		bool is_filled_with(const T& value) const {
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
#if _HAS_CXX20
			bool is_identity() const requires (C == R) {
#else
			template<class = std::enable_if_t<(C == R)>>
			bool is_identity() const {
#endif
				return is_identity_filled_with(1);
			}

#if _HAS_CXX20
			bool is_identity_filled_with(const T& value) const requires (C == R) {
#else
			template<class = std::enable_if_t<(C == R)>>
			bool is_identity_filled_with(const T& value) const {
#endif
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != R; ++y) {
						if ((x == y && _values[x][y] != value) || (x != y && _values[x][y] != T(0))) {
							return false;
						}
					}
				}
				return true;
			}

#pragma endregion // PREDEFINED_SQUARE_MATRIX_CHECKS
#pragma endregion // PREDEFINED_CHECKS

#pragma region MATRIX_OPERATIONS
		mat<R, C, T> transposed() const {
			mat<R, C, T> res;
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					res[y][x] = _values[x][y];
				}
			}
			return res;
		}

#if _HAS_CXX20
			mat<C - 1, R - 1, T> get_sub_matrix(const size_t& row_idx, const size_t& col_idx) const
			requires (C > 1 &&  R > 1) {
#else
			template<class = std::enable_if_t<(R > 1 && C > 1)>>
			mat<C - 1, R - 1, T> get_sub_matrix(const size_t& row_idx, const size_t& col_idx) const {
#endif
				mat<C - 1, R - 1, T> res;
				for (size_t x = 0, sub_x = 0; x != C; ++x) {
					if (x == col_idx) continue;
					size_t sub_y = 0;

					// kopiuje wartości kolumny od 0 do row_idx - 1
					if (row_idx != 0) {
						std::memcpy(static_cast<T*>(res[sub_x]), _values[x], std::min(row_idx, R - 1) * sizeof(T));
						sub_y += row_idx;
					}
					// kopiuje wartości kolumny od row_idx + 1 do R - 1
					if (row_idx != R - 1) {
						std::memcpy(static_cast<T*>(res[sub_x]) + sub_y, _values[x] + row_idx + 1, (R - row_idx - 1) * sizeof(T));
					}
					++sub_x;
				}
				return res;
			}
#if _HAS_CXX20
			mat<C, R - 1, T> get_sub_row_matrix(const size_t& row_idx) const
			requires (R > 1) {
#else
			template<class = std::enable_if_t<(R > 1)>>
			mat<C, R - 1, T> get_sub_row_matrix(const size_t& row_idx) const {
#endif
				mat<C, R - 1, T> res;
				for (size_t x = 0; x != C; ++x) {
					size_t sub_y = 0;

					// kopiuje wartości kolumny od 0 do row_idx - 1
					if (row_idx != 0) {
						std::memcpy(static_cast<T*>(res[x]), _values[x], std::min(row_idx, R - 1) * sizeof(T));
						sub_y += row_idx;
					}
					// kopiuje wartości kolumny od row_idx + 1 do R - 1
					if (row_idx != R - 1) {
						std::memcpy(static_cast<T*>(res[x]) + sub_y, _values[x] + row_idx + 1, (R - 1 - row_idx) * sizeof(T));
					}
				}
				return res;
			}
#if _HAS_CXX20
			mat<C - 1, R, T> get_sub_col_matrix(const size_t& col_idx) const
			requires (C > 1) {
#else
			template<class = std::enable_if_t<(C > 1)>>
			mat<C - 1, R, T> get_sub_col_matrix(const size_t& col_idx) const {
#endif
				mat<C - 1, R, T> res;
				for (size_t x = 0, sub_x = 0; x != C; ++x) {
					if (x == col_idx) continue;

					// kopiuje wartości kolumny
					std::memcpy(static_cast<T*>(res[sub_x]), _values[x], R * sizeof(T));
					++sub_x;
				}
				return res;
			}

			mat<C, R, T>& clamp(const T& min_val, const T& max_val) {
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != R; ++y) {
						_values[x][y] = std::clamp(_values[x][y], min_val, max_val);
					}
				}
				return *this;
			}

			mat<C, R, T> clampped(const T& min_val, const T& max_val) const {
				mat<C, R, T> res = *this;
				return res.clamp(min_val, max_val);
			}

			mat<C, R, T>& clamp(const mat<C, R, T>& min_val, const mat<C, R, T>& max_val) {
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != R; ++y) {
						_values[x][y] = std::clamp(_values[x][y], min_val[x][y], max_val[x][y]);
					}
				}
				return *this;
			}

			mat<C, R, T> clampped(const mat<C, R, T>& min_val, const mat<C, R, T>& max_val) const {
				mat<C, R, T> res = *this;
				return res.clamp(min_val, max_val);
			}

#pragma region SQUARE_MATRIX_OPERATIONS
#if _HAS_CXX20
			mat<R, C, T>& transpose() requires (R == C) {
#else
			template<class = std::enable_if_t<(R == C)>>
			mat<R, C, T>& transpose() {
#endif
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

#if _HAS_CXX20
			T determinant() const requires (R == C) {
#else
			template<class = std::enable_if_t<(R == C)>>
			T determinant() const {
#endif
				if constexpr (R == 1) {
					return _values[0][0];
				}
				else if constexpr (R == 2) {
					return _values[0][0] * _values[1][1] - _values[0][1] * _values[1][0];
				}
				else if constexpr (R == 3) {
					T det = 0;
					if (_values[0][0] != T(0)) {
						det += _values[0][0] * (_values[1][1] * _values[2][2] - _values[2][1] * _values[1][2]);
					}
					if (_values[1][0] != T(0)) {
						det += _values[1][0] * (_values[2][1] * _values[0][2] - _values[0][1] * _values[2][2]);
					}
					if (_values[2][0] != T(0)) {
						det += _values[2][0] * (_values[0][1] * _values[1][2] - _values[1][1] * _values[0][2]);
					}
					return det;
				}
				else if constexpr (R == 4) {
					T det = 0;
					if (_values[0][0] != T(0)) {
						det += _values[0][0] *
							(_values[1][1] * (_values[2][2] * _values[3][3] - _values[3][2] * _values[2][3]) +
								_values[2][1] * (_values[3][2] * _values[1][3] - _values[1][2] * _values[3][3]) +
								_values[3][1] * (_values[1][2] * _values[2][3] - _values[2][2] * _values[1][3]));
					}
					if (_values[0][1] != T(0)) {
						det -= _values[1][0] *
							(_values[0][1] * (_values[2][2] * _values[3][3] - _values[3][2] * _values[2][3]) +
								_values[2][1] * (_values[3][2] * _values[0][3] - _values[0][2] * _values[3][3]) +
								_values[3][1] * (_values[0][2] * _values[2][3] - _values[2][2] * _values[0][3]));
					}
					if (_values[2][0] != T(0)) {
						det += _values[2][0] *
							(_values[0][1] * (_values[1][2] * _values[3][3] - _values[3][2] * _values[1][3]) +
								_values[1][1] * (_values[3][2] * _values[0][3] - _values[0][2] * _values[3][3]) +
								_values[3][1] * (_values[0][2] * _values[1][3] - _values[1][2] * _values[0][3]));
					}
					if (_values[3][0] != T(0)) {
						det -= _values[3][0] *
							(_values[0][1] * (_values[1][2] * _values[2][3] - _values[2][2] * _values[1][3]) +
								_values[1][1] * (_values[2][2] * _values[0][3] - _values[0][2] * _values[2][3]) +
								_values[2][1] * (_values[0][2] * _values[1][3] - _values[1][2] * _values[0][3]));
					}
					return det;
				}
				else {
					T det = 0;
					int sign = 1;
					for (size_t x = 0; x != C; ++x) {
						if (_values[x][0] != T(0)) {
							// get sub matrix
							mat<C - 1, R - 1, T> sub_mat = get_sub_matrix(0, x);

							// get sub matrixes det
							T sub_det = sub_mat.determinant();

							// add sub det
							det += sign * _values[x][0] * sub_det;
						}

						// change sign
						sign *= -1;
					}
					return det;
				}
			}

#if _HAS_CXX20
			mat<C, R, T>& invert() requires (R == C) {
#else
			template<class = std::enable_if_t<(R == C)>>
			mat<C, R, T>& invert() {
#endif
				*this = inverted();
				return *this;
			}
#if _HAS_CXX20
			mat<C, R, T> inverted() const requires (R == C) {
#else
			template<class = std::enable_if_t<(R == C)>>
			mat<C, R, T> inverted() const {
#endif
				if constexpr (R == 1) {
					if (_values[0][0] != T(0)) {
						throw std::runtime_error("division by zero");
					}
					return mat<C, R, T>(1.0 / _values[0][0]);
				}
				else {
					// calculate det
					T det = determinant();

					if (det == T(0)) {
						throw std::runtime_error("determinant was zero");
					}

					T invD = (T)(1.0 / det);

					mat<C, R, T> res;
					if constexpr (R == 2) {
						res[0][0] = _values[1][1] * invD;
						res[1][0] = -_values[1][0] * invD;
						res[0][1] = -_values[0][1] * invD;
						res[1][1] = _values[0][0] * invD;
					}
					else if constexpr (R == 3) {
						res[0][0] = (_values[1][1] * _values[2][2] - _values[2][1] * _values[1][2]) * invD;
						res[1][0] = (_values[2][0] * _values[1][2] - _values[1][0] * _values[2][2]) * invD;
						res[2][0] = (_values[1][0] * _values[2][1] - _values[2][0] * _values[1][1]) * invD;
						res[0][1] = (_values[0][2] * _values[2][1] - _values[0][1] * _values[2][2]) * invD;
						res[1][1] = (_values[0][0] * _values[2][2] - _values[2][0] * _values[0][2]) * invD;
						res[2][1] = (_values[2][0] * _values[0][1] - _values[0][0] * _values[2][1]) * invD;
						res[0][2] = (_values[0][1] * _values[1][2] - _values[1][1] * _values[0][2]) * invD;
						res[1][2] = (_values[1][0] * _values[0][2] - _values[0][0] * _values[1][2]) * invD;
						res[2][2] = (_values[0][0] * _values[1][1] - _values[0][1] * _values[1][0]) * invD;
					}
					else {
						// transponowana (z niej tworzymy mniejsze macierze, usuwając kolumne (x) i wiersz (y), których obliczamy det
						// det staje się wartością elementu na pozycji (x, y) ze znakiem w zależności ((x + y) % 2 == 0) -> 1 else -1 
						// na koniec mnożymy wartość elementu na pozycji (x, y) razy invD
						const mat<R, C, T>& trans = transposed();
						for (size_t x = 0; x != C; ++x) {
							for (size_t y = 0; y != R; ++y) {
								// utworzyć mniejszą macierz
								mat<R - 1, C - 1, T> sub_mat = get_sub_matrix(y, x);

								// transponujemy sub_mat
								sub_mat.transpose();

								// obliczyć det mniejszej macierzy
								T sub_det = sub_mat.determinant();

								// jeśli sub_det != 0
								if (sub_det != T(0)) {
									// ustawiamy wartość elementu x, y
									res[x][y] = (((x + y) % 2 == 0) ? 1 : -1) * sub_det * invD;
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
		mat<C, R, T>& operator+=(const mat<C, R, T>& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] += other[x][y];
				}
			}
			return *this;
		}
		mat<C, R, T>& operator-=(const mat<C, R, T>& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] -= other[x][y];
				}
			}
			return *this;
		}

		mat<C, R, T>& operator+=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] += other;
				}
			}
			return *this;
		}
		mat<C, R, T>& operator-=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] -= other;
				}
			}
			return *this;
		}
		mat<C, R, T>& operator*=(const T& other) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] *= other;
				}
			}
			return *this;
		}
		mat<C, R, T>& operator/=(const T& other) {
			if (other == T(0)) {
				throw std::runtime_error("division by zero");
			}
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					_values[x][y] /= other;
				}
			}
			return *this;
		}

		mat<C, R, T> operator+(const mat<C, R, T>& other) const {
			mat<C, R, T> res = *this;
			res += other;
			return res;
		}
		mat<C, R, T> operator-(const mat<C, R, T>& other) const {
			mat<C, R, T> res = *this;
			res -= other;
			return res;
		}
		template<size_t OC>
		mat<OC, R, T> operator*(const mat<OC, C, T>& other) const {
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

		mat<C, R, T> operator+(const T& other) const {
			mat<C, R, T> res = *this;
			res += other;
			return res;
		}
		mat<C, R, T> operator-(const T& other) const {
			mat<C, R, T> res = *this;
			res -= other;
			return res;
		}
		mat<C, R, T> operator*(const T& other) const {
			mat<C, R, T> res = *this;
			res *= other;
			return res;
		}
		friend mat<C, R, T> operator*(const T& other, const mat<C, R, T>& matrix) {
			return matrix * other;
		}
		mat<C, R, T> operator/(const T& other) const {
			mat<C, R, T> res = *this;
			res /= other;
			return res;
		}

		vec<R, T> operator*(const vec<C, T>& other) const {
			vec<R, T> res;
			for (size_t y = 0; y != R; ++y) {
				for (size_t x = 0; x != C; ++x) {
					res[y] += _values[x][y] * other[x];
				}
			}
			return res;
		}

		mat<C, R, T> operator+() const {
			return mat<C, R, T>(*this);
		}
		mat<C, R, T> operator-() const {
			return *this * -1;
		}
		mat<C, R, T>& operator++() {
			return *this += 1;
		}
		mat<C, R, T>& operator--() {
			return *this -= 1;
		}

		template<size_t OC, size_t OR>
		bool operator==(const mat<OC, OR, T>& other) const {
			if constexpr (OC != C || OR != R) {
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
		bool operator!=(const mat<OC, OR, T>& other) const {
			if constexpr (OC != C || OR != R) {
				return true;
			}
			else {
				for (size_t x = 0; x != C; ++x) {
					if (std::memcmp(_values[x], other[x], R * sizeof(T)) != 0) {
						return true;
					}
				}
				return false;
			}
		}

		operator const T* () const {
			return _values;
		}

		mat_column operator[](const size_t& idx) {
			return mat_column(this, idx);
		}
		const const_mat_column operator[](const size_t& idx) const {
			return const_mat_column(this, idx);
		}

		friend std::ostream& operator<<(std::ostream& str, const mat<C, R, T>& matrix) {
			size_t cell_width = 0;

			for (size_t y = 0; y != R; ++y) {
				for (size_t x = 0; x != C; ++x) {
					std::ostringstream oss;
					oss << matrix[y][x];
					cell_width = std::max(cell_width, oss.str().size());
				}
			}

			for (size_t y = 0; y != R; ++y) {
				if constexpr (R > 1) {
					if (y == 0) str << (char)0xda;
					else if (y == R - 1) str << (char)0xc0;
					else str << (char)0xb3;
				}
				else {
					str << (char)0xb3;
				}
				str << ' ';

				for (size_t x = 0; x != C; ++x) {
					str << std::setw(cell_width) << matrix[x][y];
					str << ' ';
				}

				if constexpr (R > 1) {
					if (y == 0) str << (char)0xbf;
					else if (y == R - 1) str << (char)0xd9;
					else str << (char)0xb3;
				}
				else {
					str << (char)0xb3;
				}

				if (y != R - 1) {
					str << std::endl;
				}
			}
			return str;
		}

#pragma region SQUARE_MATRIX_OPERATORS
#if _HAS_CXX20
		mat<C, R, T>& operator*=(const mat<C, R, T>& other) requires (R == C) {
#else
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T>& operator*=(const mat<C, R, T>& other) {
#endif
			*this = *this * other;
			return *this;
		}
#if _HAS_CXX20
		mat<C, R, T>& operator/=(const mat<C, R, T>& other) requires (R == C) {
#else
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T>& operator/=(const mat<C, R, T>& other) {
#endif
			*this *= other.inverted();
			return *this;
		}

#if _HAS_CXX20
		mat<C, R, T> operator/(const mat<C, R, T>& other) const requires (R == C) {
#else
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T> operator/(const mat<C, R, T>& other) const {
#endif
			mat<C, R, T> res = *this;
			res /= other;
			return res;
		}
#pragma endregion // SQUARE_MATRIX_OPERATORS
#pragma endregion // OPERATORS
	};

#pragma region EXTRA_OPERATIONS
	template<size_t C, size_t R, class T>
	static mat<C, R, T> clamp(const mat<C, R, T>& a, const T& min_val, const T& max_val) {
		return a.clampped(min_val, max_val);
	}

	template<size_t C, size_t R, class T>
	static mat<C, R, T> clamp(const mat<C, R, T>& a, const mat<C, R, T>& min_val, const mat<C, R, T>& max_val) {
		return a.clampped(min_val, max_val);
	}
#pragma endregion // EXTRA_OPERATIONS

#pragma region PREDEFINED_TYPES
	using mat3x2 = mat<3, 2, float>;
	using mat2x3 = mat<2, 3, float>;

	template<size_t N, class T>
	using mat_sqr = mat<N, N, T>;
	using mat3 = mat_sqr<3ull, float>;
	using dmat3 = mat_sqr<3ull, double>;
	using mat4 = mat_sqr<4ull, float>;
	using dmat4 = mat_sqr<4ull, double>;
#pragma endregion // PREDEFINED_TYPES
}
