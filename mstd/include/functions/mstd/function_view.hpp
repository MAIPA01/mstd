/*
* mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_FUNCTION_VIEW_HPP_
#define _MSTD_FUNCTION_VIEW_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/functions_types.hpp>
#include <mstd/is_same_function.hpp>
#include <mstd/types.hpp>

namespace mstd {
	#pragma region CONTAINERS
	template<bool is_noexcept, class Ret, class... Args>
	struct _base_function_container {
		_base_function_container() noexcept = default;
		virtual ~_base_function_container() noexcept = default;

		virtual Ret invoke(Args&&... args) const noexcept(is_noexcept) = 0;
	};

	template<class F, bool is_noexcept, class Ret, class... Args>
	struct _free_function_container : _base_function_container<is_noexcept, Ret, Args...> {
	public:
		using function_type = function_type_t<F>;
		using function_ptr = c_func_t<function_type>;

	private:
		function_ptr _func = nullptr;

	public:
		_free_function_container(function_ptr function) noexcept : _func(function) {}
		~_free_function_container() noexcept override = default;

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(_func, std::forward<Args>(args)...);
		}
	};

	template<class F, class C, bool is_noexcept, class Ret, class... Args>
	struct _member_function_container : _base_function_container<is_noexcept, Ret, Args...> {
	public:
		using parent_type = C;
		using parent_ptr = parent_type*;

		using function_type = function_type_t<F>;
		using function_ptr = c_member_func_t<parent_type, function_type>;

	private:
		parent_ptr _parent = nullptr;
		function_ptr _func = nullptr;

	public:
		_member_function_container(parent_ptr parent, function_ptr function) noexcept
			: _parent(parent), _func(function) {}
		~_member_function_container() noexcept override = default;

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(_func, _parent, std::forward<Args>(args)...);
		}
	};

	template<class F, bool is_const, bool is_noexcept, class Ret, class... Args>
	struct _functor_container : _base_function_container<is_noexcept, Ret, Args...> {
	public:
		using functor_type = std::conditional_t<is_const, const F, F>;
		using functor_ptr = functor_type*;
		using functor_ref = functor_type&;

	private:
		functor_ptr _func = nullptr;

	public:
		_functor_container(functor_ref functor) noexcept : _func(&functor) {}
		~_functor_container() noexcept override = default;

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) override {
			return std::invoke(*_func, std::forward<Args>(args)...);
		}
	};
	#pragma endregion

	#pragma region VALIDITY_CHECKS
	template<class FreeFunc, class F, class Ret, class... Args>
	static _MSTD_CONSTEXPR17 const bool _is_valid_free_function_v = (is_same_function_v<F, FreeFunc> ||
																		std::is_invocable_r_v<Ret, FreeFunc, Args...>) &&
																	is_free_function_v<FreeFunc> &&
																	(!is_noexcept_function_v<F> ||
																		(is_noexcept_function_v<F> &&
																		is_noexcept_function_v<FreeFunc>));

	template<class MemberFunc, class F, class Ret, class... Args>
	static _MSTD_CONSTEXPR17 const bool _is_valid_member_function_v = (is_same_function_v<F, MemberFunc> ||
																			std::is_invocable_r_v<Ret, MemberFunc, Args...>) &&
																		is_member_function_v<MemberFunc> &&
																		(!is_noexcept_function_v<F> ||
																			(is_noexcept_function_v<F> &&
																			is_noexcept_function_v<MemberFunc>));

	template<class Functor, bool is_const, class F, class Ret, class... Args>
	static _MSTD_CONSTEXPR17 const bool _is_valid_functor_v = (is_same_function_v<F, Functor> ||
																	std::is_invocable_r_v<Ret, Functor, Args...>) &&
																is_functor_v<Functor> &&
																(!is_noexcept_function_v<F> ||
																	(is_noexcept_function_v<F> &&
																	is_noexcept_function_v<Functor>)) &&
																(!is_const ||
																	(is_const &&
																	is_const_function_v<Functor>));

	#if _MSTD_HAS_CXX20
	template<class FreeFunc, class F, class Ret, class... Args>
	concept _valid_free_function = _is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

	template<class MemberFunc, class F, class Ret, class... Args>
	concept _valid_member_function = _is_valid_member_function_v<MemberFunc, F, Ret, Args...>;

	template<class Functor, bool is_const, class F, class Ret, class... Args>
	concept _valid_functor = _is_valid_functor_v<Functor, is_const, F, Ret, Args...>;
	#endif
	#pragma endregion

	template<class F, class Ret, class ArgsTuple>
	struct _base_function_view {};

	template<class F, class Ret, class... Args>
	struct _base_function_view<F, Ret, std::tuple<Args...>> {
	private:
		template<class FreeFunc>
		using _free_function_container_type = _free_function_container<FreeFunc, is_noexcept_function_v<F>, Ret, Args...>;

		template<class MemberFunc>
		using _member_function_container_type = _member_function_container<MemberFunc,
													function_parent_t<MemberFunc>, is_noexcept_function_v<F>, Ret, Args...>;

		template<class Functor, bool is_const>
		using _functor_container_type = _functor_container<Functor, is_const, is_noexcept_function_v<F>, Ret, Args...>;

		template<class FreeFunc>
		static _MSTD_CONSTEXPR17 const bool _is_valid_free_function_v = mstd::_is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

		template<class MemberFunc>
		static _MSTD_CONSTEXPR17 const bool _is_valid_member_function_v = mstd::_is_valid_member_function_v<MemberFunc, F, Ret, Args...>;

		template<class Functor, bool is_const>
		static _MSTD_CONSTEXPR17 const bool _is_valid_functor_v = mstd::_is_valid_functor_v<Functor, is_const, F, Ret, Args...>;

		using _base_container_type = _base_function_container<is_noexcept_function_v<F>, Ret, Args...>;
		std::shared_ptr<_base_container_type> _container = nullptr;

	public:
		_base_function_view() noexcept = default;
		_base_function_view(const _base_function_view&) noexcept = default;
		_base_function_view(_base_function_view&& other) noexcept = default;
		virtual ~_base_function_view() noexcept = default;

		_base_function_view& operator=(const _base_function_view&) noexcept = default;
		_base_function_view& operator=(_base_function_view&& other) noexcept = default;

		#pragma region FREE_FUNCTIONS
		#if _MSTD_HAS_CXX20
		template<class FreeFunc>
		requires(_is_valid_free_function_v<FreeFunc>)
		#else
		template<class FreeFunc, std::enable_if_t<_is_valid_free_function_v<FreeFunc>, bool> = true>
		#endif
		_base_function_view(const FreeFunc& free_func) noexcept {
			using container_type = _free_function_container_type<FreeFunc>;

			if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
				_container = std::make_shared<container_type>(free_func);
			}
			else {
				_container = std::make_shared<container_type>(&free_func);
			}
		}

		#if _MSTD_HAS_CXX20
		template<class FreeFunc>
		requires(_is_valid_free_function_v<FreeFunc>)
		#else
		template<class FreeFunc, std::enable_if_t<_is_valid_free_function_v<FreeFunc>, bool> = true>
		#endif
		_base_function_view& operator=(const FreeFunc& free_func) noexcept {
			using container_type = _free_function_container_type<FreeFunc>;

			if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
				_container = std::make_shared<container_type>(free_func);
			}
			else if _MSTD_CONSTEXPR17 (is_function_moved_v<FreeFunc> || is_function_ref_v<FreeFunc>) {
				_container = std::make_shared<container_type>(&free_func);
			}
			return *this;
		}
		#pragma endregion

		#pragma region MEMBER_FUNCTIONS
		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires(_is_valid_member_function_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<_is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
		_base_function_view(const function_parent_t<MemberFunc>*& parent_ptr, const MemberFunc& member_func) noexcept
			: _container(
				std::make_shared<_member_function_container_type<MemberFunc>>(parent_ptr, member_func)
			) {}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires(_is_valid_member_function_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<_is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
		_base_function_view(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept
			: _container(
				std::make_shared<_member_function_container_type<MemberFunc>>(pair.first, pair.second)
			) {}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires(_is_valid_member_function_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<_is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
		_base_function_view& operator=(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept {
			_container = std::make_shared<_member_function_container_type<MemberFunc>>(pair.first, pair.second);
			return *this;
		}
		#pragma endregion

		#pragma region FUNCTORS
		#if _MSTD_HAS_CXX20
		template<class Functor>
		requires(_is_valid_functor_v<Functor, false>)
		#else
		template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, false>, bool> = true>
		#endif
		_base_function_view(Functor& functor) noexcept : _container(std::make_shared<_functor_container_type<Functor, false>>(functor)) {}

		#if _MSTD_HAS_CXX20
		template<class Functor>
		requires(_is_valid_functor_v<Functor, true>)
		#else
		template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
		#endif
		_base_function_view(const Functor& functor) noexcept : _container(std::make_shared<_functor_container_type<Functor, true>>(functor)) {}

		#if _MSTD_HAS_CXX20
		template<class Functor>
		requires(_is_valid_functor_v<Functor, false>)
		#else
		template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, false>, bool> = true>
		#endif
		_base_function_view& operator=(Functor& functor) noexcept {
			_container = std::make_shared<_functor_container_type<Functor, false>>(functor);
			return *this;
		}

		#if _MSTD_HAS_CXX20
		template<class Functor>
		requires(_is_valid_functor_v<Functor, true>)
		#else
		template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
		#endif
		_base_function_view& operator=(const Functor& functor) noexcept {
			_container = std::make_shared<_functor_container_type<Functor, true>>(functor);
			return *this;
		}
		#pragma endregion

		Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
		    return _container->invoke(std::forward<Args>(args)...);
	    }

		Ret operator()(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
			return invoke(std::forward<Args>(args)...);
		}
	};

	template<class F>
	using _base_function_view_t = _base_function_view<F, function_return_t<F>, function_args_t<F>>;

	template<class F>
	static _MSTD_CONSTEXPR17 const bool _is_valid_function_view_function_v = mstd::is_function_v<F> &&
																			!is_const_function_v<F> &&
																			!is_volatile_function_v<F> &&
																			!is_parent_ref_function_v<F> &&
																			!is_parent_moved_function_v<F>;

	#if _MSTD_HAS_CXX20
	template<class F>
	concept _valid_function_view_function = _is_valid_function_view_function_v<F>;
	#endif

	#if _MSTD_HAS_CXX20
	template<_valid_function_view_function F>
	#else
	template<class F, std::enable_if_t<_is_valid_function_view_function_v<F>, bool> = true>
	#endif
	class function_view : public _base_function_view_t<F> {
	private:
		using _base = _base_function_view_t<F>;

	public:
		function_view() noexcept = default;
		function_view(const function_view&) noexcept = default;
		function_view(function_view&&) noexcept = default;
		~function_view() noexcept override = default;

		template<class Func, std::enable_if_t<!std::is_same_v<Func, function_view> && (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Func>)), bool> = true>
		function_view(Func& func) noexcept : _base(func) {}

		template<class Func, std::enable_if_t<!std::is_same_v<Func, function_view> && (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Func>)), bool> = true>
		function_view(const Func& func) noexcept : _base(func) {}

		template<class MemberFunc, std::enable_if_t<(!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<MemberFunc>)), bool> = true>
		function_view(const function_parent_t<MemberFunc>*& parent_ptr, const MemberFunc& member_func) noexcept
			: _base(parent_ptr, member_func) {}

		template<class MemberFunc, std::enable_if_t<(!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<MemberFunc>)), bool> = true>
		function_view(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept
			: _base(pair) {}

		function_view& operator=(const function_view&) noexcept = default;
		function_view& operator=(function_view&&) noexcept = default;

		template<class Func, std::enable_if_t<!std::is_same_v<Func, function_view> && (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Func>)) && is_functor_v<Func>, bool> = true>
		function_view& operator=(Func& func) noexcept {
			_base::operator=(func);
			return *this;
		}

		template<class Func, std::enable_if_t<!std::is_same_v<Func, function_view> && (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Func>)) && !is_functor_v<Func>, bool> = true>
		function_view& operator=(const Func& func) noexcept {
			_base::operator=(func);
			return *this;
		}

		template<class MemberFunc, std::enable_if_t<(!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<MemberFunc>)), bool> = true>
		function_view& operator=(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept {
			_base::operator=(pair);
			return *this;
		}
	};

	// use function_view in event_handler and test adding function_view to assert as logger function
}

#endif
#endif