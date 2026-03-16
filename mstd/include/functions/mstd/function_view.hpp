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
#include "is_same_function.hpp"


#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/assert.hpp>

namespace mstd {
	template<class Ret, class... Args>
	struct _base_function_container {
		_base_function_container() = default;
		virtual ~_base_function_container() = default;

		virtual Ret invoke(Args&&... args) const = 0;
	};

	template<class F, class Ret, class... Args>
	struct _free_function_container : _base_function_container<Ret, Args...> {
	public:
		using function_type = Ret(Args...);
		using function_ptr = c_func_t<function_type>;

	private:
		function_ptr _func = nullptr;

	public:
		_free_function_container(function_ptr function) : _func(function) {}
		~_free_function_container() override = default;

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(_func, std::forward<Args>(args)...);
		}
	};

	template<class F, class C, class Ret, class... Args>
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

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(_func, _parent, std::forward<Args>(args)...);
		}
	};

	template<class F, class Ret, class ArgsTuple>
	struct _functor_container {};

	template<class F, class Ret, class... Args>
	struct _functor_container<F, Ret, std::tuple<Args...>> : _base_function_container<Ret, Args...> {
	public:
		using functor_type = F;
		using functor_ptr = functor_type*;

	private:
		functor_ptr _func = nullptr;

	public:
		_functor_container(F& functor) : _func(&functor) {}
		~_functor_container() override = default;

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(*_func, std::forward<Args>(args)...);
		}
	};

	template<class F>
	using _functor_container_t = _functor_container<F, function_return_t<F>, function_args_t<F>>;

	template<class Ret, class ArgsTuple>
	struct _get_base_container_type {};

	template<class Ret, class... Args>
	struct _get_base_container_type<Ret, std::tuple<Args...>> {
		using type = _base_function_container<Ret, Args...>;
	};

	template<class F>
	using _get_base_container_type_t = typename _get_base_container_type<function_return_t<F>, function_args_t<F>>::type;

	template<class F, class Ret, class ArgsTuple>
	struct _base_function_view {};

	template<class F, class Ret, class... Args>
	struct _base_function_view<F, Ret, std::tuple<Args...>> {
	private:
		using _base_container_type = _get_base_container_type_t<F>;
		_base_container_type* _container = nullptr;

	public:
		_base_function_view() noexcept = default;
		template<class FreeFunc, std::enable_if_t<(is_same_function_v<FreeFunc, F> && is_free_function_v<FreeFunc>), bool> = true>
		_base_function_view(const FreeFunc& free_func) {
			if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
				_container = new _free_function_container<FreeFunc, Ret, Args...>(free_func);
			}
			else if _MSTD_CONSTEXPR17 (is_function_moved_v<FreeFunc> || is_function_ref_v<FreeFunc>) {
				_container = new _free_function_container<FreeFunc, Ret, Args...>(&free_func);
			}
		}
		template<class MemberFunc, std::enable_if_t<(is_same_function_v<MemberFunc, F> && is_member_function_v<MemberFunc> && !is_functor_v<MemberFunc>), bool> = true>
		_base_function_view(function_parent_t<MemberFunc>* parent_ptr, const MemberFunc& member_func) {
			_container = new _member_function_container<MemberFunc, function_parent_t<MemberFunc>, Ret, Args...>(parent_ptr, member_func);
		}
		template<class Functor, std::enable_if_t<(is_same_function_v<Functor, F> && is_functor_v<Functor>), bool> = true>
		_base_function_view(Functor& functor) {
			_container = new _functor_container_t<Functor>(functor);
		}

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
			mstd_assert(_container != nullptr);
			return _container->invoke(std::forward<Args>(args)...);
		}

		Ret operator()(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
			return invoke(std::forward<Args>(args)...);
		}
	};

	template<class F>
	using _base_function_view_t = _base_function_view<F, function_return_t<F>, function_args_t<F>>;

	template<class F, std::enable_if_t<is_function_v<F>, bool> = true>
	class function_view : public _base_function_view_t<F> {
	private:
		using _base = _base_function_view_t<F>;

	public:
		function_view() noexcept = default;
		template<class StaticFunc, std::enable_if_t<(is_same_function_v<StaticFunc, F> && is_free_function_v<StaticFunc>), bool> = true>
		function_view(const StaticFunc& static_func) : _base(static_func) {}
		template<class MemberFunc, std::enable_if_t<(is_same_function_v<MemberFunc, F> && is_member_function_v<MemberFunc> && !is_functor_v<MemberFunc>), bool> = true>
		function_view(function_parent_t<MemberFunc>* parent_ptr, const MemberFunc& member_func) : _base(parent_ptr, member_func) {}
		template<class Functor, std::enable_if_t<(is_same_function_v<Functor, F> && is_functor_v<Functor>), bool> = true>
		function_view(const Functor& functor) : _base(functor) {}
	};
}

#endif