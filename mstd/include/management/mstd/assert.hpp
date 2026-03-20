/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_ASSERT_HPP_
	#define _MSTD_ASSERT_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/assert_base.hpp>

		#pragma region MSTD_ASSERT
		#if _DEBUG
			// NOLINTNEXTLINE
			#define mstd_assert(expression, ...)                                     \
				MSTD_STOP_ASSERT_BASE(expression,                                    \
				  [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#elif !defined(MSTD_DISABLE_ASSERT_ON_RELEASE)
			// NOLINTNEXTLINE
			#define mstd_assert(expression, ...)                                     \
				MSTD_STOP_ASSERT_BASE(expression,                                    \
				  [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#else
			// NOLINTNEXTLINE
			#define mstd_assert(expression, ...)                                     \
				MSTD_EMPTY_ASSERT_BASE(expression,                                   \
				  [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
		#endif
		#pragma endregion

	#endif
#endif