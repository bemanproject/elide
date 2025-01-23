# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(beman.elide INTERFACE)
add_library(beman::elide ALIAS beman.elide)

target_sources(
    beman.elide
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES ${PROJECT_SOURCE_DIR}/include/beman/elide/elide.hpp
)

set_target_properties(beman.elide PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

install(
    TARGETS beman.elide
    EXPORT beman.elide
    DESTINATION
    $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
