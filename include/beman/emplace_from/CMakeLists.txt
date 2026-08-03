# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

if(BEMAN_EMPLACE_FROM_USE_MODULES)
    target_sources(
        beman.emplace_from
        PUBLIC
            FILE_SET CXX_MODULES FILES emplace_from.cppm
            FILE_SET HEADERS
                FILES
                    config.hpp
                    deduce.hpp
                    detail/deduce.hpp
                    detail/emplace_from.hpp
                    emplace_from.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/emplace_from/config_generated.hpp"
    )
else()
    target_sources(
        beman.emplace_from
        PUBLIC
            FILE_SET HEADERS
                FILES
                    config.hpp
                    deduce.hpp
                    detail/deduce.hpp
                    detail/emplace_from.hpp
                    emplace_from.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/emplace_from/config_generated.hpp"
    )
endif()
