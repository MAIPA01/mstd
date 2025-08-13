#pragma once
#include "utils_libs.hpp"

namespace mstd {
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

	template<class... Ns>
	struct are_arithmetic : all_check<std::is_arithmetic, Ns...> {};
	template<class... Ns> constexpr bool are_arithmetic_v = are_arithmetic<Ns...>::value;

	template<class... Ns>
	struct are_floating_points : all_check<std::is_floating_point, Ns...> {};
	template<class... Ns> constexpr bool are_floating_points_v = are_floating_points<Ns...>::value;

	template<class... Ns>
	struct are_integrals : all_check<std::is_integral, Ns...> {};
	template<class... Ns> constexpr bool are_integrals_v = are_integrals<Ns...>::value;

	template<class... Ns>
	struct are_signed : all_check<std::is_signed, Ns...> {};
	template<class... Ns> constexpr bool are_signed_v = are_signed<Ns...>::value;

	template<class... Ns>
	struct are_unsigned : all_check<std::is_unsigned, Ns...> {};
	template<class... Ns> constexpr bool are_unsigned_v = are_unsigned<Ns...>::value;

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<class T> concept arithmetic = std::is_arithmetic_v<T>;
	template<class T> concept floating_point = std::is_floating_point_v<T>;
	template<class T> concept integral = std::is_integral_v<T>;
	template<class T> concept signed_integral = std::is_signed_v<T>;
	template<class T> concept unsigned_integral = std::is_unsigned_v<T>;
#endif
#pragma endregion

#pragma region LOGIC_EXPRESIONS
	template<auto A, auto B>
	struct is_eq : std::bool_constant<(A == B)> {};
	template<auto A, auto B> constexpr bool is_eq_v = is_eq<A, B>::value;

	template<auto A, auto B>
	struct is_neq : std::bool_constant<(A != B)> {};
	template<auto A, auto B> constexpr bool is_neq_v = is_neq<A, B>::value;

	template<auto A, auto B>
	struct is_gt : std::bool_constant<(A > B)> {};
	template<auto A, auto B> constexpr bool is_gt_v = is_gt<A, B>::value;

	template<auto A, auto B>
	struct is_gteq : std::bool_constant<(A >= B)> {};
	template<auto A, auto B> constexpr bool is_gteq_v = is_gteq<A, B>::value;

	template<auto A, auto B>
	struct is_lt : std::bool_constant<(A < B)> {};
	template<auto A, auto B> constexpr bool is_lt_v = is_lt<A, B>::value;

	template<auto A, auto B>
	struct is_lteq : std::bool_constant<(A <= B)> {};
	template<auto A, auto B> constexpr bool is_lteq_v = is_lteq<A, B>::value;
#pragma endregion

#pragma region IS_IN
	template<template<class, class> class Cmp, class T, class U, class... Us> constexpr bool is_type_in_v = (Cmp<T, U>::value || (sizeof...(Us) > 0 ? (Cmp<T, Us>::value || ...) : true));
	template<template<auto, auto> class Cmp, auto A, auto B, auto... Cs> constexpr bool is_value_in_v = (Cmp<A, B>::value || (sizeof...(Cs) > 0 ? (Cmp<A, Cs>::value || ...) : true));

	template<class T, class U, class... Us> constexpr bool is_same_type_in_v = is_type_in_v<std::is_same, T, U, Us...>;
	template<auto A, auto B, auto... Cs> constexpr bool is_eq_value_in_v = is_value_in_v<is_eq, A, B, Cs...>;
#pragma endregion

#pragma region IN_RANGE
	template<auto A, auto Min, auto Max> constexpr bool is_in_range_v = (is_gteq_v<A, Min> && is_lteq_v<A, Max>);
#pragma endregion

#pragma region IS_BASED_ON
	template<class T, template<class...> class U> constexpr bool is_based_on_v = false;
	template<template<class...> class U, class... Vs> constexpr bool is_based_on_v<U<Vs...>, U> = true;
#pragma endregion

#pragma region UNIQUE_TYPES
	template<class... Ts> struct types_holder {
		static constexpr size_t types_num = sizeof...(Ts);
	};

	template<class T, class U> struct unique_impl {};
	template<class... Ts, class T, class... Us> struct unique_impl<types_holder<Ts...>, types_holder<T, Us...>> {
		using type = std::conditional_t<is_type_in_v<T, Ts...>,
			unique_impl<types_holder<Ts...>, types_holder<Us...>>,
			unique_impl<types_holder<Ts..., T>, types_holder<Us...>>>;
	};
	template<class... Ts> struct unique_impl<types_holder<Ts...>, types_holder<>> {
		using type = types_holder<Ts...>;
	};
	template<class... Ts> using unique_types = unique_impl<types_holder<>, types_holder<Ts...>>::type;
#pragma endregion
}