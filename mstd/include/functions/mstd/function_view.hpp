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

namespace mstd {
	namespace utils {
		#pragma region VALIDITY_CHECKS
		template<class FreeFunc, class F, class Ret, class... Args>
		static _MSTD_CONSTEXPR17 const bool is_valid_free_function_v = (is_same_function_v<F, FreeFunc> ||
																			std::is_invocable_r_v<Ret, FreeFunc, Args...>) &&
																		is_free_function_v<FreeFunc> &&
																		(!is_noexcept_function_v<F> ||
																			(is_noexcept_function_v<F> &&
																			is_noexcept_function_v<FreeFunc>));

		template<class MemberFunc, class F, class Ret, class... Args>
		static _MSTD_CONSTEXPR17 const bool is_valid_member_function_v = (is_same_function_v<F, MemberFunc> ||
																				std::is_invocable_r_v<Ret, MemberFunc, Args...>) &&
																			is_member_function_v<MemberFunc> &&
																			(!is_noexcept_function_v<F> ||
																				(is_noexcept_function_v<F> &&
																				is_noexcept_function_v<MemberFunc>));

		template<class Functor, bool is_const, class F, class Ret, class... Args>
		static _MSTD_CONSTEXPR17 const bool is_valid_functor_v = (is_same_function_v<F, Functor> ||
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
		concept valid_free_function = is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

		template<class MemberFunc, class F, class Ret, class... Args>
		concept valid_member_function = is_valid_member_function_v<MemberFunc, F, Ret, Args...>;

		template<class Functor, bool is_const, class F, class Ret, class... Args>
		concept valid_functor = is_valid_functor_v<Functor, is_const, F, Ret, Args...>;
		#endif
		#pragma endregion

		template<class F, class Ret, class ArgsTuple>
		struct base_function_view {};

		template<class F, class Ret, class... Args>
		struct base_function_view<F, Ret, std::tuple<Args...>> {
		private:
			template<class FreeFunc>
			static _MSTD_CONSTEXPR17 const bool _is_valid_free_function_v = utils::is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

			template<class MemberFunc>
			static _MSTD_CONSTEXPR17 const bool _is_valid_member_function_v = utils::is_valid_member_function_v<MemberFunc, F, Ret, Args...>;

			template<class Functor, bool is_const>
			static _MSTD_CONSTEXPR17 const bool _is_valid_functor_v = utils::is_valid_functor_v<Functor, is_const, F, Ret, Args...>;

			template<class FreeFunc>
			static Ret _free_func_invoker(void* func, void*, Args&&... args) noexcept(is_noexcept_function_v<FreeFunc>) {
				using func_ptr = c_func_t<function_type_t<FreeFunc>>;

				func_ptr free_func = reinterpret_cast<func_ptr>(func);
				return std::invoke(free_func, std::forward<Args>(args)...);
			}

			template<class MemberFunc>
			static Ret _member_func_invoker(void* func, void* parent, Args&&... args) noexcept(is_noexcept_function_v<MemberFunc>) {
				using parent_type = function_parent_t<MemberFunc>;
				using parent_ptr = parent_type*;
				using func_ptr = c_member_func_t<parent_type, function_type_t<MemberFunc>>;

				parent_ptr parent_obj = static_cast<parent_type*>(parent);
				func_ptr member_func = *reinterpret_cast<const func_ptr*>(func);
				return std::invoke(member_func, parent_obj, std::forward<Args>(args)...);
			}

			template<class Functor>
			static Ret _functor_invoker(void* func, void*, Args&&... args) noexcept(is_noexcept_function_v<Functor>) {
				return std::invoke(*static_cast<Functor*>(func), std::forward<Args>(args)...);
			}

			using _invoker_type = Ret(*)(void*, void*, Args&&...) noexcept(is_noexcept_function_v<F>);

			void* _func = nullptr;
			void* _parent = nullptr;
			_invoker_type _invoker = nullptr;

		public:
			base_function_view() noexcept = default;
			base_function_view(const base_function_view&) noexcept = default;
			base_function_view(base_function_view&& other) noexcept
				: _func(std::exchange(other._func, nullptr)), _parent(std::exchange(other._parent, nullptr)),
					_invoker(std::exchange(other._invoker, nullptr)) {}
			virtual ~base_function_view() noexcept = default;

			base_function_view& operator=(const base_function_view&) noexcept = default;
			base_function_view& operator=(base_function_view&& other) noexcept {
				_func = std::exchange(other._func, nullptr);
				_parent = std::exchange(other._parent, nullptr);
				_invoker = std::exchange(other._invoker, nullptr);
				return *this;
			}

			#pragma region FREE_FUNCTIONS
			#if _MSTD_HAS_CXX20
			template<class FreeFunc>
			requires(_is_valid_free_function_v<FreeFunc>)
			#else
			template<class FreeFunc, std::enable_if_t<_is_valid_free_function_v<FreeFunc>, bool> = true>
			#endif
			base_function_view(const FreeFunc& free_func) noexcept : _invoker(&_free_func_invoker<FreeFunc>) {
				if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
					_func = reinterpret_cast<void*>(free_func);
				}
				else {
					_func = reinterpret_cast<void*>(&free_func);
				}
			}

			#if _MSTD_HAS_CXX20
			template<class FreeFunc>
			requires(_is_valid_free_function_v<FreeFunc>)
			#else
			template<class FreeFunc, std::enable_if_t<_is_valid_free_function_v<FreeFunc>, bool> = true>
			#endif
			base_function_view& operator=(const FreeFunc& free_func) noexcept {
				_invoker = &_free_func_invoker<FreeFunc>;
				if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
					_func = reinterpret_cast<void*>(free_func);
				}
				else {
					_func = reinterpret_cast<void*>(&free_func);
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
			base_function_view(const function_parent_t<MemberFunc>*& parent_ptr, const MemberFunc& member_func) noexcept
				: _func(member_func), _parent(parent_ptr), _invoker(&_member_func_invoker<MemberFunc>) {}

			#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires(_is_valid_member_function_v<MemberFunc>)
			#else
			template<class MemberFunc, std::enable_if_t<_is_valid_member_function_v<MemberFunc>, bool> = true>
			#endif
			base_function_view(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept
				: _func(const_cast<void*>(reinterpret_cast<const void*>(&pair.second))),
				_parent(pair.first), _invoker(&_member_func_invoker<MemberFunc>) {}

			#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires(_is_valid_member_function_v<MemberFunc>)
			#else
			template<class MemberFunc, std::enable_if_t<_is_valid_member_function_v<MemberFunc>, bool> = true>
			#endif
			base_function_view& operator=(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept {
				_func = const_cast<void*>(reinterpret_cast<const void*>(&pair.second));
				_parent = pair.first;
				_invoker = &_member_func_invoker<MemberFunc>;
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
			base_function_view(Functor& functor) noexcept : _func(&functor), _invoker(&_functor_invoker<Functor>) {}

			#if _MSTD_HAS_CXX20
			template<class Functor>
			requires(_is_valid_functor_v<Functor, true>)
			#else
			template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
			#endif
			base_function_view(const Functor& functor) noexcept : _func(const_cast<void*>(static_cast<const void*>(&functor))),
				_invoker(&_functor_invoker<const Functor>) {}

			#if _MSTD_HAS_CXX20
			template<class Functor>
			requires(_is_valid_functor_v<Functor, true>)
			#else
			template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
			#endif
			base_function_view(Functor&& functor) noexcept = delete;

			#if _MSTD_HAS_CXX20
			template<class Functor>
			requires(_is_valid_functor_v<Functor, false>)
			#else
			template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, false>, bool> = true>
			#endif
			base_function_view& operator=(Functor& functor) noexcept {
				_func = &functor;
				_invoker = &_functor_invoker<Functor>;
				return *this;
			}

			#if _MSTD_HAS_CXX20
			template<class Functor>
			requires(_is_valid_functor_v<Functor, true>)
			#else
			template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
			#endif
			base_function_view& operator=(const Functor& functor) noexcept {
				_func = static_cast<void*>(const_cast<Functor*>(&functor));
				_invoker = &_functor_invoker<const Functor>;
				return *this;
			}

			#if _MSTD_HAS_CXX20
			template<class Functor>
			requires(_is_valid_functor_v<Functor, true>)
			#else
			template<class Functor, std::enable_if_t<_is_valid_functor_v<Functor, true>, bool> = true>
			#endif
			base_function_view& operator=(Functor&& functor) noexcept = delete;
			#pragma endregion

			void reset() noexcept {
				_func = nullptr;
				_parent = nullptr;
				_invoker = nullptr;
			}

			Ret invoke(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
				return _invoker(_func, _parent, std::forward<Args>(args)...);
			}

			Ret operator()(Args&&... args) const noexcept(is_noexcept_function_v<F>) {
				return invoke(std::forward<Args>(args)...);
			}
		};

		template<class F>
		using base_function_view_t = base_function_view<F, function_return_t<F>, function_args_t<F>>;

		template<class F>
		static _MSTD_CONSTEXPR17 const bool is_valid_function_view_function_v = mstd::is_function_v<F> &&
																				!is_const_function_v<F> &&
																				!is_volatile_function_v<F> &&
																				!is_parent_ref_function_v<F> &&
																				!is_parent_moved_function_v<F>;

		#if _MSTD_HAS_CXX20
		template<class F>
		concept valid_function_view_function = is_valid_function_view_function_v<F>;
		#endif
	}

	#if _MSTD_HAS_CXX20
	template<utils::valid_function_view_function F>
	#else
	template<class F, std::enable_if_t<utils::is_valid_function_view_function_v<F>, bool> = true>
	#endif
	class function_view : public utils::base_function_view_t<F> {
	private:
		using _base = utils::base_function_view_t<F>;

		template<class Func>
		static _MSTD_CONSTEXPR17 const bool _can_be_noexcept_v = !is_noexcept_function_v<F> ||
																	(is_noexcept_function_v<F> && is_noexcept_function_v<Func>);

	public:
		function_view() noexcept = default;
		function_view(const function_view&) noexcept = default;
		function_view(function_view&&) noexcept = default;
		~function_view() noexcept override = default;

		template<class Func, std::enable_if_t<!std::is_same_v<std::decay_t<Func>, function_view> && _can_be_noexcept_v<Func>, bool> = true>
		function_view(Func& func) noexcept : _base(func) {}

		template<class Func, std::enable_if_t<!std::is_same_v<std::decay_t<Func>, function_view> && _can_be_noexcept_v<Func>, bool> = true>
		function_view(const Func& func) noexcept : _base(func) {}

		template<class MemberFunc, std::enable_if_t<_can_be_noexcept_v<MemberFunc>, bool> = true>
		function_view(const function_parent_t<MemberFunc>*& parent_ptr, const MemberFunc& member_func) noexcept
			: _base(parent_ptr, member_func) {}

		template<class MemberFunc, std::enable_if_t<_can_be_noexcept_v<MemberFunc>, bool> = true>
		function_view(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept : _base(pair) {}

		function_view& operator=(const function_view&) noexcept = default;
		function_view& operator=(function_view&&) noexcept = default;

		template<class Func, std::enable_if_t<!std::is_same_v<std::decay_t<Func>, function_view> && _can_be_noexcept_v<Func>, bool> = true>
		function_view& operator=(Func& func) noexcept {
			_base::operator=(func);
			return *this;
		}

		template<class Func, std::enable_if_t<!std::is_same_v<std::decay_t<Func>, function_view> && _can_be_noexcept_v<Func>, bool> = true>
		function_view& operator=(const Func& func) noexcept {
			_base::operator=(func);
			return *this;
		}

		template<class Func, std::enable_if_t<!std::is_same_v<std::decay_t<Func>, function_view> && _can_be_noexcept_v<Func>, bool> = true>
		function_view& operator=(Func&& func) noexcept = delete;

		template<class MemberFunc, std::enable_if_t<_can_be_noexcept_v<MemberFunc>, bool> = true>
		function_view& operator=(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept {
			_base::operator=(pair);
			return *this;
		}
	};
}

#endif
#endif