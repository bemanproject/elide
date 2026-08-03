# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES optional synchronized)
message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.emplace_from.examples.${example})
    target_sources(
        beman.emplace_from.examples.${example}
        PRIVATE ${example}.cpp
    )
    target_link_libraries(
        beman.emplace_from.examples.${example}
        PRIVATE beman::emplace_from
    )
    if(BEMAN_EMPLACE_FROM_USE_MODULES)
        set_target_properties(
            beman.emplace_from.examples.${example}
            PROPERTIES CXX_MODULE_STD ON
        )
    endif()
endforeach()
