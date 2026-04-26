/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_EVENT_HANDLER_HPP_
	#define _MSTD_EVENT_HANDLER_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/events_types.hpp>
		#include <mstd/functions.hpp>
		#include <mstd/stable_vector.hpp>

namespace mstd {
	template<class... Args>
	class event_handler {
	public:
		using event_type		   = void(Args&&...);
		using event_action_handler = action_t<Args&&...>;

		using events_type		   = stable_vector<event_action_handler>;

		using id_type			   = _MSTD_TYPENAME17 events_type::size_type;

	private:
		events_type _events = {};

	public:
		_MSTD_CONSTEXPR20 event_handler() noexcept	= default;
		_MSTD_CONSTEXPR20 ~event_handler() noexcept = default;

		_MSTD_CONSTEXPR20 id_type add_callback(const event_action_handler& callback) {
			_events.push_back(callback);
			return _events.back_id();
		}

		_MSTD_CONSTEXPR20 bool remove_callback(const id_type callbackId) {
				if (!_events.has_value(callbackId)) { return false; }
			_events.erase(callbackId);
			return true;
		}

		_MSTD_CONSTEXPR20 void remove_all_callbacks() { _events.clear(); }

		#if _MSTD_HAS_CXX20
		template<class... InvokeArgs>
		requires (std::is_invocable_v<event_type, InvokeArgs...>)
		#else
		template<class... InvokeArgs, std::enable_if_t<std::is_invocable_v<event_type, InvokeArgs...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void invoke(InvokeArgs&&... args) const {
				if (_events.empty()) { return; }

			// SAFETY WHEN CALLBACK DELETES ITSELF
			std::vector<event_action_handler> callbacksToRun;
			callbacksToRun.reserve(_events.size());
			callbacksToRun.insert(callbacksToRun.end(), _events.begin(), _events.end());

				for (const auto& callback : callbacksToRun) { callback(std::forward<InvokeArgs>(args)...); }
		}

		_MSTD_CONSTEXPR20 id_type operator+=(const event_action_handler& callback) { return add_callback(callback); }

		_MSTD_CONSTEXPR20 bool operator-=(const id_type callbackId) { return remove_callback(callbackId); }

		#if _MSTD_HAS_CXX20
		template<class... InvokeArgs>
		requires (std::is_invocable_v<event_type, InvokeArgs...>)
		#else
		template<class... InvokeArgs, std::enable_if_t<std::is_invocable_v<event_type, InvokeArgs...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void operator()(InvokeArgs&&... args) const {
			invoke(std::forward<InvokeArgs>(args)...);
		}
	};
} // namespace mstd
	#endif
#endif