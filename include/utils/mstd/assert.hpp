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
#include <mstd/string.hpp>
#endif

#if _DEBUG
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#define NOMINMAX
		#include <Windows.h>
		#include <crtdbg.h>
		#define mstd_debugbreak() __debugbreak()
	#elif defined(__linux__) || defined(__APPLE__)
		#include <csignal>
		#define mstd_debugbreak() __builtin_trap()
	#else
		#include <cstdlib>
		#define mstd_debugbreak() std::abort()
	#endif
#endif

#pragma region ASSERT_DEFINES

#if _MSTD_HAS_CXX20
#define _mstd_assert_source_location_arg const std::source_location& location
#define _mstd_assert_get_file_name location.file_name()
#define _mstd_assert_get_func_name location.function_name()
#define _mstd_assert_get_line location.line()
#define _mstd_assert_get_column location.column()
#define _mstd_assert_pass_source_location location
#define _mstd_assert_get_location ::std::source_location::current()
#else
#define _mstd_assert_source_location_arg const std::string_view& file_name, size_t line, const std::string_view& func_name
#define _mstd_assert_get_file_name file_name
#define _mstd_assert_get_func_name func_name
#define _mstd_assert_get_line line
#define _mstd_assert_get_column 0
#define _mstd_assert_pass_source_location file_name, line, func_name
#define _mstd_assert_get_location __FILE__, __LINE__, __FUNCTION__
#endif

#pragma endregion // ASSERT_DEFINES

namespace mstd {
#pragma region FORMAT_LOG
	static inline std::string format_log(const std::string_view& expression, _mstd_assert_source_location_arg, 
		const std::string_view& message = "") noexcept {
		const std::string shortFile = std::filesystem::path(_mstd_assert_get_file_name).filename().string();
#if _MSTD_HAS_CXX20
		std::string fullMessage = std::format("Assertion failed [{}:{}:{},{}]: {}", shortFile, _mstd_assert_get_func_name,
			_mstd_assert_get_line, _mstd_assert_get_column, expression);
#else
		std::string fullMessage = mstd::concat("Assertion failed [", shortFile, ":", _mstd_assert_get_func_name, ":", 
			std::to_string(_mstd_assert_get_line), ", ", std::to_string(_mstd_assert_get_column), "]: ", expression);
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
	static constexpr bool report_to_system(_mstd_assert_source_location_arg, const std::string_view& message) noexcept {
#ifdef _WIN32
		return (_CrtDbgReport(_CRT_ASSERT, _mstd_assert_get_file_name,
			_mstd_assert_get_line, _mstd_assert_get_func_name, "%s", message.data()) == 1);
#else
		return true;
#endif // _WIN32
	}
#pragma endregion // REPORT_TO_SYSTEM

#pragma region ASSERT_HANDLER_NO_MSG
	static constexpr void log_assert_handler(const std::string_view& expression, 
		const std::function<void(const std::string_view&)>& error_logger, _mstd_assert_source_location_arg) noexcept {
		const std::string msg = format_log(expression, _mstd_assert_pass_source_location);
		error_logger(msg);
	}

	static constexpr bool stop_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _mstd_assert_source_location_arg) noexcept {
		const std::string msg = format_log(expression, _mstd_assert_pass_source_location);
		error_logger(msg);
		return report_to_system(_mstd_assert_pass_source_location, msg);
	}
#pragma endregion // ASSERT_HANDLER_NO_MSG

#pragma region ASSERT_HANDLER_MSG
#if _MSTD_HAS_CXX20
	template<class... Args>
#endif
	static constexpr void log_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _mstd_assert_source_location_arg,
#if _MSTD_HAS_CXX20
		std::format_string<Args...> fmt, Args&&... args) noexcept {
		const std::string formattedMsg = std::format(fmt, std::forward<Args>(args)...);
#else
		const std::string& formattedMsg = "") noexcept {
#endif
		const std::string fullMsg = format_log(expression, _mstd_assert_pass_source_location, formattedMsg);
		error_logger(fullMsg);
	}

#if _MSTD_HAS_CXX20
	template<class... Args>
#endif
	static constexpr bool stop_assert_handler(const std::string_view& expression,
		const std::function<void(const std::string_view&)>& error_logger, _mstd_assert_source_location_arg,
#if _MSTD_HAS_CXX20
		std::format_string<Args...> fmt, Args&&... args) noexcept {
		const std::string formattedMsg = std::format(fmt, std::forward<Args>(args)...);
#else
		const std::string& formattedMsg) noexcept {
#endif
		const std::string fullMsg = format_log(expression, _mstd_assert_pass_source_location, formattedMsg);
		error_logger(fullMsg);
		return report_to_system(_mstd_assert_pass_source_location, fullMsg);
	}
#pragma endregion // ASSERT_HANDLER_MSG
}

#if _MSTD_HAS_CXX20
#define _mstd_assert_va_args_pass(...) __VA_ARGS__
#elif defined(__GNUC__) || defined(__clang__)
#define _mstd_assert_va_args_pass(...) ##__VA_ARGS__
#elif defined(_MSC_VER) && !defined(_MSVC_LANG)
#define _mstd_assert_va_args_pass(...) __VA_ARGS__
#else
#define _mstd_assert_va_args_pass(...) __VA_ARGS__
#endif

#define mstd_stop_assert_base(expression, log_error_func, ...)\
		do {\
			if (!(expression)) {\
				if (::mstd::stop_assert_handler(#expression, log_error_func, _mstd_assert_get_location __VA_OPT__(, ) _mstd_assert_va_args_pass(__VA_ARGS__))) {\
					mstd_debugbreak();\
				}\
			}\
		} while (0)

#define mstd_log_assert_base(expression, log_error_func, ...)\
		if (!(expression))\
			::mstd::log_assert_handler(#expression, log_error_func, _mstd_assert_get_location __VA_OPT__(, ) _mstd_assert_va_args_pass(__VA_ARGS__))

#define mstd_empty_assert_base(expression, log_error_func, ...)

#if _DEBUG
#define mstd_assert(expression, ...) mstd_stop_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) _mstd_assert_va_args_pass(__VA_ARGS__))
#elif !_MSTD_DISABLE_ASSERT_ON_RELEASE
#define mstd_assert(expression, ...) mstd_stop_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) _mstd_assert_va_args_pass(__VA_ARGS__))
#else
#define mstd_assert(expression, ...) mstd_empty_assert_base(expression, [](const std::string_view&) -> void {} __VA_OPT__(, ) _mstd_assert_va_args_pass(__VA_ARGS__))
#endif

#endif