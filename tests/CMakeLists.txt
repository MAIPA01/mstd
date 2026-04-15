project(mstd-tests VERSION 1.5.1 LANGUAGES CXX)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

enable_testing()

# region INSTALL_TESTING
if (MSTD_INSTALL_TEST)
    find_package(mstd REQUIRED
            HINTS "${CMAKE_SOURCE_DIR}/dist"
    )

    message(STATUS FOUND: ${mstd_FOUND})
    message(STATUS SOURCE_DIR: ${mstd_SOURCE_DIR})
    target_compile_definitions(mstd::mstd INTERFACE MSTD_ENABLE_ENUMS_MACROS
                                                    MSTD_ENABLE_CXX20
                                                    MSTD_ENABLE_FOR_EACH_MACROS
                                                    MSTD_ENABLE_CLONE_FUNC_MACROS
                                                    MSTD_ENABLE_EXTRA_MACROS
    )
endif()
# endregion

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})
add_test(${PROJECT_NAME}_TEST ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE   mstd::mstd
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
        $<$<BOOL:${MSTD_BUILD_COVERAGE}>:
            -g
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >
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
#        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
        -static
        $<$<BOOL:${MSTD_BUILD_COVERAGE}>:-fprofile-instr-generate>
    >
)