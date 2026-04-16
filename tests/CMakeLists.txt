project(mstd-tests VERSION 1.5.2 LANGUAGES CXX)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

enable_testing()

# region INSTALL_TESTING
if (MSTD_INSTALL_TEST)
    message("TESTING_INSTALL")
    find_package(mstd REQUIRED
            HINTS "${CMAKE_SOURCE_DIR}/dist"
            COMPONENTS ENUMS_MACROS CXX20 FOR_EACH_MACROS CLONE_FUNC_MACROS EXTRA_MACROS
    )

    add_library(mstd::all INTERFACE IMPORTED)
    target_link_libraries(mstd::all INTERFACE mstd::mstd mstd::ENUMS_MACROS mstd::CXX20 mstd::FOR_EACH_MACROS mstd::CLONE_FUNC_MACROS mstd::EXTRA_MACROS)
endif()
# endregion

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})
add_test(${PROJECT_NAME}_TEST ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE   $<IF:$<BOOL:${MSTD_INSTALL_TEST}>, mstd::all, mstd::mstd>
                                                gtest_main
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp")

# Optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:
            /O2
            /GL
            /fp:fast
        >
        $<$<CONFIG:Debug>:
            /RTC1
            /Zi
        >
        /Zc:preprocessor
        /Zc:__cplusplus
        /W4
        /WX
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:
            -O3
            -flto
        >
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
    >
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${MSTD_BUILD_COVERAGE}>>:
        --coverage
        -fcoverage-mcdc
        -fprofile-instr-generate
        -fcoverage-mapping
    >
)

target_link_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        -static
    >
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${MSTD_BUILD_COVERAGE}>>:
        -fprofile-instr-generate
    >
)