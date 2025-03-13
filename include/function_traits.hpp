/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "functions_libs.hpp"
#include "as_function.hpp"

namespace mstd {
	template<class F, class = void>
	struct function_traits;

	template<class R, class... Args>
	struct function_traits<std::function<R(Args...)>> {
		using return_type = std::decay_t<R>;
		using args_tuple = std::tuple<std::decay_t<Args>...>;
		static constexpr size_t args_num = sizeof...(Args);
	};

	template<class F>
	struct function_traits<F, std::void_t<as_function_t<F>>> : function_traits<as_function_t<F>> {};

	template<class F>
	using function_args_t = function_traits<F>::args_tuple;

	template<class F>
	using function_return_t = function_traits<F>::return_type;

	template<class F>
	constexpr size_t function_args_num_v = function_traits<F>::args_num;
}