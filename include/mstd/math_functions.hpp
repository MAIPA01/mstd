#pragma once
#include "arithmetic_types.hpp"

namespace mstd {
#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T signum(const T& x) noexcept {
		if constexpr (std::is_signed_v<T>) {
			return (T(0) < x) - (x < T(0));
		}
		else {
			return T(0) != x;
		}
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T step(const T& edge, const T& x) noexcept {
		return x < edge ? T(0) : T(1);
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T remap(const T& input, const T& currStart, const T& currEnd, const T& expectedStart, const T& expectedEnd) noexcept {
		return expectedStart + ((expectedEnd - expectedStart) / (currEnd - currStart)) * (input - currStart);
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T deg_to_rad(const T& angle) noexcept {
		return angle * (static_cast<T>(M_PI) / static_cast<T>(180));
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T rad_to_deg(const T& rad) noexcept {
		return rad * (static_cast<T>(180) / static_cast<T>(M_PI));
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static bool epsilon_equal(const T& a, const T& b, const T& epsilon) noexcept {
		return std::abs(a - b) < epsilon;
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static constexpr T saturate(const T& a) noexcept {
		return std::clamp(a, T(0), T(1));
	}

#if _HAS_CXX20
	template<arithmetic T>
#else
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
#endif
	static T fract(const T& x) {
		if constexpr (std::is_floating_point_v<T>) {
			return x - std::floor(x);
		}
		else {
			return x;
		}
	}

	static constexpr float Q_rsqrt(float number) noexcept
	{
		const auto y = std::bit_cast<float>(
			0x5f3759df - (std::bit_cast<uint32_t>(number) >> 1));
		return y * (1.5f - (number * 0.5f * y * y));
	}
}