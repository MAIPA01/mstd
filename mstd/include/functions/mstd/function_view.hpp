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

	#include "fmt/chrono.h"


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
		static _MSTD_CONSTEXPR17 bool is_valid_free_function_v =
		  (is_same_function_v<F, FreeFunc> || std::is_invocable_r_v<Ret, FreeFunc, Args...>) && is_free_function_v<FreeFunc> &&
		  (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<FreeFunc>));

		template<class MemberFunc, class F, class Ret, class... Args>
		static _MSTD_CONSTEXPR17 bool is_valid_member_function_v =
		  (is_same_function_v<F, MemberFunc> || std::is_invocable_r_v<Ret, MemberFunc, Args...>) &&
		  is_member_function_v<MemberFunc> &&
		  (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<MemberFunc>));

		template<class Functor, bool IsConst, class F, class Ret, class... Args>
		static _MSTD_CONSTEXPR17 bool is_valid_functor_v =
		  (is_same_function_v<F, Functor> || std::is_invocable_r_v<Ret, Functor, Args...>) && is_functor_v<Functor> &&
		  (!is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Functor>)) &&
		  (!IsConst || (IsConst && is_const_function_v<Functor>));

		#if _MSTD_HAS_CXX20
		template<class FreeFunc, class F, class Ret, class... Args>
		concept valid_free_function = is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

		template<class MemberFunc, class F, class Ret, class... Args>
		concept valid_member_function = is_valid_member_function_v<MemberFunc, F, Ret, Args...>;

		template<class Functor, bool IsConst, class F, class Ret, class... Args>
		concept valid_functor = is_valid_functor_v<Functor, IsConst, F, Ret, Args...>;
		#endif
		#pragma endregion

		struct function_storage {
			union {
				void* functor_ptr = nullptr;
				void (*func_ptr)();
				std::byte mem_func_buffer[sizeof(void*) * 3];
			};
		};

		template<class F, class Ret, class ArgsTuple>
		struct base_function_view {};

		template<class F, class Ret, class... Args>
		struct base_function_view<F, Ret, std::tuple<Args...> > {
		private:
			template<class FreeFunc>
			static _MSTD_CONSTEXPR17 bool is_valid_free_function_v = utils::is_valid_free_function_v<FreeFunc, F, Ret, Args...>;

			template<class MemberFunc>
			static _MSTD_CONSTEXPR17 bool is_valid_member_function_v =
			  utils::is_valid_member_function_v<mstd::remove_cvref_t<MemberFunc>, F, Ret, Args...>;

			template<class Functor, bool IsConst>
			static _MSTD_CONSTEXPR17 bool is_valid_functor_v = utils::is_valid_functor_v<Functor, IsConst, F, Ret, Args...>;

			template<class FreeFunc>
			static Ret _free_func_invoker(const function_storage& storage, void*,
			  Args&&... args) noexcept(is_noexcept_function_v<FreeFunc>) {
				using func_ptr	  = c_func_t<function_type_t<FreeFunc> >;

				func_ptr freeFunc = reinterpret_cast<func_ptr>(storage.func_ptr);
				return std::invoke(freeFunc, std::forward<Args>(args)...);
			}

			template<class MemberFunc>
			static Ret _member_func_invoker(const function_storage& storage, void* parent,
			  Args&&... args) noexcept(is_noexcept_function_v<MemberFunc>) {
				using parent_type	 = function_parent_t<MemberFunc>;
				using parent_ptr	 = parent_type*;
				using func_ptr		 = c_member_func_t<parent_type, function_type_t<MemberFunc> >;

				parent_ptr parentObj = static_cast<parent_type*>(parent);
				func_ptr memberFunc;
				std::memcpy(&memberFunc, storage.mem_func_buffer, sizeof(func_ptr));

				return std::invoke(memberFunc, parentObj, std::forward<Args>(args)...);
			}

			template<class Functor>
			static Ret _functor_invoker(const function_storage& storage, void*,
			  Args&&... args) noexcept(is_noexcept_function_v<Functor>) {
				return std::invoke(*static_cast<Functor*>(storage.functor_ptr), std::forward<Args>(args)...);
			}

			using _invoker_type = Ret (*)(const function_storage&, void*, Args&&...) noexcept(is_noexcept_function_v<F>);

			function_storage _storage;
			void* _parent		   = nullptr;
			_invoker_type _invoker = nullptr;

		public:
			base_function_view() noexcept { std::memset(&_storage, 0, sizeof(function_storage)); }

			base_function_view(std::nullptr_t) noexcept : base_function_view() {}

			base_function_view(const base_function_view& other) noexcept
				: _storage(other._storage), _parent(other._parent), _invoker(other._invoker) {}

			base_function_view(base_function_view&& other) noexcept
				: _parent(std::exchange(other._parent, nullptr)), _invoker(std::exchange(other._invoker, nullptr)) {
				_storage = other._storage;
				std::memset(&other._storage, 0, sizeof(function_storage));
			}

			virtual ~base_function_view() noexcept = default;

			base_function_view& operator=(std::nullptr_t) noexcept {
				reset();
				return *this;
			}

			base_function_view& operator=(const base_function_view& other) noexcept {
				_storage = other._storage;
				_parent	 = other._parent;
				_invoker = other._invoker;
				return *this;
			}

			base_function_view& operator=(base_function_view&& other) noexcept {
				_storage = other._storage;
				std::memset(&other._storage, 0, sizeof(function_storage));
				_parent	 = std::exchange(other._parent, nullptr);
				_invoker = std::exchange(other._invoker, nullptr);
				return *this;
			}

		#pragma region FREE_FUNCTIONS
		#if _MSTD_HAS_CXX20
			template<class FreeFunc>
			requires (is_valid_free_function_v<FreeFunc>)
		#else
			template<class FreeFunc, std::enable_if_t<is_valid_free_function_v<FreeFunc>, bool> = true>
		#endif
			base_function_view(FreeFunc&& freeFunc) noexcept : _invoker(&_free_func_invoker<FreeFunc>) {
				std::memset(&_storage, 0, sizeof(function_storage));
					if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
						_storage.func_ptr = reinterpret_cast<void (*)()>(freeFunc);
					}
					else { _storage.func_ptr = reinterpret_cast<void (*)()>(&freeFunc); }
			}

		#if _MSTD_HAS_CXX20
			template<class FreeFunc>
			requires (is_valid_free_function_v<FreeFunc>)
		#else
			template<class FreeFunc, std::enable_if_t<is_valid_free_function_v<FreeFunc>, bool> = true>
		#endif
			base_function_view& operator=(FreeFunc&& freeFunc) noexcept {
				_invoker = &_free_func_invoker<FreeFunc>;
				std::memset(&_storage, 0, sizeof(function_storage));
					if _MSTD_CONSTEXPR17 (is_function_ptr_v<FreeFunc>) {
						_storage.func_ptr = reinterpret_cast<void (*)()>(freeFunc);
					}
					else { _storage.func_ptr = reinterpret_cast<void (*)()>(&freeFunc); }
				return *this;
			}

		#pragma endregion

		#pragma region MEMBER_FUNCTIONS
		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view(function_parent_t<remove_cvref_t<MemberFunc> >* parentPtr, MemberFunc&& memberFunc) noexcept
				: _parent(parentPtr), _invoker(&_member_func_invoker<remove_cvref_t<MemberFunc> >) {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, &memberFunc, sizeof(MemberFunc));
			}

		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view(const function_parent_t<remove_cvref_t<MemberFunc> >* parentPtr, MemberFunc&& memberFunc) noexcept
				: _parent(parentPtr), _invoker(&_member_func_invoker<remove_cvref_t<MemberFunc> >) {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, memberFunc, sizeof(MemberFunc));
			}

		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view(std::pair<function_parent_t<remove_cvref_t<MemberFunc> >*, MemberFunc>&& pair) noexcept
				: _parent(pair.first), _invoker(&_member_func_invoker<remove_cvref_t<MemberFunc> >) {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, &pair.second, sizeof(MemberFunc));
			}

		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view(const std::pair<function_parent_t<remove_cvref_t<MemberFunc> >*, MemberFunc>& pair) noexcept
				: _parent(pair.first), _invoker(&_member_func_invoker<remove_cvref_t<MemberFunc> >) {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, &pair.second, sizeof(MemberFunc));
			}

		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view& operator=(
			  std::pair<function_parent_t<remove_cvref_t<MemberFunc> >*, MemberFunc>&& pair
			) noexcept {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, &pair.second, sizeof(MemberFunc));
				_parent	 = pair.first;
				_invoker = &_member_func_invoker<remove_cvref_t<MemberFunc> >;
				return *this;
			}

		#if _MSTD_HAS_CXX20
			template<class MemberFunc>
			requires (is_valid_member_function_v<MemberFunc>)
		#else
			template<class MemberFunc, std::enable_if_t<is_valid_member_function_v<MemberFunc>, bool> = true>
		#endif
			base_function_view& operator=(
			  const std::pair<function_parent_t<remove_cvref_t<MemberFunc> >*, MemberFunc>& pair
			) noexcept {
				std::memset(&_storage, 0, sizeof(function_storage));
				std::memcpy(_storage.mem_func_buffer, &pair.second, sizeof(MemberFunc));
				_parent	 = pair.first;
				_invoker = &_member_func_invoker<remove_cvref_t<MemberFunc> >;
				return *this;
			}

		#pragma endregion

		#pragma region FUNCTORS
		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, false>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, false>, bool> = true>
		#endif
			base_function_view(Functor& functor) noexcept : _invoker(&_functor_invoker<Functor>) {
				std::memset(&_storage, 0, sizeof(function_storage));
				_storage.functor_ptr = &functor;
			}

		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, true>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, true>, bool> = true>
		#endif
			base_function_view(const Functor& functor) noexcept : _invoker(&_functor_invoker<const Functor>) {
				std::memset(&_storage, 0, sizeof(function_storage));
				_storage.functor_ptr = &functor;
			}

		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, true>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, true>, bool> = true>
		#endif
			base_function_view(Functor&& functor) noexcept = delete;

		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, false>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, false>, bool> = true>
		#endif
			base_function_view& operator=(Functor& functor) noexcept {
				std::memset(&_storage, 0, sizeof(function_storage));
				_storage.functor_ptr = &functor;
				_invoker			 = &_functor_invoker<Functor>;
				return *this;
			}

		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, true>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, true>, bool> = true>
		#endif
			base_function_view& operator=(const Functor& functor) noexcept {
				std::memset(&_storage, 0, sizeof(function_storage));
				_storage.functor_ptr = &functor;
				_invoker			 = &_functor_invoker<const Functor>;
				return *this;
			}

		#if _MSTD_HAS_CXX20
			template<class Functor>
			requires (is_valid_functor_v<Functor, true>)
		#else
			template<class Functor, std::enable_if_t<is_valid_functor_v<Functor, true>, bool> = true>
		#endif
			base_function_view& operator=(Functor&& functor) noexcept = delete;
		#pragma endregion

			void reset() noexcept {
				std::memset(&_storage, 0, sizeof(function_storage));
				_parent	 = nullptr;
				_invoker = nullptr;
			}

		#if _MSTD_HAS_CXX20
			template<class... InvokeArgs>
			requires (std::is_invocable_v<F, InvokeArgs...>)
		#else
			template<class... InvokeArgs, std::enable_if_t<std::is_invocable_v<F, InvokeArgs...>, bool> = true>
		#endif
			[[nodiscard]] Ret invoke(InvokeArgs&&... args) const noexcept(is_noexcept_function_v<F>) {
				return _invoker(_storage, _parent, std::forward<InvokeArgs>(args)...);
			}

		#if _MSTD_HAS_CXX20
			template<class... InvokeArgs>
			requires (std::is_invocable_v<F, InvokeArgs...>)
		#else
			template<class... InvokeArgs, std::enable_if_t<std::is_invocable_v<F, InvokeArgs...>, bool> = true>
		#endif
			[[nodiscard]] Ret operator()(InvokeArgs&&... args) const noexcept(is_noexcept_function_v<F>) {
				return invoke(std::forward<InvokeArgs>(args)...);
			}

			[[nodiscard]] operator bool() const noexcept { return operator!=(nullptr); }

			[[nodiscard]] bool operator!() const noexcept { return !static_cast<bool>(*this); }

			[[nodiscard]] bool operator==(const base_function_view& other) const noexcept {
					if (_parent != other._parent) { return false; }
					if (_invoker != other._invoker) { return false; }

					if (_invoker == nullptr) { return true; }

				return std::memcmp(&_storage, &other._storage, sizeof(function_storage)) == 0;
			}

			[[nodiscard]] bool operator!=(const base_function_view& other) const noexcept
		#if _MSTD_HAS_CXX20
			  = default;
		#else
			{
				return !operator==(other);
			}
		#endif

			[[nodiscard]] bool operator==(std::nullptr_t) const noexcept { return _invoker == nullptr; }

			[[nodiscard]] bool operator!=(std::nullptr_t) const noexcept { return !operator==(nullptr); }
		};

		template<class F>
		using base_function_view_t = base_function_view<F, function_return_t<F>, function_args_t<F> >;

		template<class F>
		static _MSTD_CONSTEXPR17 bool is_valid_function_view_function_v =
		  mstd::is_function_v<F> && !is_const_function_v<F> && !is_volatile_function_v<F> && !is_parent_ref_function_v<F> &&
		  !is_parent_moved_function_v<F>;

		#if _MSTD_HAS_CXX20
		template<class F> concept valid_function_view_function = is_valid_function_view_function_v<F>;
		#endif
	} // namespace utils

		#if _MSTD_HAS_CXX20
	template<utils::valid_function_view_function F>
		#else
	template<class F, std::enable_if_t<utils::is_valid_function_view_function_v<F>, bool> = true>
		#endif
	class function_view : public utils::base_function_view_t<F> {
	private:
		using _base = utils::base_function_view_t<F>;

		template<class Func>
		static _MSTD_CONSTEXPR17 bool can_be_noexcept_v =
		  !is_noexcept_function_v<F> || (is_noexcept_function_v<F> && is_noexcept_function_v<Func>);

	public:
		function_view() noexcept = default;

		function_view(nullptr_t) noexcept : _base(nullptr) {}

		function_view(const function_view&) noexcept = default;
		function_view(function_view&&) noexcept		 = default;
		~function_view() noexcept override			 = default;

		#if _MSTD_HAS_CXX20
		template<class Func>
		requires (!std::is_same_v<mstd::remove_cvref_t<Func>, function_view> && can_be_noexcept_v<Func>)
		#else
		template<class Func,
		  std::enable_if_t<!std::is_same_v<mstd::remove_cvref_t<Func>, function_view> && can_be_noexcept_v<Func>, bool> = true>
		#endif
		function_view(Func&& func) noexcept : _base(func) {
		}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires (can_be_noexcept_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<can_be_noexcept_v<MemberFunc>, bool> = true>
		#endif
		function_view(function_parent_t<MemberFunc>* parentPtr, MemberFunc&& memberFunc) noexcept : _base(parentPtr, memberFunc) {
		}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires (can_be_noexcept_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<can_be_noexcept_v<MemberFunc>, bool> = true>
		#endif
		function_view(const function_parent_t<MemberFunc>* parentPtr, MemberFunc&& memberFunc) noexcept
			: _base(parentPtr, memberFunc) {
		}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires (can_be_noexcept_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<can_be_noexcept_v<MemberFunc>, bool> = true>
		#endif
		function_view(const std::pair<function_parent_t<MemberFunc>*, MemberFunc>& pair) noexcept : _base(pair) {
		}

		#if _MSTD_HAS_CXX20
		template<class MemberFunc>
		requires (can_be_noexcept_v<MemberFunc>)
		#else
		template<class MemberFunc, std::enable_if_t<can_be_noexcept_v<MemberFunc>, bool> = true>
		#endif
		function_view(std::pair<function_parent_t<MemberFunc>*, MemberFunc>&& pair) noexcept : _base(pair) {
		}

		function_view& operator=(const function_view&) noexcept = default;
		function_view& operator=(function_view&&) noexcept		= default;

		using _base::operator=;
	};
} // namespace mstd

	#endif
#endif