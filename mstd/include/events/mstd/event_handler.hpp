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
#include <mstd/id_manager.hpp>
#include <mstd/functions.hpp>

namespace mstd {
	template<template<class, class, class...> class EventsMap, class... Args>
	class base_event_handler {
		using id_type = size_t;
		using event_action_type = c_action_t<Args...>;
		using event_action_handler = function_view<event_action_type>;
		using events_type = EventsMap<id_type, event_action_handler>;
		using id_manager_type = base_id_manager<id_type>;

	private:
		events_type _events = {};
		id_manager_type _ids = {};

	public:
		_MSTD_CONSTEXPR20 base_event_handler() noexcept = default;
		_MSTD_CONSTEXPR20 ~base_event_handler() noexcept = default;

		[[nodiscard]] _MSTD_CONSTEXPR20 id_type add_callback(const event_action_handler& callback) {
			id_type id = _ids.get_next_id();
			if (id == id_manager_type::bad_id()) return id;

			_events[id] = callback;
			return id;
		}

		_MSTD_CONSTEXPR20 bool remove_callback(id_type callbackId) {
			auto itr = _events.find(callbackId);
			if (itr == _events.end()) {
				return false;
			}

			_events.erase(callbackId);
			return _ids.return_id(callbackId);
		}
		_MSTD_CONSTEXPR20 void remove_all_callbacks() {
			_events.clear();
			_ids.reset();
		}

		_MSTD_CONSTEXPR20 void invoke(Args&&... args) const {
			if (_events.empty()) return;

			// SAFETY WHEN CALLBACK DELETES ITSELF
			std::vector<event_action_handler> callbacks_to_run;
			callbacks_to_run.reserve(_events.size());

			for (const auto& [id, event] : _events) {
				callbacks_to_run.push_back(event);
			}

			for (const auto& callback : callbacks_to_run) {
				callback(std::forward<Args>(args)...);
			}
		}

#if _MSTD_HAS_CXX20
		template<same_function_as<event_action_type> F>
#else
		template<class F, std::enable_if_t<mstd::is_same_function_v<F, event_action_type>, bool> = true>
#endif
		_MSTD_CONSTEXPR20 id_type operator+=(const F& callback) {
			return add_callback(callback);
		}

		_MSTD_CONSTEXPR20 bool operator-=(id_type callbackId) {
			return remove_callback(callbackId);
		}

		_MSTD_CONSTEXPR20 void operator()(Args&&... args) const {
			invoke(std::forward<Args>(args)...);
		}
	};
}
#endif
#endif