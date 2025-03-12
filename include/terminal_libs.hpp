#pragma once
#include <cstdlib>
#include <algorithm>
#include <iostream>

#if defined(_WIN32)
	#define VC_EXTRALEAN
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN

	#include <Windows.h>

	#undef VC_EXTRALEAN
	#undef NOMINMAX
	#undef WIN32_LEAN_AND_MEAN
#elif defined(__linux__) || defined(__APPLE__)
	#include <sys/ioctl.h>
#endif // Windows/Linux