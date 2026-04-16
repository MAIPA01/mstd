cmake_minimum_required(VERSION 3.30)
project(mstd VERSION 1.5.2 LANGUAGES CXX)

option(MSTD_BUILD_TESTS 				"Build mstd tests" 				${PROJECT_IS_TOP_LEVEL})
option(MSTD_BUILD_COVERAGE 				"Enable coverage reporting"		${PROJECT_IS_TOP_LEVEL})
option(MSTD_BUILD_DOCUMENTATION 		"Build documentation" 			${PROJECT_IS_TOP_LEVEL})

option(MSTD_ENABLE_CLANG_TIDY 			"Enables clang-tidy check"		${PROJECT_IS_TOP_LEVEL})

option(MSTD_INSTALL 					"Enables installation of this project"					${PROJECT_IS_TOP_LEVEL})
option(MSTD_INSTALL_TEST				"This is only to test if installation of mstd works" 	OFF)

option(MSTD_FMT_EXTERNAL 				"Enable if you want to use own fmt library (tested and compatible fmt version: 12.1.0)"	OFF)

option(MSTD_ENABLE_CXX20 				"enables c++20 features" 																OFF)
option(MSTD_DISABLE_ASSERT_ON_RELEASE 	"makes every assert in this library (void)0 on release builds" 							OFF)
option(MSTD_ENABLE_FOR_EACH_MACROS 		"enables FOR_EACH macros" 																OFF)
option(MSTD_ENABLE_ENUMS_MACROS 		"enables ENUM macros" 																	OFF)
option(MSTD_ENABLE_CLONE_FUNC_MACROS 	"enables CLONE_FUNC macros" 															OFF)
option(MSTD_ENABLE_EXTRA_MACROS 		"enables external macros" 																OFF)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

if(MSTD_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()

if (MSTD_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if (NOT MSTD_INSTALL_TEST)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mstd)
endif()

if(MSTD_BUILD_TESTS OR MSTD_BUILD_COVERAGE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (MSTD_BUILD_COVERAGE)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

    setup_coverage_report(
		TEST_TARGET mstd-tests
		FORMAT html
		OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
		CLEAN_TEMP
	)
endif()