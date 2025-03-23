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
#include "vec.hpp"

namespace mstd {
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
	class quat {
	public:
		using value_type = T;

		T s;
		vec<3, T> v;

#pragma region CONSTRUCTORS
		quat() : s(0), v() {}
		quat(const T& scalar, const vec<3, T>& vector) : s(scalar), v(vector) {}
		quat(const T& scalar, const T& x, const T& y, const T& z) : s(scalar), v(x, y, z) {}
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		quat(const quat<OT>& other) : s(other.s), v(other.v) {}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~quat() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
		quat<T>& operator=(const quat<OT>& other) {
			s = other.s;
			v = other.v;
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region QUATERNION_OPERATIONS
		T magnitude() const {
			return std::sqrt(s * s + v.dot(v));
		}

		quat<T>& normalize() {
			T m = magnitude();
			*this /= m;
			return *this;
		}
		quat<T> normalized() const {
			quat<T> res = *this;
			return res.normalize();
		}

		quat<T>& to_rotation_quaternion() {
			T angle = s;
			s = std::cos(angle * 0.5);
			v = v.normalized() * std::sin(angle * 0.5);
			return *this;
		}
		quat<T> as_rotation_quaternion() const {
			quat<T> res = *this;
			return res.to_rotation_quaternion();
		}

		quat<T>& conjugate() {
			v *= -1;
			return *this;
		}
		quat<T> conjugated() const {
			quat<T> res = *this;
			return res.conjugate();
		}

		quat<T>& invert() {
			T magnitudes = magnitude();
			magnitudes *= magnitudes;
			magnitudes = 1.0 / magnitudes;

			conjugate();

			s *= magnitudes;
			v *= magnitudes;

			return *this;
		}
		quat<T> inverted() const {
			quat<T> res = *this;
			return res.invert();
		}

		vec<3, T> to_euler_angles() const {
			vec<3, T> res;
			quat<T> q = *this;

			if (q.magnitude() != T(0)) q.normalize();

			// roll (x-axis rotation)
			T sinx_cosp = 2.0 * (q.s * q.v[0] + q.v[1] * q.v[2]);
			T cosx_cosp = 1.0 - 2.0 * (q.v[0] * q.v[0] + q.v[1] * q.v[1]);
			res[0] = std::atan2(sinx_cosp, cosx_cosp);

			// pitch (y-axis rotation)
			T siny = std::sqrt(1.0 + 2.0 * (q.s * q.v[1] - q.v[0] * q.v[2]));
			T cosy = std::sqrt(1.0 - 2.0 * (q.s * q.v[1] - q.v[0] * q.v[2]));
			res[1] = 2.0 * std::atan2(siny, cosy) - M_PI / 2.0;

			// yaw (z-axis rotation)
			T sinz_cosp = 2.0 * (q.s * q.v[2] + q.v[0] * q.v[1]);
			T cosz_cosp = 1.0 - 2.0 * (q.v[1] * q.v[1] + q.v[2] * q.v[2]);
			res[2] = std::atan2(sinz_cosp, cosz_cosp);

			return res;
		}

		T scalar(const quat<T>& other) {
			return s * other.s + v.dot(other.v);
		}
#pragma endregion // QUATERNION_OPERATIONS

#pragma region OPERATORS
		quat<T>& operator+=(const quat<T>& other) {
			s += other.s;
			v += other.v;
			return *this;
		}
		quat<T>& operator-=(const quat<T>& other) {
			s -= other.s;
			v -= other.v;
			return *this;
		}
		quat<T>& operator*=(const quat<T>& other) {
			T t = s;
			s = s * other.s - v.dot(other.v);
			v = other.v * t + v * other.s + v.cross(other.v);
			return *this;
		}
		quat<T>& operator*=(const T& other) {
			s *= other;
			v *= other;
			return *this;
		}
		quat<T>& operator/=(const quat<T>& other) {
			*this *= other.inverted();
			return *this;
		}
		quat<T>& operator/=(const T& other) {
			if (other == T(0)) {
				throw std::runtime_error("division by zero");
			}
			s /= other;
			v /= other;
			return *this;
		}

		quat<T> operator+(const quat<T>& other) const {
			quat<T> res = *this;
			return res += other;
		}
		quat<T> operator-(const quat<T>& other) const {
			quat<T> res = *this;
			return res -= other;
		}
		quat<T> operator*(const quat<T>& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		quat<T> operator*(const T& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		quat<T> operator/(const quat<T>& other) const {
			quat<T> res = *this;
			return res /= other;
		}
		quat<T> operator/(const T& other) const {
			quat<T> res = *this;
			return res /= other;
		}

		quat<T> operator-() const {
			return *this * -1;
		}
		quat<T> operator+() const {
			return quat<T>(*this);
		}
		quat<T>& operator--() {
			s -= 1;
			v--;
			return *this;
		}
		quat<T>& operator++() {
			s += 1;
			v++;
			return *this;
		}

		bool operator==(const quat<T>& other) const {
			return s == other.s && v == other.v;
		}

		bool operator!=(const quat<T>& other) const {
			return s != other.s || v != other.v;
		}

		friend static std::ostream& operator<<(std::ostream& str, const quat<T>& quaternion) {
			return str << "(" << std::to_string(quaternion.s) << ", " << quaternion.v << ")";
		}
#pragma endregion // OPERATORS
	};

#pragma region PREDEFINED_TYPES
	using fquat = quat<float>;
#pragma endregion // PREDEFINED_TYPES
}
