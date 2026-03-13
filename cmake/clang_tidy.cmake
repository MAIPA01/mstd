if(DEFINED _CLANG_TIDY_HELPER_INCLUDED)
    return()
endif()
set(_CLANG_TIDY_HELPER_INCLUDED TRUE)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

function(enable_clang_tidy TARGET)
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found skipping for target ${TARGET}")
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

    set(CLANG_TIDY_ARGS
        "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
        "-p=${CMAKE_BINARY_DIR}"
        "--quiet"
        "--header-filter=^Engine/.*"
    )

    set_target_properties(${TARGET} PROPERTIES 
        CXX_CLANG_TIDY "${CLANG_TIDY_EXE};${CLANG_TIDY_ARGS}"
    )

    set(TIDY_TARGET clang-tidy-${TARGET})

    add_custom_target(
        ${TIDY_TARGET}
        COMMAND ${CLANG_TIDY_EXE}
            ${CLANG_TIDY_ARGS}
            ${TARGET_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
        COMMENT "Running clang-tidy for target ${TARGET}"
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