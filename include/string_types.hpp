/*
 * mstd - Maipa's Standard Library
 *
 * BSD 3-Clause License with Attribution Requirement
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgment:
 *    - "This product includes software developed by Patryk Antosik (MAIPA01) as
 *      part of the mstd project."
 * 4. If you modify this software and distribute it, you must include a notice
 *    stating that the original work was created by Patryk Antosik (MAIPA01) as
 *    part of the mstd project.
 * 5. Neither the name of the author nor the name mstd may be used to endorse or
 *    promote products derived from this software without specific prior written
 *    permission.
 * 6. Sale of unmodified code: The unmodified version of this software may not be
 *    sold without the explicit permission of the author.
 * 7. Sale of modified code: Modified versions of the code may be sold, provided that:
 *    - Proper attribution to Patryk Antosik (MAIPA01) as the original author is maintained.
 *    - The modified code is clearly marked as a modified version.
 *    - The modified code must be accompanied by documentation that includes
 *      references to the original version and its author.
 * 8. Exception for large projects: If this code is used as part of a larger project
 *    (e.g., an application, framework, or other libraries), permission is not required
 *    for the sale of the unmodified version, as long as:
 *    - The project is more than just a wrapper for the original code.
 *    - The code is an integral part of the larger project, and the project does not
 *      merely involve selling the original code.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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