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
#include "arithmetic_types.hpp"

namespace mstd {
#pragma region CONSTEXPR
	template<template<class> class Test, class... Ts>
	static constexpr const bool are_all_v = (Test<Ts>::value && ...);

	template<size_t Start, size_t... Indices>
	constexpr std::index_sequence<(Start + Indices)...> shift_index_sequence(std::index_sequence<Indices...>) {
		return {};
	}

	template<size_t Start, size_t End>
	using make_index_sequence_from_to = decltype(shift_index_sequence<Start>(std::make_index_sequence<End - Start>()));

	template<size_t Start, size_t Size>
	using make_index_sequence_from = decltype(shift_index_sequence<Start>(std::make_index_sequence<Size>()));

	template<size_t Start, class... Ts>
	using make_index_sequence_for_from = decltype(shift_index_sequence<Start>(std::index_sequence_for<Ts...>()));
#pragma endregion // CONSTEXPR

	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool>>
	class vec {
	public:
		static constexpr const size_t size = N;
		using value_type = T;

	private:
		T _values[N] = {};

#pragma region PRIVATE_METHODS
		template<class... Ts, size_t... Idxs>
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs] = (T)values), ...);
		}

		constexpr void _fill_values(const T& value) {
			std::fill_n(_values, N, value);
		}

		template<class OT>
		constexpr void _copy_values_from(const OT*& values, const size_t& size) {
			if (std::is_same_v<OT, T>) {
				std::memcpy(_values, values, std::min(N, size) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, size); ++i) {
					_values[i] = values[i];
				}
			}
		}
		
		template<size_t TN, class OT>
		constexpr void _copy_values_from(const OT(&values)[TN]) {
			if constexpr (std::is_same_v<T, OT>) {
				std::memcpy(_values, values, std::min(N, TN) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, TN); ++i) {
					_values[i] = values[i];
				}
			}
		}

		template<size_t ON, class OT>
		constexpr void _copy_values_from(const vec<ON, OT>& other) {
			if constexpr (std::is_same_v<OT, T>) {
				std::memcpy(_values, static_cast<const T*>(other), std::min(N, ON) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, ON); ++i) {
					_values[i] = other[i];
				}
			}
		}
#pragma endregion // PRIVATE_METHODS

	public:
#pragma region CONSTRUCTORS
		// vecN()
		vec() {
			_fill_values(0);
		}

		// vecN(x, y, ...)
		template<class... Ts, 
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N && 
				are_all_v<std::is_arithmetic, Ts...>), bool> = true>
		vec(const Ts&... values) : vec() {
			_set_values<Ts...>(std::index_sequence_for<Ts...>(), values...);
		}

		// vecN(vec, z, ...)
		template<size_t ON, class OT, class... Ts,
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N - ON && ON < N &&
				are_all_v<std::is_arithmetic, OT, Ts...>), bool> = true>
		vec(const vec<ON, OT>& other, const Ts&... values) : vec() {
			_copy_values_from(other);
			_set_values<Ts...>(make_index_sequence_for_from<ON, Ts...>(), values...);
		}

		// vecN({ 1, 2 })
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		vec(const OT (&values)[TN]) : vec() {
			_copy_values_from(values);
		}

		// vecN(&table)
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		vec(const OT* values, const size_t& size) : vec() {
			_copy_values_from(values, size);
		}

		// vecN(vecON)
		template<size_t ON, class OT>
		vec(const vec<ON, OT>& other) : vec() {
			_copy_values_from(other);
		}

#pragma region VECTOR_3_CONSTRUCTORS
		// tylko dla 3
		template<class AT, class BT, std::enable_if_t<(N == 3 && are_all_v<std::is_arithmetic, AT, BT>), bool> = true>
		vec(const vec<N, AT>& other_a, const vec<N, BT>& other_b) : vec(other_a.cross(other_b)) {}
#pragma endregion // VECTOR_3_CONSTRUCTORS
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~vec() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		vec<N, T>& operator=(const OT(&values)[TN]) {
			_fill_values(0);
			_copy_values_from(values);
			return *this;
		}
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		vec<N, T>& operator=(const vec<ON, OT>& other) {
			_fill_values(0);
			_copy_values_from(other);
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static vec<N, T> zero() {
			return vec<N, T>();
		}
		static vec<N, T> one() {
			vec<N, T> res;
			res._fill_values(1);
			return res;
		}
		static vec<N, T> fill(const T& value) {
			vec<N, T> res;
			res._fill_values(value);
			return res;
		}
#pragma endregion // PREDEFINED

#pragma region VECTOR_GETTERS
		T& x() {
			return _values[0];
		}
		T x() const {
			return _values[0];
		}

		template<class = std::enable_if_t<(N > 1)>>
		T& y() {
			return _values[1];
		}
		template<class = std::enable_if_t<(N > 1)>>
		T y() const {
			return _values[1];
		}

		template<class = std::enable_if_t<(N > 2)>>
		T& z() {
			return _values[2];
		}
		template<class = std::enable_if_t<(N > 2)>>
		T z() const {
			return _values[2];
		}

		template<class = std::enable_if_t<(N > 3)>>
		T& w() {
			return _values[3];
		}
		template<class = std::enable_if_t<(N > 3)>>
		T w() const {
			return _values[3];
		}
#pragma endregion // VECTOR_GETTERS

#pragma region VECTOR_OPERATIONS
		T length() const {
			T value = 0;
			for (size_t i = 0; i != N; ++i) {
				value += _values[i] * _values[i];
			}
			return static_cast<T>(std::sqrt(value));
		}

		vec<N, T>& normalize() {
			T len = length();
			*this /= len;
			return *this;
		}
		vec<N, T> normalized() const {
			vec<N, T> res = *this;
			return res.normalize();
		}

		T dot(const vec<N, T>& other) const {
			T res = 0;
			for (size_t i = 0; i != N; ++i) {
				res += _values[i] * other[i];
			}
			return res;
		}

		T angle_between(const vec<N, T>& other) const {
			T this_len = length();
			if (this_len == T(0)) {
				throw std::runtime_error("length was zero");
			}

			T other_len = other.length();
			if (other_len == T(0)) {
				throw std::runtime_error("length was zero");
			}

			return std::acos(dot(other) / (this_len * other_len));
		}

#pragma region VECTOR_3_OPERATIONS
		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T> cross(const vec<N, T>& other) const {
			return vec<N, T>(
				_values[1] * other[2] - _values[2] * other[1],
				_values[2] * other[0] - _values[0] * other[2],
				_values[0] * other[1] - _values[1] * other[0]
			);
		}

		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T>& rotate(const T& radians, const vec<N, T>& axis) {
			const quat<T> p(T(0), (*this));

			const vec<N, T>& norm_axis = axis.normalized();

			const quat<T>& q = quat<T>(radians, norm_axis).to_rotation_quaternion();

			const quat<T>& invers_q = q.inverted();

			*this = (q * p * invers_q).v;
			return *this;
		}
		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T> rotated(const T& radians, const vec<N, T>& axis) {
			vec<N, T> res = *this;
			return res.rotate(radians, axis);
		}
#pragma endregion // VECTOR_3_OPERATIONS
#pragma endregion // VECTOR_OPERTATIONS

#pragma region OPERATORS
		vec<N, T>& operator+=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] += other[i];
			}
			return *this;
		}
		vec<N, T>& operator-=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other[i];
			}
			return *this;
		}
		vec<N, T>& operator*=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] *= other[i];
			}
			return *this;
		}
		vec<N, T>& operator/=(const vec<N, T>& other) {
			if (other == vec<N, T>::zero()) {
				throw std::runtime_error("division by zero");
			}
			for (size_t i = 0; i != N; ++i) {
				_values[i] /= other[i];
			}
			return *this;
		}

		vec<N, T>& operator+=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] += other;
			}
			return *this;
		}
		vec<N, T>& operator-=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other;
			}
			return *this;
		}
		vec<N, T>& operator*=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] *= other;
			}
			return *this;
		}
		vec<N, T>& operator/=(const T& other) {
			if (other == 0) {
				throw std::runtime_error("division by zero");
			}
			for (size_t i = 0; i != N; ++i) {
				_values[i] /= other;
			}
			return *this;
		}

		vec<N, T> operator+(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res += other;
			return res;
		}
		vec<N, T> operator-(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res -= other;
			return res;
		}
		vec<N, T> operator*(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res *= other;
			return res;
		}
		vec<N, T> operator/(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res /= other;
			return res;
		}

		vec<N, T> operator+(const T& other) const {
			vec<N, T> res = *this;
			res += other;
			return res;
		}
		vec<N, T> operator-(const T& other) const {
			vec<N, T> res = *this;
			res -= other;
			return res;
		}
		vec<N, T> operator*(const T& other) const {
			vec<N, T> res = *this;
			res *= other;
			return res;
		}
		vec<N, T> operator/(const T& other) const {
			vec<N, T> res = *this;
			res /= other;
			return res;
		}

		vec<N, T> operator+() const {
			return vec<N, T>(*this);
		}
		vec<N, T> operator-() const {
			return *this * -1;
		}

		vec<N, T>& operator++() {
			return *this += vec<N, T>::one();
		}
		vec<N, T>& operator--() {
			return *this -= vec<N, T>::one();
		}

		template<size_t ON>
		bool operator==(const vec<ON, T>& other) const {
			if constexpr (N != ON) {
				return false;
			}
			else {
				return std::memcmp(_values, static_cast<const T*>(other), N * sizeof(T)) == 0;
			}
		}
		template<size_t ON>
		bool operator!=(const vec<ON, T>& other) const {
			if constexpr (N != ON) {
				return false;
			}
			else {
				return std::memcmp(_values, static_cast<const T*>(other), N * sizeof(T)) != 0;
			}
		}

		operator const T* () const {
			return _values;
		}

		T& operator[](const size_t& idx) {
			return _values[idx];
		}
		T operator[](const size_t& idx) const {
			return _values[idx];
		}

		friend static std::ostream& operator<<(std::ostream& str, const vec<N, T>& vector) {
			str << "[";
			for (size_t i = 0; i != N; ++i) {
				str << std::to_string(vector[i]);
				if (i != N - 1) str << ", ";
			}
			return str << "]";
		}

#pragma endregion // OPERATORS
	};

#pragma region PREDEFINED_TYPES
	using vec2 = vec<2ull, float>;
	using dvec2 = vec<2ull, double>;

	using vec3 = vec<3ull, float>;
	using dvec3 = vec<3ull, double>;

	using vec4 = vec<4ull, float>;
	using dvec4 = vec<4ull, double>;
#pragma endregion // PREDEFINED_TYPES
}

#include "quat.hpp"