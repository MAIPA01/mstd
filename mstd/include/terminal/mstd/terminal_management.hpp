/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_TERMINAL_MANAGEMENT_HPP_
	#define _MSTD_TERMINAL_MANAGEMENT_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/terminal_types.hpp>

// TODO: dodac kolorowanie

namespace mstd {
	inline void get_terminal_size(int& width, int& height) {
		#ifdef _WIN32
		width  = 0;
		height = 0;

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		#elif defined(__linux__) || defined(__APPLE__)
		struct winsize w;
		ioctl(fileno(stdout), TIOCGWINSZ, &w);
		width  = static_cast<int>(w.ws_col);
		height = static_cast<int>(w.ws_row);
		#endif
	}

	inline void clear_terminal() {
		#ifdef _WIN32
		system("cls"); // Windows: czysci ekran i historie
		#else
		system("clear && printf '\\e[3J'"); // Linux/macOS: czysci ekran i usuwa historie
		#endif
	}
} // namespace mstd

	#endif
#endif