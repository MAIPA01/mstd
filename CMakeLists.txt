cmake_minimum_required(VERSION 3.30)
project(mstd VERSION 1.4.3 LANGUAGES CXX)

option(BUILD_MSTD_TESTS 				"Build mstd tests" 				OFF)
option(BUILD_MSTD_COVERAGE 				"Enable coverage reporting" 	OFF)
option(BUILD_MSTD_DOCUMENTATION 		"Build documentation" 			OFF)

option(MSTD_USE_EXTERNAL_FMT_FORMAT 	"Enable if you want to use own fmt library" 					OFF)
option(MSTD_ENABLE_CXX20 				"enables c++20 features" 										OFF)
option(MSTD_DISABLE_ASSERT_ON_RELEASE 	"makes every assert in this library (void)0 on release builds" 	OFF)
option(MSTD_ENABLE_FOR_EACH_MACROS 		"enables FOR_EACH macros" 										OFF)
option(MSTD_ENABLE_ENUMS_MACROS 		"enables ENUM macros" 											OFF)
option(MSTD_ENABLE_CLONE_FUNC_MACROS 	"enables CLONE_FUNC macros" 									OFF)
option(MSTD_ENABLE_EXTRA_MACROS 		"enables external macros" 										OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
if (${BUILD_MSTD_DOCUMENTATION})
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mstd)

if(BUILD_MSTD_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (BUILD_MSTD_COVERAGE)
    setup_coverage_report(
		TEST_PROJECT mstd-tests
		FORMAT html
		OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
		CLEAN_TEMP
	)
endif()