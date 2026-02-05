#pragma once
#include "arithmetic_types.hpp"

namespace mstd {
#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T signum(const T& x) noexcept {
		if _MSTD_CONSTEXPR17 (std::is_signed_v<T>) {
			return static_cast<T>((static_cast<T>(0) < x) - (x < static_cast<T>(0)));
		}
		else {
			return static_cast<T>(0) != x;
		}
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T step(const T& edge, const T& x) noexcept {
		return x < edge ? T(0) : T(1);
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T remap(const T& input, const T& currStart, const T& currEnd, const T& expectedStart, const T& expectedEnd) noexcept {
		return expectedStart + ((expectedEnd - expectedStart) / (currEnd - currStart)) * (input - currStart);
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T deg_to_rad(const T& angle) noexcept {
		return angle * (static_cast<T>(M_PI) / static_cast<T>(180));
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T rad_to_deg(const T& rad) noexcept {
		return rad * (static_cast<T>(180) / static_cast<T>(M_PI));
	}

#if _MSTD_HAS_CXX20
	template<floating_point AT, floating_point BT, floating_point EpsT>
#else
	template<class AT, class BT, class EpsT, std::enable_if_t<mstd::are_floating_points_v<AT, BT, EpsT>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool epsilon_equal(const AT& a, const BT& b, const EpsT& epsilon) noexcept {
		return std::abs(a - b) < epsilon;
	}

#if _MSTD_HAS_CXX20
	template<arithmetic AT, arithmetic BT, floating_point EpsT = double>
#else
	template<class AT, class BT, class EpsT = double, 
		std::enable_if_t<mstd::are_arithmetic_v<AT, BT> && std::is_floating_point_v<EpsT>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool is_equal(const AT& a, const BT& b, const EpsT& eps = 1e-4) {
		if _MSTD_CONSTEXPR17(std::is_floating_point_v<AT> || std::is_floating_point_v<BT>) {
			return epsilon_equal(a, b, eps);
		}
		else {
			return a == b;
		}
	}

#if _MSTD_HAS_CXX20
	template<arithmetic AT, arithmetic BT, floating_point EpsT = double>
#else
	template<class AT, class BT, class EpsT = double,
		std::enable_if_t<mstd::are_arithmetic_v<AT, BT>&& std::is_floating_point_v<EpsT>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 bool is_not_equal(const AT& a, const BT& b, const EpsT& eps = 1e-4) {
		return !is_equal(a, b, eps);
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T saturate(const T& a) noexcept {
		return std::clamp(a, T(0), T(1));
	}

#if _MSTD_HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static _MSTD_CONSTEXPR20 T fract(const T& x) {
		if _MSTD_CONSTEXPR17(std::is_floating_point_v<T>) {
			return x - std::floor(x);
		}
		else {
			return 0;
		}
	}

	static _MSTD_CONSTEXPR20 float Q_rsqrt(float number) noexcept
	{
#if _MSTD_HAS_CXX20
		auto y = std::bit_cast<float>(
			0x5f3759df - (std::bit_cast<uint32_t>(number) >> 1));
#else
		auto yi = 0x5f3759df - ((*reinterpret_cast<uint32_t*>(&number)) >> 1);
		auto y = *reinterpret_cast<float*>(&yi);
#endif
		return y * (1.5f - (number * 0.5f * y * y));
	}
}