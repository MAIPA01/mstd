/*
* mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "function_types.hpp"


#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

namespace mstd {
	template<class Ret, class... Args>
	struct _base_function_container {
		_base_function_container() = default;
		virtual ~_base_function_container() = default;

		virtual Ret invoke(Args&&... args) const = 0;
	};

	template<class Ret, class... Args>
	struct _free_function_container : _base_function_container<Ret, Args...> {
	public:
		using function_type = Ret(Args...);
		using function_ptr = c_func_t<function_type>;

	private:
		function_ptr _func = nullptr;

	public:
		_free_function_container(function_ptr function) : _func(function) {}
		~_free_function_container() override = default;

		Ret invoke(Args&&... args) const override {
			return std::invoke(_func, std::forward<Args>(args)...);
		}
	};

	template<class C, class Ret, class... Args>
	struct _member_function_container : _base_function_container<Ret, Args...> {
	public:
		using parent_type = C;
		using parent_ptr = parent_type*;
		using function_type = Ret(Args...);
		using function_ptr = c_member_func_t<parent_type, function_type>;

	private:
		parent_ptr _parent = nullptr;
		function_ptr _func = nullptr;

	public:
		_member_function_container(parent_ptr parent, function_ptr function)
			: _parent(parent), _func(function) {}
		~_member_function_container() override = default;

		Ret invoke(Args&&... args) const override {
			return std::invoke(_func, _parent, std::forward<Args>(args)...);
		}
	};

	template<class Ret, class ArgsTuple>
	struct _get_base_container_type {};

	template<class Ret, class... Args>
	struct _get_base_container_type<Ret, std::tuple<Args...>> {
		using type = _base_function_container<Ret, Args...>;
	};

	template<class F, std::enable_if_t<mstd::is_function_v<F>, bool> = true>
	class function_view {
	private:
		using _base_container_type = typename _get_base_container_type<function_return_t<F>, function_args_t<F>>::type;

		_base_container_type* _container = nullptr;

	public:
		function_view()
	};
}

#endif