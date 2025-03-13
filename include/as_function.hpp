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

namespace mstd {
	// Convert to std::function type
	// main template - default false
	template<class F, class = void>
	struct as_function;

	// for std::function
	template<class R, class... Args>
	struct as_function<std::function<R(Args...)>> {
		using type = std::function<R(Args...)>;
	};

	// for function pointers
	template<class R, class... Args>
	struct as_function<R(*)(Args...)> {
		using type = std::function<R(Args...)>;
	};

	// for function references
	template<class R, class... Args>
	struct as_function<R(&)(Args...)> {
		using type = std::function<R(Args...)>;
	};

	// for callable objects (lambdas, functors)
	template<class F>
	struct as_function<F, std::void_t<decltype(&F::operator())>> : as_function<decltype(&F::operator())> {};

	// for member function pointers (non-const)
	template<class C, class R, class... Args>
	struct as_function<R(C::*)(Args...)> {
		using type = std::function<R(Args...)>;
	};

	// for member function pointers (const)
	template<class C, class R, class... Args>
	struct as_function<R(C::*)(Args...) const> {
		using type = std::function<R(Args...)>;
	};

	// for member function pointers (volatile)
	template<class C, class R, class... Args>
	struct as_function<R(C::*)(Args...) volatile> {
		using type = std::function<R(Args...)>;
	};

	// for member function pointers (const volatile)
	template<class C, class R, class... Args>
	struct as_function<R(C::*)(Args...) const volatile> {
		using type = std::function<R(Args...)>;
	};

	// Helper alias
	template<class F>
	using as_function_t = as_function<F>::type;
}