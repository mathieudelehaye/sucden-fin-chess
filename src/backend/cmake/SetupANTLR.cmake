# Setup ANTLR4 scanner / parser generator:
# https://github.com/antlr/antlr4/discussions/4418
if(NOT ANTLR_SETUP_DONE)
    include(FetchContent)

    set(ANTLR_BUILD_SHARED ON)
    set(ANTLR_TAG 4.13.1)
    set(ANTLR_BUILD_CPP_TESTS OFF)

    # Download ANTLR JAR to persistent download directory if not exists
    set(ANTLR_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/../../download")
    set(ANTLR_EXECUTABLE "${ANTLR_DOWNLOAD_DIR}/antlr-${ANTLR_TAG}-complete.jar")

    if(NOT EXISTS "${ANTLR_EXECUTABLE}")
        message(STATUS "Downloading ANTLR ${ANTLR_TAG} JAR to ${ANTLR_DOWNLOAD_DIR}...")
        file(MAKE_DIRECTORY "${ANTLR_DOWNLOAD_DIR}")
        file(
            DOWNLOAD
            "https://www.antlr.org/download/antlr-${ANTLR_TAG}-complete.jar"
            "${ANTLR_EXECUTABLE}"
            SHOW_PROGRESS
        )
        message(STATUS "Downloaded ANTLR JAR to: ${ANTLR_EXECUTABLE}")
    else()
        message(STATUS "Using existing ANTLR JAR: ${ANTLR_EXECUTABLE}")
    endif()

    # Fetch ANTLR C++ runtime
    FetchContent_Declare(
        antlr
        GIT_REPOSITORY https://github.com/antlr/antlr4
        GIT_TAG ${ANTLR_TAG}
        SOURCE_SUBDIR "runtime/Cpp"
    )
    FetchContent_MakeAvailable(antlr)

    # Include ANTLR CMake functions
    include(${antlr_SOURCE_DIR}/runtime/Cpp/cmake/FindANTLR.cmake)

    set(ANTLR_SETUP_DONE TRUE CACHE INTERNAL "ANTLR setup completed")
endif()