#pragma once
#include "arithmetic_types.hpp"

namespace mstd {
	static constexpr uint32_t swap_endians(uint32_t value) noexcept {
		return  ((value >> 24) & 0x000000FF) |
				((value >>  8) & 0x0000FF00) |
				((value <<  8) & 0x00FF0000) |
				((value << 24) & 0xFF000000);
	}
}