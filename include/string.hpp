#pragma once
#include "string_types.hpp"
#include "isstrnum.hpp"
#include "strtonum.hpp"

namespace mstd {
	static constexpr std::string trim(const std::string& str) {
		const std::string::const_iterator & start = 
			std::find_if_not(str.begin(), str.end(), &std::isspace);
		const std::string::const_iterator& end = 
			std::find_if_not(str.rbegin(), str.rend(), &std::isspace).base();

		return (start < end) ? std::string(start, end) : std::string();
	}

	template<class... _Strings>
	static constexpr std::string concat(_Strings&&... strs) {
		std::string str;
		str.reserve(_concat_impl<_Strings...>::size(std::forward<_Strings>(strs)...));
		_concat_impl<_Strings...>::concat(str, std::forward<_Strings>(strs)...);
		return str;
	}
}