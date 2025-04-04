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

#pragma region PREDEFINED_CHECKS
		bool is_zero() const {
			return is_filled_with(T(0));
		}

		bool is_one() const {
			return is_filled_with(T(1));
		}

		bool is_filled_with(const T& value) {
			for (size_t i = 0; i != N; ++i) {
				if (_values[i] != value) {
					return false;
				}
			}
			return true;
		}

		bool is_normalized() const {
			return length() == T(1);
		}
#pragma endregion // PREDEFINED_CHECKS

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

		vec<N, T>& reflect(const vec<N, T>& other) noexcept {
			float dotProduct = this->dot(other);
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= 2.0f * dotProduct * other[i];
			}
			return *this;
		}

		vec<N, T> reflected(const vec<N, T>& other) const noexcept {
			vec<N, T> res = *this;
			return res.reflect(other);
		}

		vec<N, T>& refract(const vec<N, T>& other, const T& eta) {
			float N_dot_I = other.dot(*this);
			float k = 1.f - (float)eta * (float)eta * (1.f - N_dot_I * N_dot_I);
			for (size_t i = 0; i != N; ++i) {
				if (k >= 0.f) {
					_values[i] = T((float)eta * (float)_values[i] - ((float)eta * N_dot_I + sqrtf(k)) * (float)other[i]);
				}
				else {
					_values[i] = T(0);
				}
			}
			return *this;
		}

		vec<N, T> refracted(const vec<N, T>& other, const T& eta) const {
			vec<N, T> res = *this;
			return res.refract(other, eta);
		}

		vec<N, T>& saturate() noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = saturate(_values[i]);
			}
			return *this;
		}

		vec<N, T> saturated() const noexcept {
			vec<N, T> res = *this;
			return res.saturate();
		}

		vec<N, T>& fract() noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = fract(_values[i]);
			}
			return *this;
		}

		vec<N, T> fracted() const noexcept {
			vec<N, T> res = *this;
			return res.fract();
		}

		vec<N, T>& mod(const T& y) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= y * std::floor(_values[i] / y);
			}
			return *this;
		}

		vec<N, T> modded(const T& y) const {
			vec<N, T> res = *this;
			return res.mod(y);
		}

		vec<N, T>& mod(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other[i] * std::floor(_values[i] / other[i]);
			}
			return *this;
		}

		vec<N, T> modded(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			return res.mod(other);
		}
		
		vec<N, T>& pow(const T& y) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::pow(_values[i], y);
			}
			return *this;
		}

		vec<N, T> powed(const T& y) const {
			vec<N, T> res = *this;
			return res.pow(y);
		}

		vec<N, T>& pow(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::pow(_values[i], other[i]);
			}
			return *this;
		}

		vec<N, T> powed(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			return res.pow(other);
		}
		
					vec<N, T>& clamp(const T& min_val, const T& max_val) {
				for (size_t i = 0; i != N; ++i) {
					_values[i] = std::clamp(_values[i], min_val, max_val);
				}
				return *this;
			}

			vec<N, T> clampped(const T& min_val, const T& max_val) const {
				vec<N, T> res = *this;
				return res.clamp(min_val, max_val);
			}

			vec<N, T>& clamp(const vec<N, T>& min_val, const vec<N, T>& max_val) {
				for (size_t i = 0; i != N; ++i) {
					_values[i] = std::clamp(_values[i], min_val[i], max_val[i]);
				}
				return *this;
			}

			vec<N, T> clampped(const vec<N, T>& min_val, const vec<N, T>& max_val) const {
				vec<N, T> res = *this;
				return res.clamp(min_val, max_val);
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
			if (other == T(0)) {
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
		friend static vec<N, T> operator*(const T& other, const vec<N, T>& vector) {
			return vector * other;
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

#pragma region EXTRA_OPERATORS
	template<class T, size_t N>
	static vec<N, T> max(const vec<N, T>& a, const vec<N, T>& b) noexcept {
		vec<N, T> res;
		for (size_t i = 0; i != N; ++i) {
			res[i] = std::max(a[i], b[i]);
		}
		return res;
	}

	template<class T, size_t N>
	static vec<N, T> min(const vec<N, T>& a, const vec<N, T>& b) noexcept {
		vec<N, T> res;
		for (size_t i = 0; i != N; ++i) {
			res[i] = std::min(a[i], b[i]);
		}
		return res;
	}

	template<class T, size_t N>
	static T dot(const vec<N, T>& a, const vec<N, T>& b) {
		return a.dot(b);
	}

	template<class T, size_t N>
	static vec<N, T> cross(const vec<N, T>& a, const vec<N, T>& b) {
		return a.cross(b);
	}
	
	template<class T, size_t N>
	static T angle_between(const vec<N, T>& a, const vec<N, T>& b) {
		return a.angle_between(b);
	}

	template<class T, size_t N>
	static vec<N, T> reflect(const vec<N, T>& a, const vec<N, T>& b) {
		return a.reflected(b);
	}

	template<class T, size_t N>
	static vec<N, T> refract(const vec<N, T>& a, const vec<N, T>& b) {
		return a.refracted(b);
	}

	template<class T, size_t N>
	static vec<N, T> saturate(const vec<N, T>& a) {
		return a.saturated();
	}

	template<class T, size_t N>
	static vec<N, T> fract(const vec<N, T>& a) {
		return a.fracted();
	}

	template<class T, size_t N>
	static vec<N, T> mod(const vec<N, T>& a, const T& y) {
		return a.modded(y);
	}

	template<class T, size_t N>
	static vec<N, T> mod(const vec<N, T>& a, const vec<N, T>& b) {
		return a.modded(b);
	}

	template<class T, size_t N>
	static vec<N, T> pow(const vec<N, T>& a, const T& y) {
		return a.powed(y);
	}

	template<class T, size_t N>
	static vec<N, T> pow(const vec<N, T>& a, const vec<N, T>& b) {
		return a.powed(b);
	}
	
	template<class T, size_t N>
	static vec<N, T> clamp(const vec<N, T>& a, const T& min_val, const T& max_val) {
		return a.clampped(min_val, max_val);
	}

	template<class T, size_t N>
	static vec<N, T> clamp(const vec<N, T>& a, const vec<N, T>& min_val, const vec<N, T>& max_val) {
		return a.clampped(min_val, max_val);
	}
#pragma endregion // EXTRA_OPERATORS

#pragma region PREDEFINED_TYPES
	using vec2 = vec<2ull, float>;
	using dvec2 = vec<2ull, double>;
	using ldvec2 = vec<2ull, long double>;
	using ivec2 = vec<2ull, int>;
	using uvec2 = vec<2ull, unsigned int>;
	using bvec2 = vec<2ull, bool>;
	using cvec2 = vec<2ull, char>;
	using ucvec2 = vec<2ull, unsigned char>;
	using scvec2 = vec<2ull, signed char>;
	using lvec2 = vec<2ull, long>;
	using ulvec2 = vec<2ull, unsigned long>;
	using llvec2 = vec<2ull, long long>;
	using ullvec2 = vec<2ull, unsigned long long>;

	using vec3 = vec<3ull, float>;
	using dvec3 = vec<3ull, double>;
	using ldvec3 = vec<3ull, long double>;
	using ivec3 = vec<3ull, int>;
	using uvec3 = vec<3ull, unsigned int>;
	using bvec3 = vec<3ull, bool>;
	using cvec3 = vec<3ull, char>;
	using ucvec3 = vec<3ull, unsigned char>;
	using scvec3 = vec<3ull, signed char>;
	using lvec3 = vec<3ull, long>;
	using ulvec3 = vec<3ull, unsigned long>;
	using llvec3 = vec<3ull, long long>;
	using ullvec3 = vec<3ull, unsigned long long>;

	using vec4 = vec<4ull, float>;
	using dvec4 = vec<4ull, double>;
	using ldvec4 = vec<4ull, long double>;
	using ivec4 = vec<4ull, int>;
	using uvec4 = vec<4ull, unsigned int>;
	using bvec4 = vec<4ull, bool>;
	using cvec4 = vec<4ull, char>;
	using ucvec4 = vec<4ull, unsigned char>;
	using scvec4 = vec<4ull, signed char>;
	using lvec4 = vec<4ull, long>;
	using ulvec4 = vec<4ull, unsigned long>;
	using llvec4 = vec<4ull, long long>;
	using ullvec4 = vec<4ull, unsigned long long>;
#pragma endregion // PREDEFINED_TYPES
}

#include "quat.hpp"