# Configures Doxygen documentation generation

find_package(Doxygen REQUIRED)

# Input & output directories
set(DOXYGEN_INPUT_DIR  "${CMAKE_SOURCE_DIR}/exe")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")
set(DOXYGEN_CONFIG_TEMPLATE "${CMAKE_SOURCE_DIR}/exe/Doxyfile.in")
set(DOXYGEN_CONFIG_FILE "${CMAKE_BINARY_DIR}/Doxyfile")

# Configure the template file into a usable Doxyfile
configure_file(
    "${DOXYGEN_CONFIG_TEMPLATE}"
    "${DOXYGEN_CONFIG_FILE}"
    @ONLY
)

# Documentation generation target
add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating backend documentation with Doxygen"
    VERBATIM
)

# Copy generated docs to the project documentation folder
add_custom_command(
    TARGET docs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_SOURCE_DIR}/../../doc/backend"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/doc"
            "${CMAKE_SOURCE_DIR}/../../doc/backend"
    COMMENT "Copying backend documentation to source tree doc/backend"
)
