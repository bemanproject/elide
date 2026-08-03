// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_EMPLACE_FROM_DETAIL_DEDUCE_HPP
#define BEMAN_EMPLACE_FROM_DETAIL_DEDUCE_HPP

#ifndef BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT
    #include <type_traits>
    #include <beman/emplace_from/emplace_from.hpp>
#endif

namespace beman::emplace_from::detail {

template <typename>
struct is_emplace_from : std::false_type {};
template <typename Fun>
struct is_emplace_from<::beman::emplace_from::emplace_from<Fun>> : std::true_type {};

} // namespace beman::emplace_from::detail

#endif // BEMAN_EMPLACE_FROM_DETAIL_DEDUCE_HPP
