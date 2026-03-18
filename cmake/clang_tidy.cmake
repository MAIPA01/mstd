if(DEFINED _CLANG_TIDY_HELPER_INCLUDED)
    return()
endif()
set(_CLANG_TIDY_HELPER_INCLUDED TRUE)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

function(enable_clang_tidy TARGET)
    if(NOT PROJECT_IS_TOP_LEVEL)
        return()
    endif()

    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found – skipping for target ${TARGET}")
        return()
    endif()

    if(NOT TARGET ${TARGET})
        message(FATAL_ERROR "enable_clang_tidy: target '${TARGET}' does not exist")
    endif()

    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    if(NOT TARGET_SOURCES)
        message(STATUS "Target ${TARGET} has no sources - skipping clang-tidy")
        return()
    endif()

    set(TIDY_SOURCES)
    foreach(SRC ${TARGET_SOURCES})
        get_source_file_property(TIDY_DISABLED ${SRC} CXX_CLANG_TIDY)

        if(NOT DEFINED TIDY_DISABLED OR NOT TIDY_DISABLED STREQUAL "")
            list(APPEND TIDY_SOURCES "\\\"${SRC}\\\"")
        endif()
    endforeach()
    string(JOIN ";" TIDY_SOURCES_STR ${TIDY_SOURCES})

    set(CLANG_TIDY_CONFIG ${CMAKE_SOURCE_DIR}/.clang-tidy)
    get_target_property(TARGET_SOURCE_DIR ${TARGET} SOURCE_DIR)

    file(RELATIVE_PATH TARGET_SOURCE_DIR_REL ${CMAKE_SOURCE_DIR} ${TARGET_SOURCE_DIR})
    get_filename_component(PROJECT_FOLDER_NAME "${CMAKE_SOURCE_DIR}" NAME)

    set(CLANG_TIDY_ARGS
        "--quiet"
        "--system-headers=false"
        "--extra-arg=-Wno-macro-redefined"
        "--extra-arg=-std=gnu++20"
        "--header-filter=^.*[\\\\\/\\\\]${PROJECT_FOLDER_NAME}[\\\\\/\\\\]${TARGET_SOURCE_DIR_REL}[\\\\\/\\\\].*"
        "--exclude-header-filter=^.*[\\\\\/\\\\]${PROJECT_FOLDER_NAME}[\\\\\/\\\\]ThirdParty[\\\\\/\\\\].*"
    )

    string(JOIN ";" CLANG_TIDY_ARGS_STR ${CLANG_TIDY_ARGS})

    set(CACHE_DIR ${CMAKE_BINARY_DIR}/clang-tidy-cache)
    file(MAKE_DIRECTORY ${CACHE_DIR})

    set(CACHE_FILE ${CACHE_DIR}/${TARGET}.tidy)

    set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY 
                          "${CLANG_TIDY_EXE};--config-file=${CLANG_TIDY_CONFIG};-p=${CMAKE_BINARY_DIR};${CLANG_TIDY_ARGS_STR}"
    )

    set(TIDY_TARGET clang-tidy-${TARGET})
    set(TIDY_RUNNER ${CMAKE_SOURCE_DIR}/cmake/run_clang_tidy.cmake)

    add_custom_target(
        ${TIDY_TARGET}
        COMMAND ${CMAKE_COMMAND}
            -DFORCE_TIDY=OFF
            -DCLANG_TIDY_EXE="${CLANG_TIDY_EXE}"
            -DCLANG_TIDY_ARGS="${CLANG_TIDY_ARGS}"
            -DCLANG_COMPILE_COMMANDS_DIR="${CMAKE_BINARY_DIR}"
            -DCLANG_TIDY_CONFIG="${CLANG_TIDY_CONFIG}"
            -DSOURCES="${TIDY_SOURCES_STR}"
            -DCACHE_FILE="${CACHE_FILE}"
            -P "${TIDY_RUNNER}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Running clang-tidy for target ${TARGET}"
    )

    add_custom_target(
        ${TIDY_TARGET}-full
        COMMAND ${CMAKE_COMMAND}
            -DFORCE_TIDY=ON
            -DCLANG_TIDY_EXE="${CLANG_TIDY_EXE}"
            -DCLANG_TIDY_ARGS="${CLANG_TIDY_ARGS}"
            -DCLANG_COMPILE_COMMANDS_DIR="${CMAKE_BINARY_DIR}"
            -DCLANG_TIDY_CONFIG="${CLANG_TIDY_CONFIG}"
            -DSOURCES="${TIDY_SOURCES_STR}"
            -DCACHE_FILE="${CACHE_FILE}"
            -P "${TIDY_RUNNER}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Running full clang-tidy for target ${TARGET}"
    )

    message(STATUS "clang-tidy target added: ${TIDY_TARGET}")
endfunction()

function(disable_clang_tidy_for_file FILE)

    set_source_files_properties(
        "${FILE}"
        PROPERTIES
        CXX_CLANG_TIDY ""
    )

    message(STATUS "clang-tidy disabled for: ${FILE}")
endfunction()