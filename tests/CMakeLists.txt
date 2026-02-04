project(mstd-tests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(MSTD_ENABLE_CXX20 ON CACHE INTERNAL "" FORCE)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})

target_link_libraries(${PROJECT_NAME} PRIVATE mstd gtest_main)

# Konfiguracja PCH
set(PCH_HEADER "${CMAKE_SOURCE_DIR}/pch.hpp")
if(EXISTS ${PCH_HEADER})
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})
endif()

# Optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Release>:/fp:fast>
        $<$<CONFIG:Debug>:/RTC1>
        $<$<CONFIG:Debug>:/Zi>
        /Zc:preprocessor
        /Zc:__cplusplus
        /W4
        /WX
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-flto>
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
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
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
        -static
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
    >
)