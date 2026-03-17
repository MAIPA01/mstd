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
	inline _MSTD_CONSTEXPR20 uint32_t swap_endians(uint32_t value) noexcept {
		return  ((value >> 24) & 0x000000FF) |
				((value >>  8) & 0x0000FF00) |
				((value <<  8) & 0x00FF0000) |
				((value << 24) & 0xFF000000);
	}
}
#endif
#endif