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

#if _HAS_CXX20
	template<size_t N, arithmetic T>
	requires (N > 0)
#else
	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool>>
#endif
	class vec {
	public:
		static constexpr const size_t size = N;
		using value_type = T;

	private:
		T _values[N] = {};

#pragma region PRIVATE_METHODS
#if _HAS_CXX20
		template<arithmetic... Ts, size_t... Idxs>
#else
		template<class... Ts, size_t... Idxs>
#endif
		constexpr void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs] = (T)values), ...);
		}

		constexpr void _fill_values(const T& value) {
			std::fill_n(&_values[0], N, value);
		}

		constexpr void _fill_values_from(size_t first_idx, const T& value) {
			if (first_idx >= N) return;
			std::fill_n(&_values[0] + first_idx, N - first_idx, value);
		}

#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT>
#endif
		constexpr void _copy_values_from(const OT*& values, const size_t& size) {
			if constexpr (std::is_same_v<OT, T>) {
				std::memcpy(&_values[0], values, std::min(N, size) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, size); ++i) {
					_values[i] = (T)values[i];
				}
			}
		}

#if _HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT>
#endif
		constexpr void _copy_values_from(const OT(&values)[TN]) {
			if constexpr (std::is_same_v<T, OT>) {
				std::memcpy(&_values[0], &values[0], std::min(N, TN) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, TN); ++i) {
					_values[i] = (T)values[i];
				}
			}
		}

#if _HAS_CXX20
		template<size_t ON, arithmetic OT>
		requires (ON > 0)
#else
		template<size_t ON, class OT>
#endif
		constexpr void _copy_values_from(const vec<ON, OT>& other) {
			if constexpr (std::is_same_v<OT, T>) {
				std::memcpy(&_values[0], static_cast<const T*>(other), std::min(N, ON) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, ON); ++i) {
					_values[i] = (T)other[i];
				}
			}
		}
#pragma endregion // PRIVATE_METHODS

	public:
#pragma region CONSTRUCTORS
		// vecN()
		vec() {
			_fill_values(T(0));
		}

		// vecN(x, y, ...)
#if _HAS_CXX20
		template<arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= N)
#else
		template<class... Ts,
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N &&
				are_all_v<std::is_arithmetic, Ts...>), bool> = true>
#endif
		vec(const Ts&... values) {
			_set_values<Ts...>(std::index_sequence_for<Ts...>(), values...);
			_fill_values_from(sizeof...(Ts), 0);
		}

		// vecN(vec, z, ...)
#if _HAS_CXX20
		template<size_t ON, arithmetic OT, arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= N - ON && ON < N)
#else
		template<size_t ON, class OT, class... Ts,
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N - ON && ON < N &&
				are_all_v<std::is_arithmetic, OT, Ts...>), bool> = true>
#endif
		vec(const vec<ON, OT>& other, const Ts&... values) {
			_copy_values_from(other);
			_set_values<Ts...>(make_index_sequence_for_from<ON, Ts...>(), values...);
			_fill_values_from(sizeof...(Ts) + ON, 0);
		}

		// vecN({ 1, 2 })
#if _HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		vec(const OT (&values)[TN]) {
			_copy_values_from(values);
			_fill_values_from(TN, 0);
		}

		// vecN(&table)
#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		vec(const OT* values, const size_t& size) {
			_copy_values_from(values, size);
			_fill_values_from(size, 0);
		}

		// vecN(vecON)
#if _HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT>
#endif
		vec(const vec<ON, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(ON, 0);
		}

#pragma region VECTOR_3_CONSTRUCTORS
#if _HAS_CXX20
		template<arithmetic AT, arithmetic BT, size_t ON>
		requires (ON == 3)
#else
		template<class AT, class BT, size_t ON, std::enable_if_t<(ON == 3), bool> = true>
#endif
		vec(const vec<ON, AT>& other_a, const vec<ON, BT>& other_b) : vec(other_a.cross(other_b)) {}
#pragma endregion // VECTOR_3_CONSTRUCTORS
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~vec() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		vec<N, T>& operator=(const OT(&values)[TN]) {
			_copy_values_from(values);
			_fill_values_from(TN, 0);
			return *this;
		}
#if _HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		vec<N, T>& operator=(const vec<ON, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(ON, 0);
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static vec<N, T> zero() {
			return vec<N, T>();
		}
		static vec<N, T> one() {
			return fill(T(1));
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

		bool is_filled_with(const T& value) const {
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

		T& r() {
			return _values[0];
		}
		T r() const {
			return _values[0];
		}

#if _HAS_CXX20
		T& y() requires (N > 1) {
#else
		template<class = std::enable_if_t<(N > 1)>>
		T& y() {
#endif
			return _values[1];
		}
#if _HAS_CXX20
		T y() const requires (N > 1) {
#else
		template<class = std::enable_if_t<(N > 1)>>
		T y() const {
#endif
			return _values[1];
		}

#if _HAS_CXX20
		T& g() requires (N > 1) {
#else
		template<class = std::enable_if_t<(N > 1)>>
		T& g() {
#endif
			return _values[1];
		}
#if _HAS_CXX20
		T g() const requires (N > 1) {
#else
		template<class = std::enable_if_t<(N > 1)>>
		T g() const {
#endif
			return _values[1];
		}
			
#if _HAS_CXX20
		T& z() requires (N > 2) {
#else
		template<class = std::enable_if_t<(N > 2)>>
		T& z() {
#endif
			return _values[2];
		}
#if _HAS_CXX20
		T z() const requires (N > 2) {
#else
		template<class = std::enable_if_t<(N > 2)>>
		T z() const {
#endif
			return _values[2];
		}

#if _HAS_CXX20
		T& b() requires (N > 2) {
#else
		template<class = std::enable_if_t<(N > 2)>>
		T& b() {
#endif
			return _values[2];
		}
#if _HAS_CXX20
		T b() const requires (N > 2) {
#else
		template<class = std::enable_if_t<(N > 2)>>
		T b() const {
#endif
			return _values[2];
		}

#if _HAS_CXX20
		T& w() requires (N > 3) {
#else
		template<class = std::enable_if_t<(N > 3)>>
		T& w() {
#endif
			return _values[3];
		}
#if _HAS_CXX20
		T w() const requires (N > 3) {
#else
		template<class = std::enable_if_t<(N > 3)>>
		T w() const {
#endif
			return _values[3];
		}

#if _HAS_CXX20
		T& a() requires (N > 3) {
#else
		template<class = std::enable_if_t<(N > 3)>>
		T& a() {
#endif
			return _values[3];
		}
#if _HAS_CXX20
		T a() const requires (N > 3) {
#else
		template<class = std::enable_if_t<(N > 3)>>
		T a() const {
#endif
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
			if (len == T(0)) {
				throw std::runtime_error("length was zero");
			}
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

		vec<N, T>& reflect(const vec<N, T>& normal) noexcept {
			*this -= 2.0f * this->dot(normal) * normal;
			return *this;
		}

		vec<N, T> reflected(const vec<N, T>& normal) const noexcept {
			vec<N, T> res = *this;
			return res.reflect(normal);
		}

		vec<N, T>& refract(const vec<N, T>& normal, const T& eta) {
			*this = this->refracted(normal, eta);
			return *this;
		}

		vec<N, T> refracted(const vec<N, T>& normal, const T& eta) const {
			float cos_theta = std::min(dot(-(*this), normal), 1.0f);
			vec<N, T> r_out_perp = eta * (*this + cos_theta * normal);
			float length = r_out_perp.length();
			vec<N, T> r_out_parallel = -std::sqrt(std::abs(1.0f - length * length)) * normal;
			return r_out_perp + r_out_parallel;
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
#if _HAS_CXX20
		vec<N, T> cross(const vec<N, T>& other) const requires (N == 3) {
#else
		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T> cross(const vec<N, T>& other) const {
#endif
			return vec<N, T>(
				_values[1] * other[2] - _values[2] * other[1],
				_values[2] * other[0] - _values[0] * other[2],
				_values[0] * other[1] - _values[1] * other[0]
			);
		}

#if _HAS_CXX20
		vec<N, T>& rotate(const vec<N, T>& axis, const T& radians) requires (N == 3) {
#else
		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T>& rotate(const vec<N, T>& axis, const T& radians) {
#endif
			const quat<T> p(T(0), (*this));

			vec<N, T> norm_axis = axis;
			if (!norm_axis.is_zero()) norm_axis.normalize();

			const quat<T>& q = quat<T>::rotation(norm_axis, radians);

			const quat<T>& invers_q = q.inverted();

			*this = (q * p * invers_q).v;
			return *this;
		}
#if _HAS_CXX20
		vec<N, T> rotated(const vec<N, T>& axis, const T& radians) requires (N == 3) {
#else
		template<class = std::enable_if_t<(N == 3)>>
		vec<N, T> rotated(const vec<N, T>& axis, const T& radians) {
#endif
			vec<N, T> res = *this;
			return res.rotate(axis, radians);
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
		friend vec<N, T> operator*(const T& other, const vec<N, T>& vector) {
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

		friend std::ostream& operator<<(std::ostream& str, const vec<N, T>& vector) {
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

#if _HAS_CXX20
	template<class T, size_t N>
	requires (N == 3)
#else
	template<class T, size_t N, std::enable_if_t<(N == 3), bool> = true>
#endif
	static vec<N, T> cross(const vec<N, T>& a, const vec<N, T>& b) {
		return a.cross(b);
	}

	template<class T, size_t N>
	static T angle_between(const vec<N, T>& a, const vec<N, T>& b) {
		return a.angle_between(b);
	}

	template<class T, size_t N>
	static vec<N, T> reflect(const vec<N, T>& dir, const vec<N, T>& normal) {
		return dir.reflected(normal);
	}

	template<class T, size_t N>
	static vec<N, T> refract(const vec<N, T>& dir, const vec<N, T>& normal, const T& eta) {
		return dir.refracted(normal, eta);
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
