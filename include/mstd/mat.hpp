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
	template<size_t C, size_t R, class T, 
		std::enable_if_t<(C > 0 && R > 0 && std::is_arithmetic_v<T>), bool>>
	class mat {
	public:
		static constexpr const size_t columns = C;
		static constexpr const size_t rows = R;
		using column_type = vec<R, T>;
		using row_type = vec<C, T>;

	private:
		T _values[C][R] = {};

#pragma region PRIVATE_METHODS
		template<class... Ts, size_t... Idxs>
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs / R][Idxs % R] = (T)values), ...);
		}

		template<size_t VN, class VT>
		constexpr void _set_column(const size_t idx, const vec<VN, VT>& column) {
			if constexpr (std::is_same_v<T, VT>) {
				std::memcpy(_values[idx], static_cast<const T*>(column), std::min(VN, R) * sizeof(T));
			}
			else {
				for (size_t y = 0; y != std::min(VN, R); ++y) {
					_values[idx][y] = column[y];
				}
			}
		}

		template<size_t VN, class... Ts, size_t... Idxs>
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const vec<VN, Ts>&... columns) {
			(_set_column(Idxs, columns), ...);
		}

		constexpr void _fill_values(const T& value) {
			for (size_t x = 0; x != C; ++x) {
				std::fill_n(_values[x], R, value);
			}
		}
		
		constexpr void _set_identity_values(const T& value) {
			for (size_t x = 0; x != C; ++x) {
				for (size_t y = 0; y != R; ++y) {
					if (x == y) {
						_values[x][y] = value;
					}
					else {
						_values[x][y] = 0;
					}
				}
			}
		}

		template<class OT>
		constexpr void _copy_values_from(const OT* values, const size_t& size) {
			size_t size_left = size;

			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != C; ++x) {
					std::memcpy(_values[x], values[x], std::min(size_left, R) * sizeof(T));
					if (size_left <= R) {
						break;
					}
					size_left -= R;
				}
			}
			else {
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != std::min(size_left, R); ++y) {
						_values[x][y] = values[x][y];
					}
					if (size_left <= R) {
						break;
					}
					size_left -= R;
				}
			}
		}

		template<class OT>
		constexpr void _copy_values_from(const OT* values, const size_t& columns, const size_t& rows) {
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != std::min(columns, C); ++x) {
					std::memcpy(_values[x], values[x], std::min(rows, R) * sizeof(T));
				}
			}
			else {
				for (size_t x = 0; x != std::min(C, columns); ++x) {
					for (size_t y = 0; y != std::min(R, rows); ++y) {
						_values[x][y] = values[x][y];
					}
				}
			}
		}

		template<size_t ON, class OT>
		constexpr void _copy_values_from(const OT(&values)[ON]) {
			size_t size_left = ON;
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != C; ++x) {
					std::memcpy(_values[x], values[x], std::min(size_left, R) * sizeof(T));
					if (size_left <= R) {
						break;
					}
					size_left -= R;
				}
			}
			else {
				for (size_t x = 0; x != C; ++x) {
					for (size_t y = 0; y != std::min(size_left, R); ++y) {
						_values[x][y] = values[x][y];
					}
					if (size_left <= R) {
						break;
					}
					size_left -= R;
				}
			}
		}

		template<size_t OC, size_t OR, class OT>
		constexpr void _copy_values_from(const OT (&values)[OC][OR]) {
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != std::min(OC, C); ++x) {
					std::memcpy(_values[x], values[x], std::min(OR, R) * sizeof(T));
				}
			}
			else {
				for (size_t x = 0; x != std::min(C, OC); ++x) {
					for (size_t y = 0; y != std::min(R, OR); ++y) {
						_values[x][y] = values[x][y];
					}
				}
			}
		}

		template<size_t VN, class OT>
		constexpr void _copy_values_from(const vec<VN, OT>* columns, const size_t& size) {
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != std::min(C, size); ++x) {
					std::memcpy(_values[x], static_cast<const T*>(columns[x]), 
						std::min(VN, R) * sizeof(T));
				}
			}
			else {
				for (size_t x = 0; x != std::min(C, size); ++x) {
					for (size_t y = 0; y != std::min(R, VN); ++y) {
						_values[x][y] = columns[x][y];
					}
				}
			}
		}

		template<size_t VN, size_t N, class OT>
		constexpr void _copy_values_from(const vec<VN, OT> (&columns)[N]) {
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != std::min(C, N); ++x) {
					std::memcpy(_values[x], static_cast<const T*>(columns[x]),
						std::min(R, VN) * sizeof(T));
				}
			}
			else {
				for (size_t x = 0; x != std::min(C, N); ++x) {
					for (size_t y = 0; y != std::min(R, VN); ++y) {
						_values[x][y] = columns[x][y];
					}
				}
			}
		}

		template<size_t OC, size_t OR, class OT>
		constexpr void _copy_values_from(const mat<OC, OR, OT>& other) {
			if constexpr (std::is_same_v<T, OT>) {
				for (size_t x = 0; x != std::min(OC, C); ++x) {
					std::memcpy(_values[x], other[x], std::min(OR, R) * sizeof(T));
				}
			}
			else {
				for (size_t x = 0; x != std::min(C, OC); ++x) {
					for (size_t y = 0; y != std::min(R, OR); ++y) {
						_values[x][y] = other[x][y];
					}
				}
			}
		}
#pragma endregion // PRIVATE_METHOD

	public:
#pragma region CONSTRUCTORS
		mat() {
			_fill_values(0);
		}
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat(const OT* values, const size_t& size) : mat() {
			_copy_values_from(values, size);
		}
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat(const OT* values, const size_t& columns, const size_t& rows) : mat() {
			_copy_values_from(values, columns, rows);
		}
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat(const OT(&values)[ON]) : mat() {
			_copy_values_from(values);
		}
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat(const OT(&values)[OC][OR]) : mat() {
			_copy_values_from(values);
		}
		template<class... Ts, std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= C * R && 
			are_all_v<std::is_arithmetic, Ts...>), bool> = true>
		mat(const Ts&... values) : mat() {
			_set_values(std::index_sequence_for<Ts...>(), values...);
		}
		template<size_t VN, class OT>
		mat(const vec<VN, OT>* columns, const size_t& size) : mat() {
			_copy_values_from(columns, size);
		}
		template<size_t N, size_t VN, class OT>
		mat(const vec<VN, OT>(&columns)[N]) : mat() {
			_copy_values_from(columns);
		}
		template<size_t VN, class... Ts, std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= C), bool> = true>
		mat(const vec<VN, Ts>&... columns) : mat() {
			_set_values(std::index_sequence_for<Ts...>(), columns...);
		}
		template<size_t OC, size_t OR, class OT>
		mat(const mat<OC, OR, OT>& other) : mat() {
			_copy_values_from(other);
		}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~mat() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat<C, R, T>& operator=(const OT(&values)[ON]) {
			_fill_values(0);
			_copy_values_from(values);
			return *this;
		}
		template<size_t OC, size_t OR, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		mat<C, R, T>& operator=(const OT(&values)[OC][OR]) {
			_fill_values(0);
			_copy_values_from(values);
			return *this;
		}
		template<size_t VN, class OT, size_t N>
		mat<C, R, T>& operator=(const vec<VN, OT>(&columns)[N]) {
			_fill_values(0);
			_copy_values_from(columns);
			return *this;
		}
		template<size_t OC, size_t OR, class OT>
		mat<C, R, T>& operator=(const mat<OC, OR, OT>& other) {
			_fill_values(0);
			_copy_values_from(other);
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static mat<C, R, T> zero() {
			return mat<C, R, T>();
		}
		static mat<C, R, T> one() {
			mat<C, R, T> res;
			res._fill_values(1);
			return res;
		}
		static mat<C, R, T> fill(const T& value) {
			mat<C, R, T> res;
			res._fill_values(value);
			return res;
		}
		static mat<C, R, T> identity() {
			mat<C, R, T> res;
			res._set_identity_values(1);
			return res;
		}
		static mat<C, R, T> fill_identity(const T& value) {
			mat<C, R, T> res;
			res._set_identity_values(value);
			return res;
		}

#pragma region PREDEFINED_SQUARE_MATRIX
		static typename std::enable_if_t<(C == R && R > 1), mat<C, R, T>> 
			translation(const vec<R - 1, T>& trans_vec) {
			mat<C, R, T> res = mat<C, R, T>::identity();
			for (size_t y = 0; y != R - 1; ++y) {
				res[C - 1][y] = trans_vec[y];
			}
			return res;
		}

		static typename std::enable_if_t<(C == R && R > 1), mat<C, R, T>> 
			scale(const vec<R - 1, T>& scale_vec) {
			mat<C, R, T> res = mat<C, R, T>::identity();
			for (size_t i = 0; i != R - 1; ++i) {
				res[i][i] = scale_vec[i];
			}
			return res;
		}

		static typename std::enable_if_t<(C == R), mat<C, R, T>>
			scale(const T& scale_factor) {
			return mat<C, R, T>::fill_identity(scale_factor);
		}

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
		static mat<C, R, T>	rot(const T& radians, const vec<R - 1, T>& axis) {
			const vec<R - 1, T>& normAxis = axis.normalized();
			
			const T& sinA = (T)std::sin(radians);
			const T& cosA = (T)std::cos(radians);
			const T& oneMinCosA = T(1) - cosA;

			mat<C, R, T> res = mat<C, R, T>::identity();
			res[0][0] = (normAxis[0] * normAxis[0]) + cosA * (1 - (normAxis[0] * normAxis[0]));
			res[0][1] = (normAxis[0] * normAxis[1]) * oneMinCosA - sinA * normAxis[2];
			res[0][2] = (normAxis[0] * normAxis[2]) * oneMinCosA - sinA * normAxis[1];

			res[1][0] = (normAxis[0] * normAxis[1]) * oneMinCosA + sinA * normAxis[2];
			res[1][1] = (normAxis[1] * normAxis[1]) + cosA * (1 - (normAxis[1] * normAxis[1]));
			res[1][2] = (normAxis[1] * normAxis[2]) * oneMinCosA - sinA * normAxis[0];

			res[2][0] = (normAxis[0] * normAxis[2]) * oneMinCosA - sinA * normAxis[1];
			res[2][1] = (normAxis[1] * normAxis[2]) * oneMinCosA + sinA * normAxis[0];
			res[2][2] = (normAxis[2] * normAxis[2]) + cosA * (1 - (normAxis[2] * normAxis[2]));

			return res;
		}
#pragma endregion // PREDEFINED_MATRIX_4x4
#pragma endregion // PREDEFINED_SQUARE_MATRIX
#pragma endregion // PREDEFINED

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

		template<class = std::enable_if_t<(R > 1 && C > 1)>>
		mat<R - 1, C - 1, T> get_sub_matrix(const size_t& row_idx, const size_t& col_idx) const {
			mat<R - 1, C - 1, T> res;
			for (size_t x = 0, sub_x = 0; x != C; ++x) {
				if (x == col_idx) continue;
				size_t sub_y = 0;

				// kopiuje wartości kolumny od 0 do row_idx - 1
				if (row_idx != 0) {
					std::memcpy(res[sub_x], _values[x], std::min(row_idx, R - 1) * sizeof(T));
					sub_y += row_idx;
				}
				// kopiuje wartości kolumny od row_idx + 1 do R - 1
				if (row_idx != R - 1) {
					std::memcpy(res[sub_x] + sub_y, _values[x] + row_idx + 1, (R - row_idx - 1) * sizeof(T));
				}
				++sub_x;
			}
			return res;
		}
		template<class = std::enable_if_t<(R > 1)>>
		mat<R - 1, C, T> get_sub_row_matrix(const size_t& row_idx) const {
			mat<R - 1, C, T> res;
			for (size_t x = 0; x != C; ++x) {
				size_t sub_y = 0;

				// kopiuje wartości kolumny od 0 do row_idx - 1
				if (row_idx != 0) {
					std::memcpy(res[x], _values[x], std::min(row_idx, R - 1) * sizeof(T));
					sub_y += row_idx;
				}
				// kopiuje wartości kolumny od row_idx + 1 do R - 1
				if (row_idx != R - 1) {
					std::memcpy(res[x] + sub_y, _values[x] + row_idx + 1, (R - 1 - row_idx) * sizeof(T));
				}
			}
			return res;
		}
		template<class = std::enable_if_t<(C > 1)>>
		mat<R, C - 1, T> get_sub_col_matrix(const size_t& col_idx) const {
			mat<R, C - 1, T> res;
			for (size_t x = 0, sub_x = 0; x != C; ++x) {
				if (x == col_idx) continue;

				// kopiuje wartości kolumny
				std::memcpy(res[sub_x], _values[x], R * sizeof(T));
				++sub_x;
			}
			return res;
		}
#pragma region SQUARE_MATRIX_OPERATIONS
		template<class = std::enable_if_t<(R == C)>>
		mat<R, C, T>& transpose() {
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

		template<class = std::enable_if_t<(R == C)>>
		T determinant() const {
			if constexpr (R == 1) {
				return _values[0][0];
			}
			else if constexpr (R == 2) {
				return _values[0][0] * _values[1][1] - _values[0][1] * _values[1][0];
			}
			else if constexpr (R == 3) {
				T det = 0;
				if (_values[0][0] != 0) {
					det += _values[0][0] * (_values[1][1] * _values[2][2] - _values[2][1] * _values[1][2]);
				}
				if (_values[1][0] != 0) {
					det += _values[1][0] * (_values[2][1] * _values[0][2] - _values[0][1] * _values[2][2]);
				}
				if (_values[2][0] != 0) {
					det += _values[2][0] * (_values[0][1] * _values[1][2] - _values[1][1] * _values[0][2]);
				}
				return det;
			}
			else if constexpr (R == 4) {
				T det = 0;
				if (_values[0][0] != 0) {
					det += _values[0][0] * 
						(_values[1][1] * (_values[2][2] * _values[3][3] - _values[3][2] * _values[2][3]) +
						_values[2][1] * (_values[3][2] * _values[1][3] - _values[1][2] * _values[3][3]) +
						_values[3][1] * (_values[1][2] * _values[2][3] - _values[2][2] * _values[1][3]));
				}
				if (_values[0][1] != 0) {
					det -= _values[1][0] *
						(_values[0][1] * (_values[2][2] * _values[3][3] - _values[3][2] * _values[2][3]) +
						_values[2][1] * (_values[3][2] * _values[0][3] - _values[0][2] * _values[3][3]) +
						_values[3][1] * (_values[0][2] * _values[2][3] - _values[2][2] * _values[0][3]));
				}
				if (_values[2][0] != 0) {
					det += _values[2][0] *
						(_values[0][1] * (_values[1][2] * _values[3][3] - _values[3][2] * _values[1][3]) +
						_values[1][1] * (_values[3][2] * _values[0][3] - _values[0][2] * _values[3][3]) +
						_values[3][1] * (_values[0][2] * _values[1][3] - _values[1][2] * _values[0][3]));
				}
				if (_values[3][0] != 0) {
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
					if (_values[x][0] != 0) {
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

		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T>& invert() {
			*this = inverted();
			return *this;
		}
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T> inverted() const {
			if constexpr (R == 1) {
				if (_values[0][0] != 0) {
					throw std::runtime_error("division by zero");
				}
				return mat<C, R, T>(1.0 / _values[0][0]);
			}
			else {
				// calculate det
				T det = determinant();

				if (det == 0) {
					throw std::runtime_error("determinant was zero");
				}

				T invD = 1.0 / det;

				mat<C, R, T> res;
				if constexpr (R == 2) {
					res[0][0] = _values[1][1] * invD;
					res[1][0] = -_values[1][0] * invD;
					res[0][1] = -_values[0][1] * invD;
					res[1][1] = _values[0][0] * invD;
				}
				else if constexpr (R == 3) {
					res[0][0] = (_values[1][1] * _values[2][2] - _values[2][1] * _values[1][2]) * invD;
					res[1][0] = -(_values[1][0] * _values[2][2] - _values[2][0] * _values[1][2]) * invD;
					res[2][0] = (_values[1][0] * _values[2][1] - _values[2][0] * _values[1][1]) * invD;
					res[0][1] = -(_values[0][1] * _values[2][2] - _values[2][1] * _values[0][2]) * invD;
					res[1][1] = -(_values[0][0] * _values[2][2] - _values[0][2] * _values[2][0]) * invD;
					res[2][1] = -(_values[0][0] * _values[1][2] - _values[0][2] * _values[1][0]) * invD;
					res[0][2] = (_values[0][1] * _values[1][2] - _values[0][2] * _values[1][1]) * invD;
					res[1][2] = -(_values[0][0] * _values[1][2] - _values[0][2] * _values[1][0]) * invD;
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
			if (other == 0) {
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
		mat<R, R, T> operator*(const mat<R, C, T>& other) const {
			mat<R, R, T> res;
			for (size_t x = 0; x != R; ++x) {
				for (size_t y = 0; y != R; ++y) {
					res[x][y] = 0;
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

		template<class = std::enable_if_t<(C > 1)>>
		vec<R, T> operator*(const vec<C - 1, T>& other) const {
			return (*this) * vec<C, T>(other, 1);
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

		T* operator[](const size_t& idx) {
			return _values[idx];
		}
		const T* operator[](const size_t& idx) const {
			return _values[idx];
		}

		friend static std::ostream& operator<<(std::ostream& str, const mat<C, R, T>& matrix) {
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
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T>& operator*=(const mat<C, R, T>& other) {
			*this = *this * other;
			return *this;
		}
		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T>& operator/=(const mat<C, R, T>& other) {
			*this *= other.inverted();
			return *this;
		}

		template<class = std::enable_if_t<(R == C)>>
		mat<C, R, T> operator/(const mat<C, R, T>& other) const {
			mat<C, R, T> res = *this;
			res /= other;
			return res;
		}
#pragma endregion // SQUARE_MATRIX_OPERATORS
#pragma endregion // OPERATORS
	};

#pragma region PREDEFINED_TYPES
	using mat3x2 = mat<3, 2, float>;

	template<size_t N, class T>
	using mat_sqr = mat<N, N, T>;
	using mat3 = mat_sqr<3, float>;
	using mat4 = mat_sqr<4, float>;
#pragma endregion // PREDEFINED_TYPES
}