/*
* mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_EVENTS_TYPES_HPP_
	#define _MSTD_EVENTS_TYPES_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/events_utils.hpp>

namespace mstd {
	template<template<class, class, class...> class EventsMap, class... Args>
	class base_event_handler;

	template<template<class, class> class EventsMap>
	using method_base_event_handler = base_event_handler<EventsMap>;

	template<class... Args>
	using unordered_event_handler		 = base_event_handler<std::unordered_map, Args...>;
	using unordered_method_event_handler = unordered_event_handler<>;

	template<class... Args>
	using event_handler		   = base_event_handler<std::map, Args...>;
	using method_event_handler = event_handler<>;
} // namespace mstd

	#endif
#endif