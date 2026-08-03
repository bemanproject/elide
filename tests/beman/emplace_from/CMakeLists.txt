# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(Catch2 3 REQUIRED)

add_executable(beman.emplace_from.tests.emplace_from)
target_sources(
    beman.emplace_from.tests.emplace_from
    PRIVATE emplace_from.test.cpp
)
target_link_libraries(
    beman.emplace_from.tests.emplace_from
    PRIVATE beman::emplace_from Catch2::Catch2WithMain
)
if(BEMAN_EMPLACE_FROM_USE_MODULES)
    set_target_properties(
        beman.emplace_from.tests.emplace_from
        PROPERTIES CXX_MODULE_STD ON
    )
endif()

include(Catch)
catch_discover_tests(beman.emplace_from.tests.emplace_from)

add_executable(beman.emplace_from.tests.deduce)
target_sources(beman.emplace_from.tests.deduce PRIVATE deduce.test.cpp)
target_link_libraries(
    beman.emplace_from.tests.deduce
    PRIVATE beman::emplace_from Catch2::Catch2WithMain
)
if(BEMAN_EMPLACE_FROM_USE_MODULES)
    set_target_properties(
        beman.emplace_from.tests.deduce
        PROPERTIES CXX_MODULE_STD ON
    )
endif()

catch_discover_tests(beman.emplace_from.tests.deduce)
