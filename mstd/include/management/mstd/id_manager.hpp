/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_ID_MANAGER_HPP_
#define _MSTD_ID_MANAGER_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING ("this is only available for c++17 and greater!");
#else

#include <mstd/types.hpp>

namespace mstd {
#if _MSTD_HAS_CXX20
	template <unsigned_integral _idT>
#else
	template <class _idT, std::enable_if_t<mstd::is_unsigned_integral_v<_idT>, bool>>
#endif
	class base_id_manager {
	public:
		using id_type = _idT;

	private:
		id_type _nextId = 0;
		std::set<id_type> _removedIds = {};

		static _MSTD_CONSTEXPR17 id_type _maxIds = std::numeric_limits<id_type>::max();

		_MSTD_CONSTEXPR20 void _update_removed_ids() {
			if (_removedIds.empty()) return;

			auto last = std::prev(_removedIds.end());
			while (*last == _nextId - 1) {
				--_nextId;
				_removedIds.erase(*last);

				if (_removedIds.empty()) return;

				last = std::prev(_removedIds.end());
			}
		}

	public:
		_MSTD_CONSTEXPR20 base_id_manager() noexcept = default;
		_MSTD_CONSTEXPR20 base_id_manager(const base_id_manager& other) noexcept = default;
		_MSTD_CONSTEXPR20 base_id_manager(base_id_manager&& other) noexcept = default;
		_MSTD_CONSTEXPR20 ~base_id_manager() noexcept = default;

		_MSTD_CONSTEXPR20 base_id_manager& operator=(const base_id_manager& other) noexcept = default;
		_MSTD_CONSTEXPR20 base_id_manager& operator=(base_id_manager&& other) noexcept = default;

		[[nodiscard]] _MSTD_CONSTEXPR20 id_type get_next_id() {
			if (!_removedIds.empty()) {
				const id_type id = *_removedIds.begin();
				_removedIds.erase(id);
				return id;
			}

			if (_nextId == _maxIds) return bad_id();

			const id_type id = _nextId;
			++_nextId;
			return id;
		}

		_MSTD_CONSTEXPR20 bool return_id(id_type id) {
#if _MSTD_HAS_CXX20
			if (id == bad_id() || id >= _nextId || _removedIds.contains(id)) {
#else
			if (id == bad_id() || id >= _nextId || _removedIds.find(id) != _removedIds.end()) {
#endif
				return false;
			}

			_removedIds.insert(id);
			_update_removed_ids();
			return true;
		}

		_MSTD_CONSTEXPR20 void reset() noexcept {
			_nextId = 0;
			_removedIds.clear();
		}

		[[nodiscard]] static _MSTD_CONSTEXPR20 id_type bad_id() noexcept {
			return ~static_cast<id_type>(0);
		}

		[[nodiscard]] static _MSTD_CONSTEXPR20 id_type max_ids() noexcept {
			return _maxIds;
		}

		[[nodiscard]] static _MSTD_CONSTEXPR20 id_type last_id() noexcept {
			return _maxIds - 1;
		}
	};
}
#endif
#endif