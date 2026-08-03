// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_EMPLACE_FROM_DETAIL_EMPLACE_FROM_HPP
#define BEMAN_EMPLACE_FROM_DETAIL_EMPLACE_FROM_HPP

#ifndef BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT
    #include <concepts>
    #include <utility>
#endif

namespace beman::emplace_from::detail {

template <typename T>
concept is_not_void = !std::same_as<T, void>;

template <typename T>
concept emplace_from_concept = requires(T t) {
    { std::forward<T>(t)() } -> is_not_void;
};

} // namespace beman::emplace_from::detail

#endif // BEMAN_EMPLACE_FROM_DETAIL_EMPLACE_FROM_HPP
