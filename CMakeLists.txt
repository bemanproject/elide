# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.4)

include(infra/cmake/enable-experimental-import-std.cmake)

project(
    beman.emplace_from
    DESCRIPTION
        "Utility to enable emplacement of immovable objects returned by a function via guaranteed RVO."
    LANGUAGES CXX
    VERSION 0.1.0
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_EMPLACE_FROM_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_EMPLACE_FROM_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(
    BEMAN_EMPLACE_FROM_USE_MODULES
    "Provide beman.emplace_from as a C++ module"
    OFF
)

if(BEMAN_EMPLACE_FROM_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/include/beman/emplace_from/config_generated.hpp.in"
    "${PROJECT_BINARY_DIR}/include/beman/emplace_from/config_generated.hpp"
    @ONLY
)

# for find of beman_install_library and configure_build_telemetry
include(infra/cmake/beman-install-library.cmake)
include(infra/cmake/BuildTelemetryConfig.cmake)

if(BEMAN_EMPLACE_FROM_USE_MODULES)
    add_library(beman.emplace_from STATIC)
else()
    add_library(beman.emplace_from INTERFACE)
endif()
add_library(beman::emplace_from ALIAS beman.emplace_from)

if(BEMAN_EMPLACE_FROM_USE_MODULES)
    target_sources(
        beman.emplace_from
        PUBLIC
            FILE_SET CXX_MODULES
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(beman.emplace_from PROPERTIES CXX_MODULE_STD ON)
    target_compile_features(beman.emplace_from PUBLIC cxx_std_23)
else()
    target_sources(
        beman.emplace_from
        PUBLIC
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(
        beman.emplace_from
        PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
    )
endif()

add_subdirectory(include/beman/emplace_from)

beman_install_library(beman.emplace_from TARGETS beman.emplace_from)
configure_build_telemetry()

if(BEMAN_EMPLACE_FROM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/emplace_from)
endif()

if(BEMAN_EMPLACE_FROM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
