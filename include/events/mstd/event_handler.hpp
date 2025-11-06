#pragma once

#include "events_libs.hpp"

namespace mstd {
	template<class Ret, class... Args> using func = std::function<Ret(Args...)>;
	template<class... Args> using action = func<void, Args...>;
	using method = action<>;

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
	template<class F, class AT>
	concept event_action_func = mstd::is_same_function_v<F, AT>;
#endif

	template<typename... Args>
	class event_handler {
		using id_type = size_t;
		using event_action_type = action<Args...>;
		using events_type = std::unordered_map<id_type, event_action_type>;
		using id_manager_type = id_manager<id_type>;

	private:
		events_type _events = {};
		id_manager_type _ids = {};

	public:
		event_handler() noexcept = default;
		virtual ~event_handler() noexcept = default;

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
		template<event_action_func<event_action_type> F>
#else
		template<class F, std::enable_if_t<mstd::is_same_function_v<F, event_action_type>, bool> = true>
#endif
		constexpr id_type add_callback(const F& callback) {
			id_type id = _ids.get_next_id();
			if (id == id_manager_type::bad_id()) return id;

			_events[id] = callback;
			return id;
		}

		constexpr bool remove_callback(const id_type& callbackId) {
			auto itr = _events.find(callbackId);
			if (itr == _events.end()) {
				return false;
			}

			_events.erase(callbackId);
			_ids.return_id(callbackId);
			return true;
		}
		constexpr void remove_all_callbacks() {
			_events.clear();
			_ids.reset();
		}

		void invoke(Args... args) const {
			for (const auto& [id, event] : _events) {
				event.action(args...);
			}
		}

#if _HAS_CXX20 && _MSTD_ENABLE_CXX20
		template<event_action_func<event_action_type> F>
#else
		template<class F, std::enable_if_t<mstd::is_same_function_v<F, event_action_type>, bool> = true>
#endif
		constexpr id_type operator+=(const F& callback) {
			return add_callback(callback);
		}

		constexpr bool operator-=(id_type callbackId) {
			return remove_callback(callbackId);
		}

		constexpr void operator()(Args... args) const {
			invoke(args...);
		}
	};
	using method_event_handler = event_handler<>;
}