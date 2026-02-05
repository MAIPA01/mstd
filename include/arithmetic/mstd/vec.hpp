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
#include "math_functions.hpp"

namespace mstd {
#pragma region CONSTEXPR
	template<template<class> class Test, class... Ts>
	static _MSTD_CONSTEXPR20 const bool are_all_v = (Test<Ts>::value && ...);

	template<size_t Start, size_t... Indices>
	_MSTD_CONSTEXPR20 std::index_sequence<(Start + Indices)...> shift_index_sequence(std::index_sequence<Indices...>) {
		return {};
	}

	template<size_t Start, size_t End>
	using make_index_sequence_from_to = decltype(shift_index_sequence<Start>(std::make_index_sequence<End - Start>()));

	template<size_t Start, size_t Size>
	using make_index_sequence_from = decltype(shift_index_sequence<Start>(std::make_index_sequence<Size>()));

	template<size_t Start, class... Ts>
	using make_index_sequence_for_from = decltype(shift_index_sequence<Start>(std::index_sequence_for<Ts...>()));
#pragma endregion // CONSTEXPR

#if _MSTD_HAS_CXX20
	template<size_t N, arithmetic T>
	requires (N > 0)
#else
	template<size_t N, class T, std::enable_if_t<(N > 0 && std::is_arithmetic_v<T>), bool>>
#endif
	class vec {
	public:
		static _MSTD_CONSTEXPR20 const size_t size = N;
		using value_type = T;

	private:
		T _values[N] = {};

#pragma region PRIVATE_METHODS
#if _MSTD_HAS_CXX20
		template<arithmetic... Ts, size_t... Idxs>
#else
		template<class... Ts, size_t... Idxs>
#endif
		_MSTD_CONSTEXPR20 void _set_values(const std::index_sequence<Idxs...>&, const Ts&... values) {
			((_values[Idxs] = static_cast<T>(values)), ...);
		}

		_MSTD_CONSTEXPR20 void _fill_values(const T& value) {
			std::fill_n(&_values[0], N, value);
		}

		_MSTD_CONSTEXPR20 void _fill_values_from(size_t first_idx, const T& value) {
			if (first_idx >= N) return;
			std::fill_n(&_values[0] + first_idx, N - first_idx, value);
		}

#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT*& values, const size_t& size) {
			if _MSTD_CONSTEXPR17 (std::is_same_v<OT, T>) {
				std::memcpy(&_values[0], values, std::min(N, size) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, size); ++i) {
					_values[i] = static_cast<T>(values[i]);
				}
			}
		}

#if _MSTD_HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const OT(&values)[TN]) {
			if _MSTD_CONSTEXPR17 (std::is_same_v<T, OT>) {
				std::memcpy(&_values[0], &values[0], std::min(N, TN) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, TN); ++i) {
					_values[i] = static_cast<T>(values[i]);
				}
			}
		}

#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
		requires (ON > 0)
#else
		template<size_t ON, class OT>
#endif
		_MSTD_CONSTEXPR20 void _copy_values_from(const vec<ON, OT>& other) {
			if _MSTD_CONSTEXPR17 (std::is_same_v<OT, T>) {
				std::memcpy(&_values[0], static_cast<const T*>(other), std::min(N, ON) * sizeof(T));
			}
			else {
				for (size_t i = 0; i != std::min(N, ON); ++i) {
					_values[i] = static_cast<T>(other[i]);
				}
			}
		}
#pragma endregion // PRIVATE_METHODS

	public:
#pragma region CONSTRUCTORS
		// vecN()
		_MSTD_CONSTEXPR20 vec() {
			_fill_values(T(0));
		}

		// vecN(x, y, ...)
#if _MSTD_HAS_CXX20
		template<arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= N)
#else
		template<class... Ts,
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N &&
				are_all_v<std::is_arithmetic, Ts...>), bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec(const Ts&... values) {
			_set_values<Ts...>(std::index_sequence_for<Ts...>(), values...);
			_fill_values_from(sizeof...(Ts), 0);
		}

		// vecN(vec, z, ...)
#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT, arithmetic... Ts>
		requires (sizeof...(Ts) > 0 && sizeof...(Ts) <= N - ON && ON < N)
#else
		template<size_t ON, class OT, class... Ts,
			std::enable_if_t<(sizeof...(Ts) > 0 && sizeof...(Ts) <= N - ON && ON < N &&
				are_all_v<std::is_arithmetic, OT, Ts...>), bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec(const vec<ON, OT>& other, const Ts&... values) {
			_copy_values_from(other);
			_set_values<Ts...>(make_index_sequence_for_from<ON, Ts...>(), values...);
			_fill_values_from(sizeof...(Ts) + ON, 0);
		}

		// vecN({ 1, 2 })
#if _MSTD_HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec(const OT (&values)[TN]) {
			_copy_values_from(values);
			_fill_values_from(TN, 0);
		}

		// vecN(&table)
#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec(const OT* values, const size_t& size) {
			_copy_values_from(values, size);
			_fill_values_from(size, 0);
		}

		// vecN(vecON)
#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT>
#endif
		_MSTD_CONSTEXPR20 vec(const vec<ON, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(ON, 0);
		}

#pragma region VECTOR_3_CONSTRUCTORS
#if _MSTD_HAS_CXX20
		template<arithmetic AT, arithmetic BT, size_t ON>
		requires (ON == 3)
#else
		template<class AT, class BT, size_t ON, std::enable_if_t<(ON == 3), bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec(const vec<ON, AT>& other_a, const vec<ON, BT>& other_b) : vec(other_a.cross(other_b)) {}
#pragma endregion // VECTOR_3_CONSTRUCTORS
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		_MSTD_CONSTEXPR20 ~vec() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _MSTD_HAS_CXX20
		template<size_t TN, arithmetic OT>
#else
		template<size_t TN, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec<N, T>& operator=(const OT(&values)[TN]) {
			_copy_values_from(values);
			_fill_values_from(TN, 0);
			return *this;
		}
#if _MSTD_HAS_CXX20
		template<size_t ON, arithmetic OT>
#else
		template<size_t ON, class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 vec<N, T>& operator=(const vec<ON, OT>& other) {
			_copy_values_from(other);
			_fill_values_from(ON, 0);
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED
		static _MSTD_CONSTEXPR20 vec<N, T> zero() {
			return vec<N, T>();
		}
		static _MSTD_CONSTEXPR20 vec<N, T> one() {
			return fill(static_cast<T>(1));
		}
		static _MSTD_CONSTEXPR20 vec<N, T> fill(const T& value) {
			vec<N, T> res;
			res._fill_values(value);
			return res;
		}
#pragma endregion // PREDEFINED

#pragma region PREDEFINED_CHECKS
		_MSTD_CONSTEXPR20 bool is_zero() const {
			return is_filled_with(static_cast<T>(0));
		}

		_MSTD_CONSTEXPR20 bool is_one() const {
			return is_filled_with(static_cast<T>(1));
		}

		_MSTD_CONSTEXPR20 bool is_filled_with(const T& value) const {
			for (size_t i = 0; i != N; ++i) {
				if (_values[i] != value) {
					return false;
				}
			}
			return true;
		}

		_MSTD_CONSTEXPR20 bool is_normalized() const {
			return length() == static_cast<T>(1);
		}
#pragma endregion // PREDEFINED_CHECKS

#pragma region VECTOR_GETTERS
		_MSTD_CONSTEXPR20 T& x() {
			return _values[0];
		}
		_MSTD_CONSTEXPR20 T x() const {
			return _values[0];
		}

		_MSTD_CONSTEXPR20 T& r() {
			return _values[0];
		}
		_MSTD_CONSTEXPR20 T r() const {
			return _values[0];
		}

		_MSTD_ENABLE_IF_TEMPLATE(N > 1)
		_MSTD_CONSTEXPR20 T& y() _MSTD_REQUIRES(N > 1) {
			return _values[1];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 1)
		_MSTD_CONSTEXPR20 T y() const _MSTD_REQUIRES(N > 1) {
			return _values[1];
		}

		_MSTD_ENABLE_IF_TEMPLATE(N > 1)
		_MSTD_CONSTEXPR20 T& g() _MSTD_REQUIRES(N > 1) {
			return _values[1];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 1)
		_MSTD_CONSTEXPR20 T g() const _MSTD_REQUIRES(N > 1) {
			return _values[1];
		}
		
		_MSTD_ENABLE_IF_TEMPLATE(N > 2)
		_MSTD_CONSTEXPR20 T& z() _MSTD_REQUIRES(N > 2) {
			return _values[2];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 2)
		_MSTD_CONSTEXPR20 T z() const _MSTD_REQUIRES(N > 2) {
			return _values[2];
		}

		_MSTD_ENABLE_IF_TEMPLATE(N > 2)
		_MSTD_CONSTEXPR20 T& b() _MSTD_REQUIRES(N > 2) {
			return _values[2];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 2)
		_MSTD_CONSTEXPR20 T b() const _MSTD_REQUIRES(N > 2) {
			return _values[2];
		}

		_MSTD_ENABLE_IF_TEMPLATE(N > 3)
		_MSTD_CONSTEXPR20 T& w() _MSTD_REQUIRES(N > 3) {
			return _values[3];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 3)
		_MSTD_CONSTEXPR20 T w() const _MSTD_REQUIRES(N > 3) {
			return _values[3];
		}

		_MSTD_ENABLE_IF_TEMPLATE(N > 3)
		_MSTD_CONSTEXPR20 T& a() _MSTD_REQUIRES(N > 3) {
			return _values[3];
		}
		_MSTD_ENABLE_IF_TEMPLATE(N > 3)
		_MSTD_CONSTEXPR20 T a() const _MSTD_REQUIRES(N > 3) {
			return _values[3];
		}
#pragma endregion // VECTOR_GETTERS

#pragma region VECTOR_OPERATIONS
		_MSTD_CONSTEXPR20 T length() const {
			T value = static_cast<T>(0);
			for (size_t i = 0; i != N; ++i) {
				value += _values[i] * _values[i];
			}
			return static_cast<T>(std::sqrt(value));
		}

		_MSTD_CONSTEXPR20 vec<N, T>& normalize() {
			T len = length();
			if (len == static_cast<T>(0)) {
				return *this;
			}
			*this /= len;
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T> normalized() const {
			vec<N, T> res = *this;
			return res.normalize();
		}

		_MSTD_CONSTEXPR20 T dot(const vec<N, T>& other) const {
			T res = static_cast<T>(0);
			for (size_t i = 0; i != N; ++i) {
				res += _values[i] * other[i];
			}
			return res;
		}

		_MSTD_CONSTEXPR20 T angle_between(const vec<N, T>& other) const {
			T this_len = length();
			if (this_len == static_cast<T>(0)) {
				return static_cast<T>(0);
			}

			T other_len = other.length();
			if (other_len == static_cast<T>(0)) {
				return static_cast<T>(0);
			}

			return static_cast<T>(std::acos(dot(other) / (this_len * other_len)));
		}

		_MSTD_CONSTEXPR20 vec<N, T>& reflect(const vec<N, T>& normal) noexcept {
			*this -= 2.0f * this->dot(normal) * normal;
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> reflected(const vec<N, T>& normal) const noexcept {
			vec<N, T> res = *this;
			return res.reflect(normal);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& refract(const vec<N, T>& normal, const T& eta) {
			*this = this->refracted(normal, eta);
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> refracted(const vec<N, T>& normal, const T& eta) const {
			float cos_theta = std::min((-(*this)).dot(normal), 1.0f);
			vec<N, T> r_out_perp = eta * (*this + cos_theta * normal);
			float length = r_out_perp.length();
			vec<N, T> r_out_parallel = -std::sqrt(std::abs(1.0f - length * length)) * normal;
			return r_out_perp + r_out_parallel;
		}

		_MSTD_CONSTEXPR20 vec<N, T>& saturate() noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = ::mstd::saturate(_values[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> saturated() const noexcept {
			vec<N, T> res = *this;
			return res.saturate();
		}

		_MSTD_CONSTEXPR20 vec<N, T>& fract() noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = ::mstd::fract(_values[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> fracted() const noexcept {
			vec<N, T> res = *this;
			return res.fract();
		}

		_MSTD_CONSTEXPR20 vec<N, T>& mod(const T& y) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= y * std::floor(_values[i] / y);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> modded(const T& y) const {
			vec<N, T> res = *this;
			return res.mod(y);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& mod(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other[i] * std::floor(_values[i] / other[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> modded(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			return res.mod(other);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& pow(const T& y) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::pow(_values[i], y);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> powed(const T& y) const {
			vec<N, T> res = *this;
			return res.pow(y);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& pow(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::pow(_values[i], other[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> powed(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			return res.pow(other);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& clamp(const T& min_val, const T& max_val) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::clamp(_values[i], min_val, max_val);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> clampped(const T& min_val, const T& max_val) const {
			vec<N, T> res = *this;
			return res.clamp(min_val, max_val);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& clamp(const vec<N, T>& min_val, const vec<N, T>& max_val) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = std::clamp(_values[i], min_val[i], max_val[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> clampped(const vec<N, T>& min_val, const vec<N, T>& max_val) const {
			vec<N, T> res = *this;
			return res.clamp(min_val, max_val);
		}

		_MSTD_CONSTEXPR20 vec<N, T>& step(const T& edge) noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = ::mstd::step(edge, _values[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T>& step(const vec<N, T>& edge) noexcept {
			for (size_t i = 0; i != N; ++i) {
				_values[i] = ::mstd::step(edge[i], _values[i]);
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> stepped(const T& edge) const noexcept {
			vec<N, T> res = *this;
			return res.step(edge);
		}

		_MSTD_CONSTEXPR20 vec<N, T> stepped(const vec<N, T>& edge) const noexcept {
			vec<N, T> res = *this;
			return res.step(edge);
		}

#pragma region VECTOR_3_OPERATIONS
		_MSTD_ENABLE_IF_TEMPLATE(N == 3)
		_MSTD_CONSTEXPR20 vec<N, T> cross(const vec<N, T>& other) const _MSTD_REQUIRES(N == 3) {
			return vec<N, T>(
				_values[1] * other[2] - _values[2] * other[1],
				_values[2] * other[0] - _values[0] * other[2],
				_values[0] * other[1] - _values[1] * other[0]
			);
		}

		_MSTD_ENABLE_IF_TEMPLATE(N == 3)
		_MSTD_CONSTEXPR20 vec<N, T>& rotate(const vec<N, T>& axis, const T& radians) _MSTD_REQUIRES(N == 3) {
			const quat<T> p(T(0), (*this));

			vec<N, T> norm_axis = axis;
			if (!norm_axis.is_zero()) norm_axis.normalize();

			const quat<T>& q = quat<T>::rotation(norm_axis, radians);

			const quat<T>& invers_q = q.inverted();

			*this = (q * p * invers_q).v;
			return *this;
		}
		_MSTD_ENABLE_IF_TEMPLATE(N == 3)
		_MSTD_CONSTEXPR20 vec<N, T> rotated(const vec<N, T>& axis, const T& radians) _MSTD_REQUIRES(N == 3) {
			vec<N, T> res = *this;
			return res.rotate(axis, radians);
		}
#pragma endregion // VECTOR_3_OPERATIONS
#pragma endregion // VECTOR_OPERTATIONS

#pragma region OPERATORS
		_MSTD_CONSTEXPR20 vec<N, T>& operator+=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] += other[i];
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator-=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other[i];
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator*=(const vec<N, T>& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] *= other[i];
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator/=(const vec<N, T>& other) {
			if (other == vec<N, T>::zero()) {
				return *this;
			}
			for (size_t i = 0; i != N; ++i) {
				_values[i] /= other[i];
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T>& operator+=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] += other;
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator-=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] -= other;
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator*=(const T& other) {
			for (size_t i = 0; i != N; ++i) {
				_values[i] *= other;
			}
			return *this;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator/=(const T& other) {
			if (other == static_cast<T>(0)) {
				return *this;
			}
			for (size_t i = 0; i != N; ++i) {
				_values[i] /= other;
			}
			return *this;
		}

		_MSTD_CONSTEXPR20 vec<N, T> operator+(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res += other;
			return res;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator-(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res -= other;
			return res;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator*(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res *= other;
			return res;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator/(const vec<N, T>& other) const {
			vec<N, T> res = *this;
			res /= other;
			return res;
		}

		_MSTD_CONSTEXPR20 vec<N, T> operator+(const T& other) const {
			vec<N, T> res = *this;
			res += other;
			return res;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator-(const T& other) const {
			vec<N, T> res = *this;
			res -= other;
			return res;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator*(const T& other) const {
			vec<N, T> res = *this;
			res *= other;
			return res;
		}
		friend _MSTD_CONSTEXPR20 vec<N, T> operator*(const T& other, const vec<N, T>& vector) {
			return vector * other;
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator/(const T& other) const {
			vec<N, T> res = *this;
			res /= other;
			return res;
		}

		_MSTD_CONSTEXPR20 vec<N, T> operator+() const {
			return vec<N, T>(*this);
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator-() const {
			return *this * -1;
		}

		_MSTD_CONSTEXPR20 vec<N, T>& operator++() {
			return *this += vec<N, T>::one();
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator++(int) {
			vec<N, T> old = *this;
			operator++();
			return old;
		}
		_MSTD_CONSTEXPR20 vec<N, T>& operator--() {
			return *this -= vec<N, T>::one();
		}
		_MSTD_CONSTEXPR20 vec<N, T> operator--(int) {
			vec<N, T> old = *this;
			operator--();
			return old;
		}

		template<size_t ON>
		_MSTD_CONSTEXPR20 bool operator==(const vec<ON, T>& other) const {
			if constexpr (N != ON) {
				return false;
			}
			else {
				return std::memcmp(_values, static_cast<const T*>(other), N * sizeof(T)) == 0;
			}
		}
		template<size_t ON>
		_MSTD_CONSTEXPR20 bool operator!=(const vec<ON, T>& other) const {
			return !(*this == other);
		}

		_MSTD_CONSTEXPR20 operator const T* () const {
			return _values;
		}

		_MSTD_CONSTEXPR20 T& operator[](const size_t& idx) {
			return _values[idx];
		}
		_MSTD_CONSTEXPR20 T operator[](const size_t& idx) const {
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
	static _MSTD_CONSTEXPR20 T length(const vec<N, T>& a) noexcept {
		return a.length();
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> normalize(const vec<N, T>& a) noexcept {
		return a.normalized();
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> max(const vec<N, T>& a, const vec<N, T>& b) noexcept {
		vec<N, T> res;
		for (size_t i = 0; i != N; ++i) {
			res[i] = std::max(a[i], b[i]);
		}
		return res;
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> min(const vec<N, T>& a, const vec<N, T>& b) noexcept {
		vec<N, T> res;
		for (size_t i = 0; i != N; ++i) {
			res[i] = std::min(a[i], b[i]);
		}
		return res;
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 T dot(const vec<N, T>& a, const vec<N, T>& b) {
		return a.dot(b);
	}

#if _MSTD_HAS_CXX20
	template<class T, size_t N>
	requires (N == 3)
#else
	template<class T, size_t N, std::enable_if_t<(N == 3), bool> = true>
#endif
	static _MSTD_CONSTEXPR20 vec<N, T> cross(const vec<N, T>& a, const vec<N, T>& b) {
		return a.cross(b);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 T angle_between(const vec<N, T>& a, const vec<N, T>& b) {
		return a.angle_between(b);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> reflect(const vec<N, T>& dir, const vec<N, T>& normal) {
		return dir.reflected(normal);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> refract(const vec<N, T>& dir, const vec<N, T>& normal, const T& eta) {
		return dir.refracted(normal, eta);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> saturate(const vec<N, T>& a) {
		return a.saturated();
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> fract(const vec<N, T>& a) {
		return a.fracted();
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> mod(const vec<N, T>& a, const T& y) {
		return a.modded(y);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> mod(const vec<N, T>& a, const vec<N, T>& b) {
		return a.modded(b);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> pow(const vec<N, T>& a, const T& y) {
		return a.powed(y);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> pow(const vec<N, T>& a, const vec<N, T>& b) {
		return a.powed(b);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> clamp(const vec<N, T>& a, const T& min_val, const T& max_val) {
		return a.clampped(min_val, max_val);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> clamp(const vec<N, T>& a, const vec<N, T>& min_val, const vec<N, T>& max_val) {
		return a.clampped(min_val, max_val);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> step(const T& edge, const vec<N, T>& a) {
		return a.stepped(edge);
	}

	template<class T, size_t N>
	static _MSTD_CONSTEXPR20 vec<N, T> step(const vec<N, T>& edge, const vec<N, T>& a) {
		return a.stepped(edge);
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
