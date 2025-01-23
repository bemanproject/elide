# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GoogleTest)

add_executable(beman.elide.tests.elide)
target_sources(beman.elide.tests.elide PRIVATE elide.test.cpp)
target_link_libraries(
    beman.elide.tests.elide
    PRIVATE beman::elide GTest::gtest GTest::gtest_main
)

gtest_add_tests(beman.elide.tests.elide "" AUTO)
