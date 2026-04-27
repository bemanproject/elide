// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_ELIDE_DEDUCE_HPP
#define BEMAN_ELIDE_DEDUCE_HPP

#include <type_traits>
#include <beman/elide/elide.hpp>

namespace beman::elide {

namespace detail::deduce {

template <typename>
struct is_elide : std::false_type {};
template <typename T, typename... Args>
struct is_elide<beman::elide::elide<T, Args...>> : std::true_type {};

} // namespace detail::deduce

template <typename T>
struct deduce : std::remove_cvref<T> {};
template <typename T>
    requires detail::deduce::is_elide<std::remove_cvref_t<T>>::value && std::is_invocable_v<T>
struct deduce<T> : std::invoke_result<T> {};

template <typename T>
using deduce_t = typename deduce<T>::type;

} // namespace beman::elide

#endif // BEMAN_ELIDE_DEDUCE_HPP
