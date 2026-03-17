/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */
#pragma once
#ifndef _MSTD_MSTD_HPP_
#define _MSTD_MSTD_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else
	
// #define MSTD_ENABLE_CXX20				-> enables c++20 features
// #define MSTD_DIABLE_ASSERT_ON_RELEASE	-> makes every assert in this library (void)0 on release builds
// #define MSTD_USE_FOR_EACH_MACROS			-> enables FOR_EACH macros
// #define MSTD_USE_ENUMS_MACROS			-> enables ENUM macros
// #define MSTD_USE_CLONE_FUNC_MACROS		-> enables CLONE_FUNC macros
// #define MSTD_USE_EXTRA_MACROS			-> enables external macros

#include <mstd/utils.hpp>
#include <mstd/macros.hpp>
#include <mstd/functions.hpp>
#include <mstd/arithmetic.hpp>
#include <mstd/containers.hpp>
#include <mstd/terminal.hpp>
#include <mstd/string.hpp>
#include <mstd/events.hpp>
#include <mstd/utils.hpp>

#endif
#endif