/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "utils_libs.hpp"
#include "types.hpp"

namespace mstd {
#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<unsigned_integral _idT = size_t>
#else
	template<class _idT = size_t, std::enable_if_t<std::is_unsigned_v<_idT>, bool> = true>
#endif
	class id_manager {
	public:
		using id_type = _idT;

	private:
		id_type _nextId = 0;
		std::unordered_set<id_type> _removedIds = {};

		static constexpr id_type _maxIds = std::numeric_limits<id_type>::max();

		constexpr void _update_removed_ids() {
			if (_removedIds.empty()) return;

			auto& last = --_removedIds.end();
			while (*last == _nextId - 1) {
				--_nextId;
				_removedIds.erase(*last);

				if (_removedIds.empty()) return;

				last = --_removedIds.end();
			}
		}

	public:
		id_manager() noexcept = default;
		virtual ~id_manager() noexcept = default;

		[[nodiscard]] constexpr id_type get_next_id() {
			if (!_removedIds.empty()) {
				const id_type id = *_removedIds.begin();
				_removedIds.erase(id);
				return id;
			}

			if (_nextId == _maxIds) return bad_id();

			const id_type id = _nextId;
			++_nextId;
			return _nextId;
		}

		[[nodiscard]] constexpr bool return_id(const id_type& id) {
			if (id == bad_id() || id >= _nextId || _removedIds.find(id) != _removedIds.end()) {
				return false;
			}

			_removedIds.insert(id);
			_update_removed_ids();
			return true;
		}

		void reset() noexcept {
			_nextId = 0;
			_removedIds.clear();
		}

		static constexpr id_type bad_id() noexcept {
			return static_cast<id_type>(-1);
		}

		static constexpr id_type max_ids() noexcept {
			return _maxIds;
		}

		static constexpr id_type last_id() noexcept {
			return _maxIds - 1;
		}
	};

	using id8_manager = id_manager<uint8_t>;
	using id16_manager = id_manager<uint16_t>;
	using id32_manager = id_manager<uint32_t>;
	using id64_manager = id_manager<uint64_t>;
}