/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <filesystem>
#include <string_view>
#include <functional>

#if _MSTD_HAS_CXX20
#include <format>
#include <source_location>
#else
#include <mstd/strconcat.hpp>
#endif

#if _DEBUG
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#define NOMINMAX
		#include <Windows.h>
		#include <crtdbg.h>
		#define _mstd_debugbreak() __debugbreak()
	#elif defined(__linux__) || defined(__APPLE__)
		#include <csignal>
		#define _mstd_debugbreak() __builtin_trap()
	#else
		#include <cstdlib>
		#define _mstd_debugbreak() std::abort()
	#endif
#else
	#define _mstd_debugbreak() ((void)0)
#endif

#pragma region ASSERT_DEFINES

#if _MSTD_HAS_CXX20
#define _MSTD_SOURCE_LOCATION_ARG const std::source_location& location
#define _MSTD_GET_SOURCE_FILE_NAME location.file_name()
#define _MSTD_GET_SOURCE_FUNC_NAME location.function_name()
#define _MSTD_GET_SOURCE_LINE location.line()
#define _MSTD_GET_SOURCE_COLUMN location.column()
#define _MSTD_PASS_SOURCE_LOCATION location
#define _MSTD_GET_CURRENT_SOURCE_LOCATION ::std::source_location::current()
#else
#define _MSTD_SOURCE_LOCATION_ARG const std::string_view& file_name, size_t line, const std::string_view& func_name
#define _MSTD_GET_SOURCE_FILE_NAME file_name.data()
#define _MSTD_GET_SOURCE_FUNC_NAME func_name.data()
#define _MSTD_GET_SOURCE_LINE line
#define _MSTD_GET_SOURCE_COLUMN 0
#define _MSTD_PASS_SOURCE_LOCATION file_name, line, func_name
#define _MSTD_GET_CURRENT_SOURCE_LOCATION __FILE__, __LINE__, __FUNCTION__
#endif

#pragma endregion // ASSERT_DEFINES

namespace mstd {
#pragma region FORMAT_LOG
	inline std::string format_log(const std::string_view& expression, _MSTD_SOURCE_LOCATION_ARG,
		const std::string_view& message = "") noexcept {
		const std::string shortFile = std::filesystem::path(_MSTD_GET_SOURCE_FILE_NAME).filename().string();
#if _MSTD_HAS_CXX20
		std::string fullMessage = std::format("Assertion failed [{}:{}:{},{}]: {}", shortFile, _MSTD_GET_SOURCE_FUNC_NAME,
			_MSTD_GET_SOURCE_LINE, _MSTD_GET_SOURCE_COLUMN, expression);
#else
		std::string fullMessage = mstd::concat("Assertion failed [", shortFile, ":", _MSTD_GET_SOURCE_FUNC_NAME, ":",
			std::to_string(_MSTD_GET_SOURCE_LINE), ", ", std::to_string(_MSTD_GET_SOURCE_COLUMN), "]: ", expression);
#endif

		if (!message.empty()) {
#if _MSTD_HAS_CXX20
			fullMessage += std::format("\nmessage: {}", message);
#else
			mstd::concat_to(fullMessage, "\nmessage: ", message);
#endif
		}
		return fullMessage;
	}
#pragma endregion // FORMAT_LOG

#pragma region REPORT_TO_SYSTEM
#if _DEBUG
	inline constexpr bool report_to_system(_MSTD_SOURCE_LOCATION_ARG, const std::string_view& message) noexcept {
#ifdef _WIN32
		return (_CrtDbgReport(_CRT_ASSERT, _MSTD_GET_SOURCE_FILE_NAME,
			_MSTD_GET_SOURCE_LINE, _MSTD_GET_SOURCE_FUNC_NAME, "%s", message.data()) == 1);
#else
		return true;
#endif // _WIN32
	}
#endif
#pragma endregion // REPORT_TO_SYSTEM

#pragma region ASSERT_HANDLER_NO_MSG
	inline void log_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _MSTD_SOURCE_LOCATION_ARG) noexcept {
		const std::string msg = format_log(expression, _MSTD_PASS_SOURCE_LOCATION);
		error_logger(msg);
	}

	inline bool stop_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _MSTD_SOURCE_LOCATION_ARG) noexcept {
		const std::string msg = format_log(expression, _MSTD_PASS_SOURCE_LOCATION);
		error_logger(msg);
#if _DEBUG
		return report_to_system(_MSTD_PASS_SOURCE_LOCATION, msg);
#else
		return true;
#endif
	}
#pragma endregion // ASSERT_HANDLER_NO_MSG

#pragma region ASSERT_HANDLER_MSG
	template<class... Args>
	inline void log_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _MSTD_SOURCE_LOCATION_ARG,
#if _MSTD_HAS_CXX20
		std::format_string<Args...> fmt, Args&&... args) noexcept {
		const std::string formattedMsg = std::format(fmt, std::forward<Args>(args)...);
#else
		Args&&... args) noexcept {
		const std::string formattedMsg = mstd::concat(args...);
#endif
		const std::string fullMsg = format_log(expression, _MSTD_PASS_SOURCE_LOCATION, formattedMsg);
		error_logger(fullMsg);
	}

	template<class... Args>
	inline bool stop_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _MSTD_SOURCE_LOCATION_ARG,
#if _MSTD_HAS_CXX20
		std::format_string<Args...> fmt, Args&&... args) noexcept {
		const std::string formattedMsg = std::format(fmt, std::forward<Args>(args)...);
#else
		Args&&... args) noexcept {
		const std::string formattedMsg = mstd::concat(args...);
#endif
		const std::string fullMsg = format_log(expression, _MSTD_PASS_SOURCE_LOCATION, formattedMsg);
		error_logger(fullMsg);
#if _DEBUG
		return report_to_system(_MSTD_PASS_SOURCE_LOCATION, fullMsg);
#else
		return true;
#endif
	}
#pragma endregion // ASSERT_HANDLER_MSG
}

#define mstd_stop_assert_base(expression, log_error_func, ...)\
		do {\
			if (!(expression)) {\
				if (::mstd::stop_assert_handler(#expression, log_error_func, _MSTD_GET_CURRENT_SOURCE_LOCATION __VA_OPT__(, ) __VA_ARGS__)) {\
					_mstd_debugbreak();\
				}\
			}\
		} while (0)

#define mstd_log_assert_base(expression, log_error_func, ...)\
		if (!(expression))\
			::mstd::log_assert_handler(#expression, log_error_func, _MSTD_GET_CURRENT_SOURCE_LOCATION __VA_OPT__(, ) __VA_ARGS__)

#define mstd_empty_assert_base(expression, log_error_func, ...) ((void)0)

#if _DEBUG
#define mstd_assert(expression, ...) mstd_stop_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#elif !MSTD_DISABLE_ASSERT_ON_RELEASE
#define mstd_assert(expression, ...) mstd_stop_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#else
#define mstd_assert(expression, ...) mstd_empty_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) __VA_ARGS__)
#endif

#endif