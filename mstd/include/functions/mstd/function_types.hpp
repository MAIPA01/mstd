/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include "as_std_function.hpp"

namespace mstd {
	template<class F>
	using func = as_std_function<F>;

	template<class... Args> 
	using action = func<void(Args...)>;

	using method = action<>;

	template<class F, class C>
	struct _cfunc_impl {};

	template<class Ret, class... Args>
	struct _cfunc_impl<Ret(Args...), void> {
		using type = Ret(*)(Args...);
	};

	template<class Ret, class... Args, class C>
	struct _cfunc_impl<Ret(Args...), C> {
		using type = Ret(C::*)(Args...);
	};

	template<class F, class C = void>
	using cfunc = typename _cfunc_impl<F, C>::type;

	template<class... Args>
	using caction = cfunc<void(Args...)>;

	using cmethod = caction<>;

	template<class F, class>
	struct _is_func_impl;


}
#endif