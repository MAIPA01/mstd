/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "string_libs.hpp"

namespace mstd {
	template<class _String>
	struct _string_size_impl {};

	template<size_t N>
	struct _string_size_impl<const char[N]> {
		static constexpr size_t size(const char(&)[N]) { return N - 1; }
	};

	template<size_t N>
	struct _string_size_impl<char[N]> {
		static constexpr size_t size(char (&s)[N]) { return N ? std::strlen(s) : 0; }
	};

	template<>
	struct _string_size_impl<const char*> {
		static constexpr size_t size(const char* s) { return s ? std::strlen(s) : 0; }
	};

	template<>
	struct _string_size_impl<char*> {
		static constexpr size_t size(char* s) { return s ? std::strlen(s) : 0; }
	};

	template<>
	struct _string_size_impl<std::string> {
		static constexpr size_t size(const std::string& s) { return s.size(); }
	};

	template<class _String>
	static constexpr size_t string_size(_String&& s) {
		using string_t = std::remove_reference_t<_String>;
		return _string_size_impl<string_t>::size(s);
	}

	template<class _String, class... _Rest>
	struct _concat_impl {
		static constexpr size_t size(_String&& s, _Rest&&... rest) {
			if constexpr (sizeof...(_Rest) == 0) {
				return string_size(s);
			}
			else {
				return string_size(s) + _concat_impl<_Rest...>::size(std::forward<_Rest>(rest)...);
			}
		}

		static constexpr void concat(std::string& str, _String&& s, _Rest&&... rest) {
			str += s;
			if constexpr (sizeof...(_Rest) > 0) {
				_concat_impl<_Rest...>::concat(str, std::forward<_Rest>(rest)...);
			}
		}
	};
}