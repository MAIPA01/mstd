if(NOT MSTD_FMT_EXTERNAL OR MSTD_BUILD_TESTS)
    include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)
endif()

# FMT
if(NOT MSTD_FMT_EXTERNAL)
    CPMAddPackage("gh:fmtlib/fmt#12.1.0")
endif()

# GOOGLE TESTS
if(MSTD_BUILD_TESTS)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()