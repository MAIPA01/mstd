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
#define MSTD_VERSION_MINOR 0
#define MSTD_VERSION_PATCH 0

#define MSTD_VERSION_TO_STRING(major, minor, patch) #major #minor #patch
#define MSTD_VERSION_TO_INT(major, minor, patch) (major * 100 + minor * 10 + patch)

#define MSTD_VERSION_STRING MSTD_VERSION_TO_STRING(MSTD_VERSION_MAJOR, \
												MSTD_VERSION_MINOR, \
												MSTD_VERSION_PATCH)
#define MSTD_VERSION_INT MSTD_VERSION_TO_INT(MSTD_VERSION_MAJOR, \
											MSTD_VERSION_MINOR, \
											MSTD_VERSION_PATCH)
#define MSTD_VERSION MSTD_VERSION_STRING
#pragma endregion

#pragma region LAST_UPDATE
#define MSTD_LAST_UPDATE_DAY 13
#define MSTD_LAST_UPDATE_MONTH 3
#define MSTD_LAST_UPDATE_YEAR 2025

#define MSTD_LAST_UPDATE_DATE "13.03.2025"
#pragma endregion