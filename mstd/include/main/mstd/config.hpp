/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#pragma region VERSION
#define MSTD_VERSION_MAJOR 1
#define MSTD_VERSION_MINOR 4
#define MSTD_VERSION_PATCH 2

#define _MSTD_STRINGIFY_HELPER(x) #x

#define _MSTD_VERSION_TO_STRING(major, minor, patch) _MSTD_STRINGIFY_HELPER(major)"."\
													 _MSTD_STRINGIFY_HELPER(minor)"."\
													 _MSTD_STRINGIFY_HELPER(patch)
#define _MSTD_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define MSTD_VERSION_STRING _MSTD_VERSION_TO_STRING(MSTD_VERSION_MAJOR, \
												MSTD_VERSION_MINOR, \
												MSTD_VERSION_PATCH)
#define MSTD_VERSION_INT _MSTD_VERSION_TO_INT(MSTD_VERSION_MAJOR, \
											MSTD_VERSION_MINOR, \
											MSTD_VERSION_PATCH)
#define MSTD_VERSION MSTD_VERSION_STRING
#pragma endregion

#pragma region LAST_UPDATE
#define MSTD_LAST_UPDATE_DAY 13
#define MSTD_LAST_UPDATE_MONTH 03
#define MSTD_LAST_UPDATE_YEAR 2026

#define _MSTD_LAST_UPDATE_DATE_HELPER(day, month, year) _MSTD_STRINGIFY_HELPER(day)"."\
															_MSTD_STRINGIFY_HELPER(month)"."\
															_MSTD_STRINGIFY_HELPER(year)

#define MSTD_LAST_UPDATE_DATE _MSTD_LAST_UPDATE_DATE_HELPER(MSTD_LAST_UPDATE_DAY, \
															MSTD_LAST_UPDATE_MONTH, \
														    MSTD_LAST_UPDATE_YEAR)
#pragma endregion

#pragma region DEFAULT_MACRO_VALUES
#ifndef MSTD_ENABLE_CXX20
	#define MSTD_ENABLE_CXX20 0
#endif

#ifndef MSTD_DISABLE_ASSERT_ON_RELEASE
	#define MSTD_DISABLE_ASSERT_ON_RELEASE 0
#endif
#pragma endregion

#pragma region VERSION_CHECKS
#ifndef _HAS_CXX17
	#define _MSTD_HAS_CXX17 __cplusplus >= 201703L
#else
	#define _MSTD_HAS_CXX17 _HAS_CXX17
#endif

#ifndef _HAS_CXX20
	#define _MSTD_HAS_CXX20 __cplusplus >= 202002L && MSTD_ENABLE_CXX20
#else
	#define _MSTD_HAS_CXX20 _HAS_CXX20 && MSTD_ENABLE_CXX20
#endif
#pragma endregion

#pragma region PRAGMA_MESSAGES
#define _MSTD_STRINGIZE_(S) #S
#define _MSTD_STRINGIZE(S)  _MSTD_STRINGIZE_(S)

#define _MSTD_PRAGMA(PRAGMA)          _Pragma(#PRAGMA)
#define _MSTD_PRAGMA_MESSAGE(MESSAGE) _MSTD_PRAGMA(message(MESSAGE))
#define _MSTD_MESSAGE(MESSAGE)   _MSTD_PRAGMA_MESSAGE(__FILE__ "(" _MSTD_STRINGIZE(__LINE__) "): " MESSAGE)

#define _MSTD_WARNING(MESSAGE) _MSTD_MESSAGE("warning: " MESSAGE)
#define _MSTD_ERROR(MESSAGE)   static_assert(false, "error: " MESSAGE)
#pragma endregion

#pragma region VERSION_DEPENDENT
#if _MSTD_HAS_CXX17
	#define _MSTD_CONSTEXPR17 constexpr
#else
	#define _MSTD_CONSTEXPR17
#endif

#if _MSTD_HAS_CXX20
	#define _MSTD_CONSTEXPR20 constexpr
	#define _MSTD_RETURN_VALUE_IF(condition, ...) __VA_ARGS__
	#define _MSTD_REQUIRES(condition) requires (condition)
	#define _MSTD_ENABLE_IF_TEMPLATE(class_name, condition)
#else
	#define _MSTD_CONSTEXPR20
	#define _MSTD_RETURN_VALUE_IF(condition, ...) typename std::enable_if_t<(condition), __VA_ARGS__>
	#define _MSTD_REQUIRES(condition)
	#define _MSTD_ENABLE_IF_TEMPLATE(class_name, condition) template<class class_name, std::enable_if_t<(condition), bool> = true>
#endif
#pragma endregion