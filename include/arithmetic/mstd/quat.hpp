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
#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
#endif
	class quat {
	public:
		using value_type = T;
		using vec_type = vec<3ull, T>;

		T s;
		vec_type v;

#pragma region CONSTRUCTORS
		quat() : s(0), v() {}
		quat(const T& scalar, const vec_type& vector) : s(scalar), v(vector) {}
		quat(const T& scalar, const T& x, const T& y, const T& z) : s(scalar), v(x, y, z) {}
#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		quat(const quat<OT>& other) : s(other.s), v(other.v) {}
#pragma endregion // CONSTRUCTORS

#pragma region DESTRUCTOR
		virtual ~quat() = default;
#pragma endregion // DESTRUCTOR

#pragma region ASSIGN
#if _HAS_CXX20
		template<arithmetic OT>
#else
		template<class OT, std::enable_if_t<std::is_arithmetic_v<OT>, bool> = true>
#endif
		quat<T>& operator=(const quat<OT>& other) {
			s = (T)other.s;
			v = other.v;
			return *this;
		}
#pragma endregion // ASSIGN

#pragma region PREDEFINED_QUATERNIONS
		static quat<T> rotation(const vec_type& axis, const T& radians) {
			quat<T> q;
			if (!axis.is_zero()) {
				q = quat<T>((T)std::cos(radians * 0.5), axis.normalized() * (T)std::sin(radians * 0.5));
			}
			else {
				q = quat<T>((T)std::cos(radians * 0.5), axis);
			}
			if (q.magnitude() != T(0)) q.normalize();
			return q;
		}

		static quat<T> from_euler_angels(const vec_type& euler_angels) {
			return from_radians({ deg_to_rad(euler_angels[0]), deg_to_rad(euler_angels[1]), deg_to_rad(euler_angels[2]) });
		}

		static quat<T> from_radians(const vec_type& radians) {
			quat<T> qx = rotation(vec_type(T(1), T(0), T(0)), radians[0]);
			quat<T> qy = rotation(vec_type(T(0), T(1), T(0)), radians[1]);
			quat<T> qz = rotation(vec_type(T(0), T(0), T(1)), radians[2]);

			// ZYX convention
			quat<T> q = qz * qy * qx;
			if (q.magnitude() != T(0)) q.normalize();
			return q;
		}
#pragma endregion // PREDEFINED_QUATERNIONS

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
			magnitudes = (T)(1.0 / magnitudes);

			conjugate();

			s *= magnitudes;
			v *= magnitudes;

			return *this;
		}
		quat<T> inverted() const {
			quat<T> res = *this;
			return res.invert();
		}

		vec_type to_radians() const {
			vec_type res;
			quat<T> q = *this;

			if (q.magnitude() != T(0)) q.normalize();

			// roll (x-axis rotation)
			T sinx_cosp = (T)(2.0 * (q.s * q.v[0] + q.v[1] * q.v[2]));
			T cosx_cosp = (T)(1.0 - 2.0 * (q.v[0] * q.v[0] + q.v[1] * q.v[1]));
			res[0] = (T)std::atan2(sinx_cosp, cosx_cosp);

			// pitch (y-axis rotation)
			T siny = (T)std::sqrt(1.0 + 2.0 * (q.s * q.v[1] - q.v[0] * q.v[2]));
			T cosy = (T)std::sqrt(1.0 - 2.0 * (q.s * q.v[1] - q.v[0] * q.v[2]));
			res[1] = (T)(2.0 * std::atan2(siny, cosy) - M_PI / 2.0);

			// yaw (z-axis rotation)
			T sinz_cosp = (T)(2.0 * (q.s * q.v[2] + q.v[0] * q.v[1]));
			T cosz_cosp = (T)(1.0 - 2.0 * (q.v[1] * q.v[1] + q.v[2] * q.v[2]));
			res[2] = (T)std::atan2(sinz_cosp, cosz_cosp);

			return res;
		}

		vec_type to_euler_angles() const {
			vec_type res = to_radians();
			res[0] = rad_to_deg(res[0]);
			res[1] = rad_to_deg(res[1]);
			res[2] = rad_to_deg(res[2]);
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
		quat<T>& operator*=(const vec_type& other) {
			quat<T> p(T(0), other);
			*this = p;
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
		quat<T> operator*(const vec_type& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		friend quat<T> operator*(const vec_type& other, const quat<T>& quaternion) {
			return quaternion * other;
		}
		quat<T> operator*(const T& other) const {
			quat<T> res = *this;
			return res *= other;
		}
		friend quat<T> operator*(const T& other, const quat<T>& quaternion) {
			return quaternion * other;
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

		friend std::ostream& operator<<(std::ostream& str, const quat<T>& quaternion) {
			return str << "(" << std::to_string(quaternion.s) << ", " << quaternion.v << ")";
		}
#pragma endregion // OPERATORS
	};

#pragma region PREDEFINED_TYPES
	using fquat = quat<float>;
	using dquat = quat<double>;
	using ldquat = quat<long double>;
	using iquat = quat<int>;
	using uquat = quat<unsigned int>;
	using bquat = quat<bool>;
	using cquat = quat<char>;
	using ucquat = quat<unsigned char>;
	using scquat = quat<signed char>;
	using lquat = quat<long>;
	using ulquat = quat<unsigned long>;
	using llquat = quat<long long>;
	using ullquat = quat<unsigned long long>;
#pragma endregion // PREDEFINED_TYPES
}
