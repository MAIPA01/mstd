/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_FLAGS_HPP_
	#define _MSTD_FLAGS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/management_types.hpp>

namespace mstd {
		#if _MSTD_HAS_CXX20
	template<class BitsEnum>
		#else
	template<class BitsEnum, std::enable_if_t<std::is_enum_v<BitsEnum>, bool> >
		#endif
	_MSTD_REQUIRES(std::is_enum_v<BitsEnum>)
	class flags {
	public:
		using bits_type	 = BitsEnum;
		using value_type = std::underlying_type_t<BitsEnum>;

	private:
		value_type _flags = 0;

	public:
		#pragma region DEFAULT_CONSTRUCTORS
		_MSTD_CONSTEXPR17 flags() noexcept					 = default;
		_MSTD_CONSTEXPR17 flags(const flags& other) noexcept = default;
		_MSTD_CONSTEXPR17 flags(flags&& other) noexcept		 = default;
		#pragma endregion

		#pragma region CONVERSIONS_CONSTRUCTORS

		_MSTD_CONSTEXPR17 flags(const bits_type bit) noexcept : _flags(static_cast<value_type>(bit)) {}

		explicit _MSTD_CONSTEXPR17 flags(const value_type flags) noexcept : _flags(flags) {}

		#pragma endregion

		_MSTD_CONSTEXPR20 ~flags() noexcept								= default;

		#pragma region DEFAULT_ASSIGNMENT_OPERATOR
		_MSTD_CONSTEXPR17 flags& operator=(const flags& other) noexcept = default;
		_MSTD_CONSTEXPR17 flags& operator=(flags&& other) noexcept		= default;
		#pragma endregion

		[[nodiscard]] _MSTD_CONSTEXPR17 bool contains(const flags other) const noexcept {
			return (_flags & other._flags) == other._flags;
		}

		[[nodiscard]] _MSTD_CONSTEXPR17 value_type value() const noexcept { return _flags; }

		[[nodiscard]] _MSTD_CONSTEXPR17 explicit operator value_type() const noexcept { return value(); }

		[[nodiscard]] _MSTD_CONSTEXPR17 flags operator|(const flags other) const noexcept { return flags(_flags | other._flags); }

		[[nodiscard]] _MSTD_CONSTEXPR17 flags operator&(const flags other) const noexcept { return flags(_flags & other._flags); }

		[[nodiscard]] _MSTD_CONSTEXPR17 flags operator^(const flags other) const noexcept { return flags(_flags ^ other._flags); }

		_MSTD_CONSTEXPR17 flags& operator|=(const flags other) noexcept {
			_flags |= other._flags;
			return *this;
		}

		_MSTD_CONSTEXPR17 flags& operator&=(const flags other) noexcept {
			_flags &= other._flags;
			return *this;
		}

		_MSTD_CONSTEXPR17 flags& operator^=(const flags other) noexcept {
			_flags ^= other._flags;
			return *this;
		}

		[[nodiscard]] _MSTD_CONSTEXPR17 bool operator==(const flags other) const noexcept { return _flags == other._flags; }

		[[nodiscard]] _MSTD_CONSTEXPR17 bool operator!=(const flags other) const noexcept { return !(*this == other); }

		[[nodiscard]] _MSTD_CONSTEXPR17 flags operator~() const noexcept {
			return flags(~_flags & std::numeric_limits<value_type>::max());
		}

		[[nodiscard]] _MSTD_CONSTEXPR17 bool operator!() const noexcept { return _flags == 0; }

		[[nodiscard]] _MSTD_CONSTEXPR17 operator bool() const noexcept { return _flags != 0; }
	};

	template<class BitsEnum>
	[[nodiscard]] _MSTD_CONSTEXPR17 flags<BitsEnum> operator|(const BitsEnum bit, const BitsEnum otherBit) noexcept {
		return flags(bit) | otherBit;
	}
} // namespace mstd
	#endif
#endif