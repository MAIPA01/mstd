/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#pragma region VERSION
#define MSTD_VERSION_MAJOR 1
#define MSTD_VERSION_MINOR 2
#define MSTD_VERSION_PATCH 7

#define _MSTD_STRINGIFY_HELPER(x) #x

#define _MSTD_VERSION_TO_STRING(major, minor, patch) _MSTD_STRINGIFY_HELPER(major)"."\
													 _MSTD_STRINGIFY_HELPER(minor)"."\
													 _MSTD_STRINGIFY_HELPER(patch)
#define _MSTD_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define MSTD_VERSION_STRING _MSTD_VERSION_TO_STRING(MSTD_VERSION_MAJOR, \
												MSTD_VERSION_MINOR, \
												MSTD_VERSION_PATCH)
#define MSTD_VERSION_INT _MSTD_VERSION_TO_INT(MSTD_VERSION_MAJOR, \
											MSTD_VERSION_MINOR, \
											MSTD_VERSION_PATCH)
#define MSTD_VERSION MSTD_VERSION_STRING
#pragma endregion

#pragma region LAST_UPDATE
#define MSTD_LAST_UPDATE_DAY 04
#define MSTD_LAST_UPDATE_MONTH 02
#define MSTD_LAST_UPDATE_YEAR 2026

#define _MSTD_LAST_UPDATE_DATE_HELPER(day, month, year) _MSTD_STRINGIFY_HELPER(day)"."\
															_MSTD_STRINGIFY_HELPER(month)"."\
															_MSTD_STRINGIFY_HELPER(year)

#define MSTD_LAST_UPDATE_DATE _MSTD_LAST_UPDATE_DATE_HELPER(MSTD_LAST_UPDATE_DAY, \
															MSTD_LAST_UPDATE_MONTH, \
														    MSTD_LAST_UPDATE_YEAR)
#pragma endregion