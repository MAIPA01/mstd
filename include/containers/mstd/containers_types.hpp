/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "containers_libs.hpp"

namespace mstd {
	template<class T>
	class ordered_set;

	template<class Key, class T>
	class ordered_map;

	template<class Key, class T, template<class, class, class...> class Map = std::map>
	class bimap;

	template<class Key, class T>
	using unordered_bimap = bimap<Key, T, std::unordered_map>;

	template<class Key, class T>
	using ordered_bimap = bimap<Key, T, ordered_map>;
}