#pragma once
#include "arithmetic_libs.hpp"

namespace mstd {
	template<class... Ns>
	constexpr bool are_unsigned_v = (std::is_unsigned_v<Ns> && ...);

	template<class... Ns>
	constexpr bool are_arithmetic_v = (std::is_arithmetic_v<Ns> && ...);
}