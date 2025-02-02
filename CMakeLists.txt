# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.25)

project(beman.elide DESCRIPTION "Implements proposed std::elide" LANGUAGES CXX)

enable_testing()

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_ELIDE_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ON. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

include(FetchContent)
include(GNUInstallDirs)

if(BEMAN_ELIDE_BUILD_TESTS)
    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG
            f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
        EXCLUDE_FROM_ALL
    )
    set(INSTALL_GTEST OFF) # Disable GoogleTest installation
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(src/beman/elide)

if(BEMAN_ELIDE_BUILD_TESTS)
    add_subdirectory(tests/beman/elide)
endif()
