/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_LIBS_HPP_
	#define _MSTD_LIBS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/arithmetic_libs.hpp>
		#include <mstd/containers_libs.hpp>
		#include <mstd/functions_libs.hpp>
		#include <mstd/management_libs.hpp>
		#include <mstd/string_libs.hpp>
		#include <mstd/terminal_libs.hpp>

	#endif
#endif