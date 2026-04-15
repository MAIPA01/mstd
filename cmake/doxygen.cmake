find_package(Doxygen)

if (DOXYGEN_FOUND)
    add_custom_target(documentation_${PROJECT_NAME}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                      COMMENT "Generating documentation..."
                      VERBATIM)
else()
    message(WARNING "Doxygen not found, documentation target will not be available.")
endif()