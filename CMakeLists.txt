cmake_minimum_required(VERSION 3.16)
project(mstd VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# header files
file(GLOB_RECURSE MSTD_HEADERS include/*.hpp)

# make library
add_library(${PROJECT_NAME} INTERFACE ${MSTD_HEADERS})

# set library alias
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# set headers folder
target_include_directories(${PROJECT_NAME} INTERFACE
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# Optional compiler flags
if(MSVC)
	target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX /Zc:preprocessor)
else()
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpendantic -Werror)
endif()

# library installation
install(TARGETS ${PROJECT_NAME} EXPORT mstd-config
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

# export cmake config
install(EXPORT mstd-config DESTINATION cmake/mstd NAMESPACE ${PROJECT_NAME}::)
export(EXPORT mstd-config NAMESPACE ${PROJECT_NAME}::)

# configuration version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/mstd-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/mstd-config-version.cmake"
	DESTINATION cmake/mstd
)