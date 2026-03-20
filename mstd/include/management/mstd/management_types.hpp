/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_MANAGEMENT_TYPES_HPP_
#define _MSTD_MANAGEMENT_TYPES_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/management_utils.hpp>

namespace mstd {
	#pragma region ARE_ALL
	template<template<class> class Test, class... Ts>
	static _MSTD_CONSTEXPR17 const bool are_all_v = (Test<Ts>::value && ...);
	#pragma endregion

	#pragma region INDEX_SEQUENCE_FROM_TO
	template<size_t Start, size_t... Indices>
	_MSTD_CONSTEXPR20 std::index_sequence<(Start + Indices)...> shift_index_sequence(const std::index_sequence<Indices...>&) {
		return {};
	}

	template<size_t Start, size_t End>
	using make_index_sequence_from_to = decltype(shift_index_sequence<Start>(std::make_index_sequence<End - Start>()));

	template<size_t Start, size_t Size>
	using make_index_sequence_from = decltype(shift_index_sequence<Start>(std::make_index_sequence<Size>()));

	template<size_t Start, class... Ts>
	using make_index_sequence_for_from = decltype(shift_index_sequence<Start>(std::index_sequence_for<Ts...>()));
	#pragma endregion

#pragma region UNIVERSAL_CHECKS
	template<template<class> class Check, class... Ts>
	struct all_check : std::bool_constant<(Check<Ts>::value && ...)> {};
	template<template<class> class Check, class... Ts>
	constexpr bool all_check_v = all_check<Check, Ts...>::value;

	template<template<class> class Check, class... Ts>
	struct any_check : std::bool_constant<(Check<Ts>::value || ...)> {};
	template<template<class> class Check, class... Ts>
	constexpr bool any_check_v = any_check<Check, Ts...>::value;
#pragma endregion

#pragma region NUMERIC_TYPES

	template<class T>
	constexpr bool is_signed_integral_v = std::is_integral_v<T> && std::is_signed_v<T>;
	template<class T> struct is_signed_integral : std::bool_constant<is_signed_integral_v<T>> {};
	template<class T>
	constexpr bool is_unsigned_integral_v = std::is_integral_v<T> && std::is_unsigned_v<T>;
	template<class T> struct is_unsigned_integral : std::bool_constant<is_unsigned_integral_v<T>> {};

	template<class... Ns>
	struct are_arithmetic : all_check<std::is_arithmetic, Ns...> {};
	template<class... Ns> constexpr bool are_arithmetic_v = are_arithmetic<Ns...>::value;

	template<class... Ns>
	struct are_signed : all_check<std::is_signed, Ns...> {};
	template<class... Ns> constexpr bool are_signed_v = are_signed<Ns...>::value;

	template<class... Ns>
	struct are_unsigned : all_check<std::is_unsigned, Ns...> {};
	template<class... Ns> constexpr bool are_unsigned_v = are_unsigned<Ns...>::value;

	template<class... Ns>
	struct are_floating_points : all_check<std::is_floating_point, Ns...> {};
	template<class... Ns> constexpr bool are_floating_points_v = are_floating_points<Ns...>::value;

	template<class... Ns>
	struct are_integrals : all_check<std::is_integral, Ns...> {};
	template<class... Ns> constexpr bool are_integrals_v = are_integrals<Ns...>::value;

	template<class... Ns>
	struct are_signed_integrals : all_check<mstd::is_signed_integral, Ns...> {};
	template<class... Ns> constexpr bool are_signed_integrals_v = are_signed_integrals<Ns...>::value;

	template<class... Ns>
	struct are_unsigned_integrals : all_check<mstd::is_unsigned_integral, Ns...> {};
	template<class... Ns> constexpr bool are_unsigned_integrals_v = are_unsigned_integrals<Ns...>::value;

#if _MSTD_HAS_CXX20
	template<class T> concept arithmetic = std::is_arithmetic_v<T>;
	template<class T> concept signed_arithmetic = std::is_signed_v<T>;
	template<class T> concept unsigned_arithmetic = std::is_unsigned_v<T>;
	template<class T> concept floating_point = std::is_floating_point_v<T>;
	template<class T> concept integral = std::is_integral_v<T>;
	template<class T> concept signed_integral = mstd::is_signed_integral_v<T>;
	template<class T> concept unsigned_integral = mstd::is_unsigned_integral_v<T>;
#endif
#pragma endregion

#pragma region COMPARE_ARITHMETIC
	namespace utils {
		template<auto A>
		struct abs_impl : std::conditional_t<
			std::is_unsigned_v<decltype(A)>,
			std::integral_constant<decltype(A), A>,
			std::integral_constant<decltype(A), (A > 0 ? A : -A)>
		> {};
	}

	template<auto A>
	static _MSTD_CONSTEXPR17 const auto abs_v = utils::abs_impl<A>::value;

#if _MSTD_HAS_CXX20
	template<auto A, auto B, auto Eps, arithmetic AT = decltype(A), arithmetic BT = decltype(B), floating_point EpsT = decltype(Eps)>
	_MSTD_CONSTEXPR17 const bool is_eq_arithmetic_v = (std::is_floating_point_v<AT> || std::is_floating_point_v<BT>) ?
		(abs_v<A - B> < Eps) :
		(A == B);
#else
	template<auto A, auto B, class AT = decltype(A), class BT = decltype(B),
	std::enable_if_t<(std::is_arithmetic_v<AT> && std::is_arithmetic_v<BT>), bool> = true>
	_MSTD_CONSTEXPR17 const bool is_eq_arithmetic_v = (A == B);
#endif

#if _MSTD_HAS_CXX20
	template<auto A, auto B, auto Eps, arithmetic AT = decltype(A), arithmetic BT = decltype(B), floating_point EpsT = decltype(Eps)>
	_MSTD_CONSTEXPR17 const bool is_neq_arithmetic_v = !is_eq_arithmetic_v<A, B, Eps>;
#else
	template<auto A, auto B, class AT = decltype(A), class BT = decltype(B),
		std::enable_if_t<(std::is_arithmetic_v<AT> && std::is_arithmetic_v<BT>), bool> = true>
	_MSTD_CONSTEXPR17 const bool is_neq_arithmetic_v = !is_eq_arithmetic_v<A, B>;
#endif

#pragma endregion

#pragma region LOGIC_EXPRESIONS
	template<auto A, auto B>
	struct is_eq : std::bool_constant<(A == B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_eq_v = is_eq<A, B>::value;

	template<auto A, auto B>
	struct is_neq : std::bool_constant<(A != B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_neq_v = is_neq<A, B>::value;

	template<auto A, auto B>
	struct is_gt : std::bool_constant<(A > B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_gt_v = is_gt<A, B>::value;

	template<auto A, auto B>
	struct is_gteq : std::bool_constant<(A >= B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_gteq_v = is_gteq<A, B>::value;

	template<auto A, auto B>
	struct is_lt : std::bool_constant<(A < B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_lt_v = is_lt<A, B>::value;

	template<auto A, auto B>
	struct is_lteq : std::bool_constant<(A <= B)> {};
	template<auto A, auto B> _MSTD_CONSTEXPR17 const bool is_lteq_v = is_lteq<A, B>::value;
#pragma endregion

#pragma region IS_IN
	template<template<class, class> class Cmp, class T, class U, class... Us> 
	_MSTD_CONSTEXPR17 const bool is_type_in_v = (Cmp<T, U>::value || (sizeof...(Us) > 0 ? (Cmp<T, Us>::value || ...) : false));

	template<template<auto, auto> class Cmp, auto A, auto B, auto... Cs> 
	_MSTD_CONSTEXPR17 const bool is_value_in_v = (Cmp<A, B>::value || (sizeof...(Cs) > 0 ? (Cmp<A, Cs>::value || ...) : false));

	template<class T, class U, class... Us> _MSTD_CONSTEXPR17 const bool is_same_type_in_v = is_type_in_v<std::is_same, T, U, Us...>;
	template<auto A, auto B, auto... Cs> _MSTD_CONSTEXPR17 const bool is_eq_value_in_v = is_value_in_v<is_eq, A, B, Cs...>;
#pragma endregion

#pragma region IN_RANGE
	template<auto A, auto Min, auto Max> _MSTD_CONSTEXPR17 const bool is_in_range_v = (is_gteq_v<A, Min> && is_lteq_v<A, Max>);
#pragma endregion

#pragma region IS_BASED_ON
	template<class T, template<class...> class U> _MSTD_CONSTEXPR17 const bool is_based_on_v = false;
	template<template<class...> class U, class... Vs> _MSTD_CONSTEXPR17 const bool is_based_on_v<U<Vs...>, U> = true;
#pragma endregion

#pragma region UNIQUE_TYPES
	template<class... Ts> struct types_holder {
		using as_tuple = std::tuple<Ts...>;
		static _MSTD_CONSTEXPR17 const size_t types_num = sizeof...(Ts);
	};

	namespace utils {
		template<class T, class U>
		struct unique_impl {};

		template<class T, class... Us>
		struct unique_impl<types_holder<>, types_holder<T, Us...>>
			: unique_impl<types_holder<T>, types_holder<Us...>> {
		};

		template<class... Ts, class T, class... Us>
		struct unique_impl<types_holder<Ts...>, types_holder<T, Us...>>
			: std::conditional_t<is_same_type_in_v<T, Ts...>,
			unique_impl<types_holder<Ts...>, types_holder<Us...>>,
			unique_impl<types_holder<Ts..., T>, types_holder<Us...>>> {
		};

		template<class... Ts> struct unique_impl<types_holder<Ts...>, types_holder<>> {
			using type = types_holder<Ts...>;
		};
	}

	template<class... Ts> using unique_types = _MSTD_TYPENAME17 utils::unique_impl<types_holder<>, types_holder<Ts...>>::type;
#pragma endregion

	#pragma region IF
	namespace utils {
		template<bool Condition, auto TrueValue, auto FalseValue>
		struct if_impl {
			static _MSTD_CONSTEXPR17 const auto value = FalseValue;
		};

		template<auto TrueValue, auto FalseValue>
		struct if_impl<true, TrueValue, FalseValue> {
			static _MSTD_CONSTEXPR17 const auto value = TrueValue;
		};
	}

	template<bool Condition, auto TrueValue, auto FalseValue>
	static _MSTD_CONSTEXPR17 const auto if_v = utils::if_impl<Condition, TrueValue, FalseValue>::value;
	#pragma endregion

	#pragma region ID_MANAGER
	#if _MSTD_HAS_CXX20
	template <unsigned_integral IdT>
	#else
	template <class IdT, std::enable_if_t<mstd::is_unsigned_integral_v<IdT>, bool> = true>
	#endif
	class base_id_manager;

	using id_manager = base_id_manager<size_t>;
	using id8_manager = base_id_manager<uint8_t>;
	using id16_manager = base_id_manager<uint16_t>;
	using id32_manager = base_id_manager<uint32_t>;
	using id64_manager = base_id_manager<uint64_t>;
	#pragma endregion
}

#endif
#endif