/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_FUNCTION_TRAITS_HPP_
	#define _MSTD_FUNCTION_TRAITS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/functions_libs.hpp>

namespace mstd {
	namespace utils {
		#pragma region DEFAULT_TYPE_TRAITS

		template<bool IsMember>
		struct function_default_type_traits {
			static _MSTD_CONSTEXPR17 const bool is_std_function = false;
			static _MSTD_CONSTEXPR17 const bool is_ptr			= IsMember;
			static _MSTD_CONSTEXPR17 const bool is_ref			= false;
			static _MSTD_CONSTEXPR17 const bool is_moved		= false;
			static _MSTD_CONSTEXPR17 const bool is_functor		= false;
			static _MSTD_CONSTEXPR17 const bool is_member		= IsMember;
			static _MSTD_CONSTEXPR17 const bool is_free			= !IsMember;

			static _MSTD_CONSTEXPR17 const bool is_parent_ref	= false;
			static _MSTD_CONSTEXPR17 const bool is_parent_moved = false;
			static _MSTD_CONSTEXPR17 const bool is_const		= false;
			static _MSTD_CONSTEXPR17 const bool is_noexcept		= false;
			static _MSTD_CONSTEXPR17 const bool is_volatile		= false;
		};

		#pragma endregion

		#pragma region CORE_TRAITS
		template<class F, class C = void>
		struct function_core_traits;

		template<class R, class... Args>
		struct function_core_traits<R(Args...), void> {
			using core_function_type					   = R(Args...);
			using std_function_type						   = std::function<R(Args...)>;

			using return_type							   = R;
			using decayed_return_type					   = std::decay_t<R>;

			using args_tuple							   = std::tuple<Args...>;
			using decayed_args_tuple					   = std::tuple<std::decay_t<Args>...>;
			static _MSTD_CONSTEXPR17 const size_t args_num = sizeof...(Args);

			template<size_t N>
			using arg_type = _MSTD_TYPENAME17 std::tuple_element_t<N, args_tuple>;
			template<size_t N>
			using decayed_arg_type = _MSTD_TYPENAME17 std::tuple_element_t<N, decayed_args_tuple>;
		};

		template<class R, class... Args, class C>
		struct function_core_traits<R(Args...), C> : utils::function_core_traits<R(Args...)> {
			using parent_type = C;
		};

		#pragma endregion
	} // namespace utils

		#pragma region FUNCTION_TRAITS

	template<class F, class C = void>
	struct function_traits : utils::function_default_type_traits<false> {
		static _MSTD_CONSTEXPR17 const bool is_free = false;
	};

		#pragma region STATIC

	// global/static
	template<class R, class... Args>
	struct function_traits<R(Args...)> : utils::function_core_traits<R(Args...)>,
										 utils::function_default_type_traits<false> {
		using function_type = R(Args...);
	};

	// global/static (noexcept)
	template<class R, class... Args>
	struct function_traits<R(Args...) noexcept> : function_traits<R(Args...)> {
		using function_type								= R(Args...) noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region MEMBER

	// member
	template<class R, class... Args, class C>
	struct function_traits<R(Args...), C> : utils::function_core_traits<R(Args...), C>,
											utils::function_default_type_traits<true> {
		using function_type = R(Args...);
	};

	// member (const)
	template<class R, class... Args, class C>
	struct function_traits<R(Args...) const, C> : function_traits<R(Args...), C> {
		using function_type							 = R(Args...) const;

		static _MSTD_CONSTEXPR17 const bool is_const = true;
	};

	// member (volatile)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile, C> : function_traits<R(Args...), C> {
		using function_type								= R(Args...) volatile;

		static _MSTD_CONSTEXPR17 const bool is_volatile = true;
	};

	// member (&)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) &, C> : function_traits<R(Args...), C> {
		using function_type								  = R(Args...) &;

		static _MSTD_CONSTEXPR17 const bool is_parent_ref = true;
	};

	// member (&&)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) &&, C> : function_traits<R(Args...), C> {
		using function_type									= R(Args...) &&;

		static _MSTD_CONSTEXPR17 const bool is_parent_moved = true;
	};

	// member (noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) noexcept, C> : function_traits<R(Args...), C> {
		using function_type								= R(Args...) noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma region CONST

	// member (const volatile)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile, C> : function_traits<R(Args...) const, C> {
		using function_type								= R(Args...) const volatile;

		static _MSTD_CONSTEXPR17 const bool is_volatile = true;
	};

	// member (const &)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const&, C> : function_traits<R(Args...) const, C> {
		using function_type								  = R(Args...) const&;

		static _MSTD_CONSTEXPR17 const bool is_parent_ref = true;
	};

	// member (const &&)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const&&, C> : function_traits<R(Args...) const, C> {
		using function_type									= R(Args...) const&&;

		static _MSTD_CONSTEXPR17 const bool is_parent_moved = true;
	};

	// member (const noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const noexcept, C> : function_traits<R(Args...) const, C> {
		using function_type								= R(Args...) const noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region CONST_VOLATILE

	// member (const volatile &)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile&, C> : function_traits<R(Args...) const volatile, C> {
		using function_type								  = R(Args...) const volatile&;

		static _MSTD_CONSTEXPR17 const bool is_parent_ref = true;
	};

	// member (const volatile &&)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile&&, C> : function_traits<R(Args...) const volatile, C> {
		using function_type									= R(Args...) const volatile&&;

		static _MSTD_CONSTEXPR17 const bool is_parent_moved = true;
	};

	// member (const volatile noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile noexcept, C>
		: function_traits<R(Args...) const volatile, C> {
		using function_type								= R(Args...) const volatile noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region CONST_VOLATILE_PARENT_REF

	// member (const volatile & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile & noexcept, C>
		: function_traits<R(Args...) const volatile&, C> {
		using function_type								= R(Args...) const volatile& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region CONST_VOLATILE_PARENT_MOVED

	// member (const volatile && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const volatile && noexcept, C>
		: function_traits<R(Args...) const volatile&&, C> {
		using function_type								= R(Args...) const volatile&& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region CONST_PARENT_REF

	// member (const & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const & noexcept, C> : function_traits<R(Args...) const&, C> {
		using function_type								= R(Args...) const& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region CONST_PARENT_MOVED

	// member (const && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) const && noexcept, C> : function_traits<R(Args...) const&&, C> {
		using function_type								= R(Args...) const&& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region VOLATILE

	// member (volatile &)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile&, C> : function_traits<R(Args...) volatile, C> {
		using function_type								  = R(Args...) volatile&;

		static _MSTD_CONSTEXPR17 const bool is_parent_ref = true;
	};

	// member (volatile &&)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile&&, C> : function_traits<R(Args...) volatile, C> {
		using function_type									= R(Args...) volatile&&;

		static _MSTD_CONSTEXPR17 const bool is_parent_moved = true;
	};

	// member (volatile noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile noexcept, C> : function_traits<R(Args...) volatile, C> {
		using function_type								= R(Args...) volatile noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region VOLATILE_PARENT_REF

	// member (volatile & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile & noexcept, C> : function_traits<R(Args...) volatile&, C> {
		using function_type								= R(Args...) volatile& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region VOLATILE_PARENT_MOVED

	// member (volatile && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) volatile && noexcept, C> : function_traits<R(Args...) volatile&&, C> {
		using function_type								= R(Args...) volatile&& noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region PARENT_REF

	// member (& noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) & noexcept, C> : function_traits<R(Args...) &, C> {
		using function_type								= R(Args...) & noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion

		#pragma region PARENT_MOVED

	// member (&& noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R(Args...) && noexcept, C> : function_traits<R(Args...) &&, C> {
		using function_type								= R(Args...) && noexcept;

		static _MSTD_CONSTEXPR17 const bool is_noexcept = true;
	};

		#pragma endregion
		#pragma endregion

		#pragma region STD_FUNCTION

	// for std::function
	template<class R, class... Args>
	struct function_traits<std::function<R(Args...)> > : function_traits<R(Args...)> {
		using function_type									= R(Args...);

		static _MSTD_CONSTEXPR17 const bool is_std_function = true;
		static _MSTD_CONSTEXPR17 const bool is_functor		= true;
		static _MSTD_CONSTEXPR17 const bool is_free			= false;
	};

		#pragma endregion

		#pragma region STATIC_POINTER

	// for static/global function pointer
	template<class R, class... Args>
	struct function_traits<R (*)(Args...)> : function_traits<R(Args...)> {
		static _MSTD_CONSTEXPR17 const bool is_ptr = true;
	};

	// for static/global function pointer (noexcept)
	template<class R, class... Args>
	struct function_traits<R (*)(Args...) noexcept> : function_traits<R(Args...) noexcept> {
		static _MSTD_CONSTEXPR17 const bool is_ptr = true;
	};

		#pragma endregion

		#pragma region STATIC_REFERENCE

	// for static/global function reference
	template<class R, class... Args>
	struct function_traits<R (&)(Args...)> : function_traits<R(Args...)> {
		static _MSTD_CONSTEXPR17 const bool is_ref = true;
	};

	// for static/global function reference (noexcept)
	template<class R, class... Args>
	struct function_traits<R (&)(Args...) noexcept> : function_traits<R(Args...) noexcept> {
		static _MSTD_CONSTEXPR17 const bool is_ref = true;
	};

		#pragma endregion

		#pragma region STATIC_MOVED

	// for static/global function moved
	template<class R, class... Args>
	struct function_traits<R (&&)(Args...)> : function_traits<R(Args...)> {
		static _MSTD_CONSTEXPR17 const bool is_moved = true;
	};

	// for static/global function moved (noexcept)
	template<class R, class... Args>
	struct function_traits<R (&&)(Args...) noexcept> : function_traits<R(Args...) noexcept> {
		static _MSTD_CONSTEXPR17 const bool is_moved = true;
	};

		#pragma endregion

		#pragma region LAMBDA_AND_FUNCTORS

	// for callable objects (lambdas, functors)
	template<class F>
	struct function_traits<F, std::void_t<decltype(&F::operator())> >
		: function_traits<decltype(&F::operator())> {
		static _MSTD_CONSTEXPR17 const bool is_ptr	   = false;
		static _MSTD_CONSTEXPR17 const bool is_functor = true;
		static _MSTD_CONSTEXPR17 const bool is_free	   = false;
	};

		#pragma endregion

		#pragma region MEMBER_POINTER

	// for member function pointer
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...)> : function_traits<R(Args...), C> {};

	// for member function pointer (const)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const> : function_traits<R(Args...) const, C> {};

	// for member function pointer (volatile)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile> : function_traits<R(Args...) volatile, C> {};

	// for member function pointer (&)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) &> : function_traits<R(Args...) &, C> {};

	// for member function pointer (&&)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) &&> : function_traits<R(Args...) &&, C> {};

	// for member function pointer (noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) noexcept> : function_traits<R(Args...) noexcept, C> {};

		#pragma region CONST

	// for member function pointers (const volatile)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile> : function_traits<R(Args...) const volatile, C> {
	};

	// for member function pointers (const &)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const&> : function_traits<R(Args...) const&, C> {};

	// for member function pointers (const &&)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const&&> : function_traits<R(Args...) const&&, C> {};

	// for member function pointers (const noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const noexcept> : function_traits<R(Args...) const noexcept, C> {
	};

		#pragma endregion

		#pragma region CONST_VOLATILE

	// for member function pointers (const volatile &)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile&>
		: function_traits<R(Args...) const volatile&, C> {};

	// for member function pointers (const volatile &&)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile&&>
		: function_traits<R(Args...) const volatile&&, C> {};

	// for member function pointers (const volatile noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile noexcept>
		: function_traits<R(Args...) const volatile noexcept, C> {};

		#pragma endregion

		#pragma region CONST_VOLATILE_PARENT_REF

	// for member function pointers (const volatile & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile & noexcept>
		: function_traits<R(Args...) const volatile & noexcept, C> {};

		#pragma endregion

		#pragma region CONST_VOLATILE_PARENT_MOVED

	// for member function pointers (const volatile && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const volatile && noexcept>
		: function_traits<R(Args...) const volatile && noexcept, C> {};

		#pragma endregion

		#pragma region CONST_PARENT_REF

	// for member function pointers (const & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const & noexcept>
		: function_traits<R(Args...) const & noexcept, C> {};

		#pragma endregion

		#pragma region CONST_PARENT_MOVED

	// for member function pointers (const && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) const && noexcept>
		: function_traits<R(Args...) const && noexcept, C> {};

		#pragma endregion

		#pragma region VOLATILE

	// for member function pointers (volatile &)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile&> : function_traits<R(Args...) volatile&, C> {};

	// for member function pointers (volatile &&)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile&&> : function_traits<R(Args...) volatile&&, C> {};

	// for member function pointers (volatile noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile noexcept>
		: function_traits<R(Args...) volatile noexcept, C> {};

		#pragma endregion

		#pragma region VOLATILE_PARENT_REF

	// for member function pointers (volatile & noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile & noexcept>
		: function_traits<R(Args...) volatile & noexcept, C> {};

		#pragma endregion

		#pragma region VOLATILE_PARENT_MOVED

	// for member function pointers (volatile && noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) volatile && noexcept>
		: function_traits<R (C::*)(Args...) volatile && noexcept, C> {};

		#pragma endregion

		#pragma region PARENT_REF

	// for member function pointers (& noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) & noexcept> : function_traits<R(Args...) & noexcept, C> {};

		#pragma endregion

		#pragma region PARENT_MOVED

	// for member function pointers (&& noexcept)
	template<class C, class R, class... Args>
	struct function_traits<R (C::*)(Args...) && noexcept> : function_traits<R(Args...) && noexcept, C> {};

		#pragma endregion
		#pragma endregion
		#pragma endregion

		#pragma region TYPE_TRAITS
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_std_function_v = function_traits<F>::is_std_function;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_function_ptr_v = function_traits<F>::is_ptr;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_function_ref_v = function_traits<F>::is_ref;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_function_moved_v = function_traits<F>::is_moved;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_functor_v = function_traits<F>::is_functor;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_member_function_v = function_traits<F>::is_member;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_free_function_v = function_traits<F>::is_free;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_parent_ref_function_v = function_traits<F>::is_parent_ref;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_parent_moved_function_v = function_traits<F>::is_parent_moved;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_const_function_v = function_traits<F>::is_const;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_noexcept_function_v = function_traits<F>::is_noexcept;
	template<class F>
	static _MSTD_CONSTEXPR17 const bool is_volatile_function_v = function_traits<F>::is_volatile;

		#if _MSTD_HAS_CXX20
	template<class F> concept function_ptr			= is_function_ptr_v<F>;
	template<class F> concept function_ref			= is_function_ref_v<F>;
	template<class F> concept function_moved		= is_function_moved_v<F>;
	template<class F> concept functor				= is_functor_v<F>;
	template<class F> concept member_function		= is_member_function_v<F>;
	template<class F> concept free_function			= is_free_function_v<F>;
	template<class F> concept parent_ref_function	= is_parent_ref_function_v<F>;
	template<class F> concept parent_moved_function = is_parent_moved_function_v<F>;
	template<class F> concept const_function		= is_const_function_v<F>;
	template<class F> concept noexcept_function		= is_noexcept_function_v<F>;
	template<class F> concept volatile_function		= is_volatile_function_v<F>;
		#endif
		#pragma endregion

		#pragma region CORE_TRAITS
	template<class F>
	using function_type_t = _MSTD_TYPENAME17 function_traits<F>::function_type;
	template<class F>
	using core_function_type_t = _MSTD_TYPENAME17 function_traits<F>::core_function_type;

	template<class F>
	using as_std_function_t = _MSTD_TYPENAME17 function_traits<F>::std_function_type;

	template<class F>
	using function_return_t = _MSTD_TYPENAME17 function_traits<F>::return_type;
	template<class F>
	using function_decayed_return_t = _MSTD_TYPENAME17 function_traits<F>::decayed_return_type;

	template<class F>
	using function_args_t = _MSTD_TYPENAME17 function_traits<F>::args_tuple;
	template<class F>
	using function_decayed_args_t = _MSTD_TYPENAME17 function_traits<F>::decayed_args_tuple;

	template<class F>
	static _MSTD_CONSTEXPR17 const size_t function_args_num_v = function_traits<F>::args_num;

	template<class F, size_t N>
	using function_arg_t = _MSTD_TYPENAME17 function_traits<F>::template arg_type<N>;
	template<class F, size_t N>
	using function_decayed_arg_t = _MSTD_TYPENAME17 function_traits<F>::template decayed_arg_type<N>;

	template<class F>
	using function_parent_t = _MSTD_TYPENAME17 function_traits<F>::parent_type;
		#pragma endregion

		#pragma region IS_CALLABLE

	// main template - default false
	template<class F, class = void>
	struct is_callable : std::false_type {};

	template<class F>
	struct is_callable<F, std::void_t<typename function_traits<F>::std_function_type> > : std::true_type {};

	// helper alias
	template<class F>
	_MSTD_CONSTEXPR17 bool is_callable_v = is_callable<F>::value;

		#if _MSTD_HAS_CXX20
	template<class F> concept callable = mstd::is_callable_v<F>;
		#endif
		#pragma endregion
} // namespace mstd
	#endif
#endif