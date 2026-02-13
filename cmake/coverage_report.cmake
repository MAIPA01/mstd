
function(setup_coverage_report)
    # CHECK COMPILER VERSION


    # CHECK IF PROGRAMS EXISTS
    find_program(LLVM_PROFDATA llvm-profdata REQUIRED)
    find_program(LLVM_COV llvm-cov REQUIRED)

    set(options CLEAN_TEMP)
    set(oneValueArgs TEST_PROJECT FORMAT OUTPUT_DIR)
    set(multiValueArgs EXE_PROJECTS)

    cmake_parse_arguments(PARSE_ARGV 0 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    # CHECK IF TEST PROJECT WAS PROVIEDED
    if(NOT DEFINED COVERAGE_TEST_PROJECT)
        message(FATAL_ERROR "Please provide explicitly TEST_PROJECT.")
    endif()

    # DEFAULT FORMAT
    if(NOT DEFINED COVERAGE_FORMAT)
        set(COVERAGE_FORMAT html)
    endif()

    # DEFAULT OUTPUT_DIR
    if(NOT DEFINED COVERAGE_OUTPUT_DIR)
        set(COVERAGE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/coverage_report)
    endif()

    if(NOT EXISTS ${COVERAGE_OUTPUT_DIR})
        file(MAKE_DIRECTORY ${COVERAGE_OUTPUT_DIR})
    endif()

    # PROFDATA FILE PATH VARIABLE
    set(PROFDATA_FILE "${COVERAGE_OUTPUT_DIR}/coverage.profdata")
    file(TO_NATIVE_PATH ${PROFDATA_FILE} PROFDATA_FILE)

    # SETUP PROJECTS LIST
    set(EXE_PROJECTS "${COVERAGE_TEST_PROJECT};${COVERAGE_EXE_PROJECTS};${COVERAGE_UNPARSED_ARGUMENTS}")

    # CREATE TARGET
    add_custom_target(generate_coverage_report
        COMMAND rm -rf "${COVERAGE_OUTPUT_DIR}/*"
        COMMENT "Generating coverage report (format: ${COVERAGE_FORMAT}) in ${COVERAGE_OUTPUT_DIR}"
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        DEPENDS ${EXE_PROJECTS}
        VERBATIM
    )

    # LIST OF PROFRAWS FOR EACH PROJECT
    set(PROJECTS_PROFRAWS "")
    set(PROJECTS_EXECS "")
    foreach(PROJECT_NAME ${EXE_PROJECTS})
        # GET BINARY_DIR OF PROJECT
        set(BINARY_DIR ${${PROJECT_NAME}_BINARY_DIR})
        # GET EXE FILE
        set(EXE_FILE "${BINARY_DIR}/${PROJECT_NAME}.exe")

        list(APPEND PROJECTS_EXECS ${EXE_FILE})

        # SET PROJECT_PROFRAW PATH
        set(PROJECT_PROFRAW "${BINARY_DIR}/${PROJECT_NAME}.profraw")
        file(TO_NATIVE_PATH ${PROJECT_PROFRAW} PROJECT_PROFRAW)

        # APPEND TO LIST
        list(APPEND PROJECTS_PROFRAWS ${PROJECT_PROFRAW})

        # ADD COMMAND
        add_custom_command(TARGET generate_coverage_report POST_BUILD
            COMMAND set LLVM_PROFILE_FILE=${PROJECT_PROFRAW}
            COMMAND ${EXE_FILE}
            COMMENT "Running program to generate ${PROFRAW_TESTS}"
            WORKING_DIRECTORY ${BINARY_DIR}
            VERBATIM
        )
    endforeach()

    # ADD MERGE COMMAND
    list(JOIN PROJECTS_PROFRAWS " " PROFRAWS_TO_MERGE)
    add_custom_command(TARGET generate_coverage_report POST_BUILD
        COMMAND ${LLVM_PROFDATA} merge -sparse -instr -output=${PROFDATA_FILE} ${PROFRAWS_TO_MERGE}
        COMMENT "Merging coverage data into ${PROFDATA_FILE}"
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        VERBATIM
    )

    # ADD GENERATE REPORT COMMAND
    set(TEST_PROJECT_SOURCE_DIR ${${COVERAGE_TEST_PROJECT}_SOURCE_DIR})
    file(RELATIVE_PATH TEST_PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_PROJECT_SOURCE_DIR})
    file(TO_NATIVE_PATH ${TEST_PROJECT_SOURCE_DIR} TEST_PROJECT_SOURCE_DIR)
    set(TEST_FILES_REGEX ".*[\\\\|\\\/]${TEST_PROJECT_SOURCE_DIR}[\\\\|\\\/].*")
    
    list(GET PROJECTS_EXECS 0 TEST_EXE)
    add_custom_command(TARGET generate_coverage_report POST_BUILD
        COMMAND ${LLVM_COV} show ${TEST_EXE} -instr-profile=${PROFDATA_FILE} 
        -format=${COVERAGE_FORMAT} -output-dir=${COVERAGE_OUTPUT_DIR} -ignore-filename-regex=${TEST_FILES_REGEX} -show-mcdc -show-line-counts 
        -show-expansions -show-instantiations -show-regions -show-line-counts-or-regions -show-directory-coverage -use-color --show-branches percent
        COMMENT "Generating report..."
        WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
        VERBATIM
    )

    # CLEAN TEMP
    if(${COVERAGE_CLEAN_TEMP})
        foreach(PROFRAW ${PROJECTS_PROFRAWS})
            get_filename_component(PROFRAWS_DIR ${PROFRAW} DIRECTORY)

            add_custom_command(TARGET generate_coverage_report POST_BUILD
                COMMAND rm ${PROFRAW}
                COMMENT "Cleaning profraw file (${PROFRAW})..."
                WORKING_DIRECTORY ${PROFRAWS_DIR}
                VERBATIM
            )
        endforeach()

        add_custom_command(TARGET generate_coverage_report POST_BUILD
            COMMAND rm ${PROFDATA_FILE}
            COMMENT "Cleaning profdata file (${PROFDATA_FILE})..."
            WORKING_DIRECTORY ${COVERAGE_OUTPUT_DIR}
            VERBATIM
        )
    endif()
endfunction()