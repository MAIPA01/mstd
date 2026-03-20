/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_CONTAINERS_HPP_
	#define _MSTD_CONTAINERS_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/bimap.hpp>
		#include <mstd/containers_types.hpp>
		#include <mstd/ordered_map.hpp>
		#include <mstd/ordered_set.hpp>
		#include <mstd/stable_vector.hpp>

	#endif
#endif