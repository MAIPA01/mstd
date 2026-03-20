/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 *
 * Tested and fixed by Muppetsg2 (https://github.com/Muppetsg2)
 * Good reference site: https://www.tobynorris.com/work/prog/csharp/quatview/help/orientations_and_quaternions.htm
 */

#pragma once
#ifndef _MSTD_QUAT_HPP_
#define _MSTD_QUAT_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/vec.hpp>

namespace mstd {
#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
#endif
	class quat {
	public:
		using value_type = T;
		using vec_type = vec<3, T>;

		T s;
		vec_type v;

#pragma region CONSTRUCTORS
		_MSTD_CONSTEXPR20 quat() : s(0), v() {}
		_MSTD_CONSTEXPR20 quat(const T& scalar, const vec_type& vector) : s(scalar), v(vector) {}
		_MSTD_CONSTEXPR20 quat(const T& scalar, const T& x, const T& y, const T& z) : s(scalar), v(x, y, z) {}
#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 quat(const quat<OT>& other) : s(other.s), v(other.v) {}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		_MSTD_CONSTEXPR20 ~quat() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _MSTD_HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 quat<T>& operator=(const quat<OT>& other) {
			s = static_cast<T>(other.s);
			v = other.v;
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED_QUATERNIONS
		static _MSTD_CONSTEXPR20 quat<T> rotation(const vec_type& axis, const T& radians) {
			_MSTD_CONSTEXPR17 const double half = 0.5;

			quat<T> q;
			if (!axis.is_zero()) {
				q = quat<T>(static_cast<T>(std::cos(radians * half)), axis.normalized() * static_cast<T>(std::sin(radians * half)));
			}
			else {
				q = quat<T>(static_cast<T>(std::cos(radians * half)), axis);
			}
			if (q.magnitude() != static_cast<T>(0)) q.normalize();
			return q;
		}

		static _MSTD_CONSTEXPR20 quat<T> from_euler_angels(const vec_type& eulerAngels) {
			return from_radians({ deg_to_rad(eulerAngels[0]),
				deg_to_rad(eulerAngels[1]), deg_to_rad(eulerAngels[2]) });
		}

		static _MSTD_CONSTEXPR20 quat<T> from_radians(const vec_type& radians) {
			quat<T> qx = rotation(vec_type(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)), radians[0]);
			quat<T> qy = rotation(vec_type(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0)), radians[1]);
			quat<T> qz = rotation(vec_type(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1)), radians[2]);

			// ZYX convention
			quat<T> q = qz * qy * qx;
			if (q.magnitude() != static_cast<T>(0)) q.normalize();
			return q;
		}
#pragma endregion // PREDEFINED_QUATERNIONS

#pragma region QUATERNION_OPERATIONS
		_MSTD_CONSTEXPR20 T magnitude() const {
			return std::sqrt((s * s) + v.dot(v));
		}

		_MSTD_CONSTEXPR20 quat<T>& normalize() {
			T m = magnitude();
			*this /= m;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T> normalized() const {
			quat<T> res = *this;
			return res.normalize();
		}

		_MSTD_CONSTEXPR20 quat<T>& conjugate() {
			v *= -1;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T> conjugated() const {
			quat<T> res = *this;
			return res.conjugate();
		}

		_MSTD_CONSTEXPR20 quat<T>& invert() {
			T magnitudes = magnitude();
			magnitudes *= magnitudes;
			magnitudes = static_cast<T>(1.0 / magnitudes);

			conjugate();

			s *= magnitudes;
			v *= magnitudes;

			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T> inverted() const {
			quat<T> res = *this;
			return res.invert();
		}

		_MSTD_CONSTEXPR20 vec_type to_radians() const {
			_MSTD_CONSTEXPR17 const double two = 2.0;
			_MSTD_CONSTEXPR17 const double half = 0.5;

			vec_type res;
			quat<T> q = *this;

			if (q.magnitude() != static_cast<T>(0)) q.normalize();

			// roll (x-axis rotation)
			T sinxCosp = static_cast<T>(two * ((q.s * q.v[0]) + (q.v[1] * q.v[2])));
			T cosxCosp = static_cast<T>(1.0 - (two * ((q.v[0] * q.v[0]) + (q.v[1] * q.v[1]))));
			res[0] = static_cast<T>(std::atan2(sinxCosp, cosxCosp));

			// pitch (y-axis rotation)
			T siny = static_cast<T>(std::sqrt(1.0 + (two * ((q.s * q.v[1]) - (q.v[0] * q.v[2])))));
			T cosy = static_cast<T>(std::sqrt(1.0 - (two * ((q.s * q.v[1]) - (q.v[0] * q.v[2])))));
			res[1] = static_cast<T>((two * std::atan2(siny, cosy)) - (M_PI * half));

			// yaw (z-axis rotation)
			T sinzCosp = static_cast<T>(two * ((q.s * q.v[2]) + (q.v[0] * q.v[1])));
			T coszCosp = static_cast<T>(1.0 - (two * ((q.v[1] * q.v[1]) + (q.v[2] * q.v[2]))));
			res[2] = static_cast<T>(std::atan2(sinzCosp, coszCosp));

			return res;
		}

		_MSTD_CONSTEXPR20 vec_type to_euler_angles() const {
			vec_type res = to_radians();
			res[0] = rad_to_deg(res[0]);
			res[1] = rad_to_deg(res[1]);
			res[2] = rad_to_deg(res[2]);
			return res;
		}

		_MSTD_CONSTEXPR20 T scalar(const quat<T>& other) {
			return (s * other.s) + v.dot(other.v);
		}
#pragma endregion // QUATERNION_OPERATIONS

#pragma region OPERATORS
		_MSTD_CONSTEXPR20 quat<T>& operator+=(const quat<T>& other) {
			s += other.s;
			v += other.v;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator-=(const quat<T>& other) {
			s -= other.s;
			v -= other.v;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator*=(const quat<T>& other) {
			T t = s;
			s = (s * other.s) - v.dot(other.v);
			v = (other.v * t) + (v * other.s) + v.cross(other.v);
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator*=(const vec_type& other) {
			quat<T> p(static_cast<T>(0), other);
			*this = p;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator*=(const T& other) {
			s *= other;
			v *= other;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator/=(const quat<T>& other) {
			*this *= other.inverted();
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator/=(const T& other) {
			if (other == static_cast<T>(0)) {
				return *this;
			}
			s /= other;
			v /= other;
			return *this;
		}

		_MSTD_CONSTEXPR20 quat<T> operator+(const quat<T>& other) const {
			quat<T> res = *this;
			return res += other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator-(const quat<T>& other) const {
			quat<T> res = *this;
			return res -= other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator*(const quat<T>& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator*(const vec_type& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		friend _MSTD_CONSTEXPR20 quat<T> operator*(const vec_type& other, const quat<T>& quaternion) {
			return quaternion * other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator*(const T& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		friend _MSTD_CONSTEXPR20 quat<T> operator*(const T& other, const quat<T>& quaternion) {
			return quaternion * other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator/(const quat<T>& other) const {
			quat<T> res = *this;
			return res /= other;
		}
		_MSTD_CONSTEXPR20 quat<T> operator/(const T& other) const {
			quat<T> res = *this;
			return res /= other;
		}

		_MSTD_CONSTEXPR20 quat<T> operator-() const {
			return *this * -1;
		}
		_MSTD_CONSTEXPR20 quat<T> operator+() const {
			return quat<T>(*this);
		}
		_MSTD_CONSTEXPR20 quat<T>& operator--() {
			s -= 1;
			--v;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T> operator--(int) {
			quat<T> old = *this;
			operator--();
			return old;
		}
		_MSTD_CONSTEXPR20 quat<T>& operator++() {
			s += 1;
			++v;
			return *this;
		}
		_MSTD_CONSTEXPR20 quat<T> operator++(int) {
			quat<T> old = *this;
			operator++();
			return old;
		}

		_MSTD_CONSTEXPR20 bool operator==(const quat<T>& other) const {
			return s == other.s && v == other.v;
		}
		_MSTD_CONSTEXPR20 bool operator!=(const quat<T>& other) const {
			return s != other.s || v != other.v;
		}

		friend std::ostream& operator<<(std::ostream& str, const quat<T>& quaternion) {
			return str << "(" << std::to_string(quaternion.s) << ", " << quaternion.v << ")";
		}
#pragma endregion // OPERATORS
	};
}
#endif
#endif