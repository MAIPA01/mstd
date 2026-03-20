/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_BIT_OPERATIONS_HPP_
	#define _MSTD_BIT_OPERATIONS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/arithmetic_types.hpp>

namespace mstd {
	_MSTD_INLINE17 _MSTD_CONSTEXPR20 uint32_t swap_endians(uint32_t value) noexcept {
		_MSTD_CONSTEXPR17 const uint32_t first_elem_mask	   = 0x0000'00FF;
		_MSTD_CONSTEXPR17 const uint32_t second_elem_mask	   = 0x0000'FF00;
		_MSTD_CONSTEXPR17 const uint32_t third_elem_mask	   = 0x00FF'0000;
		_MSTD_CONSTEXPR17 const uint32_t forth_elem_mask	   = 0xFF00'0000;

		_MSTD_CONSTEXPR17 const uint32_t corner_elements_shift = 24;
		_MSTD_CONSTEXPR17 const uint32_t middle_elements_shift = 8;

		return ((value >> corner_elements_shift) & first_elem_mask) |
			   ((value >> middle_elements_shift) & second_elem_mask) |
			   ((value << middle_elements_shift) & third_elem_mask) |
			   ((value << corner_elements_shift) & forth_elem_mask);
	}
} // namespace mstd
	#endif
#endif