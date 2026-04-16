if(NOT MSTD_FMT_EXTERNAL AND NOT TARGET fmt::fmt)
    set(DOWNLOAD_FMT ON)
else()
    set(DOWNLOAD_FMT OFF)
endif()

if ((MSTD_BUILD_TESTS OR MSTD_BUILD_COVERAGE) AND NOT TARGET gtest_main)
    set(DOWNLOAD_GTEST ON)
else()
    set(DOWNLOAD_GTEST OFF)
endif()

if(DOWNLOAD_FMT OR DOWNLOAD_GTEST)
    include(${CMAKE_CURRENT_LIST_DIR}/get_cpm.cmake)
endif()

# FMT
if(DOWNLOAD_FMT)
    CPMAddPackage("gh:fmtlib/fmt#12.1.0")
endif()

# GOOGLE TESTS
if(DOWNLOAD_GTEST)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()