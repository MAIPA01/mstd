cmake_minimum_required(VERSION 3.30)
project(mstd VERSION 1.4.0 LANGUAGES CXX)

option(BUILD_MSTD_TESTS "Build mstd tests" OFF)
option(BUILD_MSTD_COVERAGE "Enable coverage reporting" OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

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