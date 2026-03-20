/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */
#pragma once
#ifndef _MSTD_UTILS_HPP_
	#define _MSTD_UTILS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/arithmetic_utils.hpp>
		#include <mstd/containers_utils.hpp>
		#include <mstd/events_utils.hpp>
		#include <mstd/functions_utils.hpp>
		#include <mstd/libs.hpp>
		#include <mstd/management_utils.hpp>
		#include <mstd/string_utils.hpp>
		#include <mstd/terminal_utils.hpp>

	#endif
#endif