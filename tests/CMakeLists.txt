project(mstd-tests VERSION 1.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# source files
set(PROJECT_TEST_SOURCES ${PROJECT_NAME}_TEST_SOURCES)
file(GLOB_RECURSE ${PROJECT_TEST_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# make exec
add_executable(${PROJECT_NAME} ${${PROJECT_TEST_SOURCES}})

target_link_libraries(${PROJECT_NAME} PRIVATE mstd gtest_main)

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
        $<$<BOOL:${BUILD_MSTD_COVERAGE}>:
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:
            -O3
            -flto
        >
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
        $<$<BOOL:${BUILD_MSTD_COVERAGE}>:
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
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-fuse-ld=lld>
        -static
        $<$<BOOL:${BUILD_MSTD_COVERAGE}>:-fprofile-instr-generate>
    >
    # disable noexcept type warning on GCC
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-flto>
        -static
        $<$<BOOL:${BUILD_MSTD_COVERAGE}>:-fprofile-instr-generate>
    >
)