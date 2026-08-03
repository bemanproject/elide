// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_EMPLACE_FROM_DEDUCE_HPP
#define BEMAN_EMPLACE_FROM_DEDUCE_HPP

#include <beman/emplace_from/config.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES() && !defined(BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT)

import beman.emplace_from;

#else

    #ifndef BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT
        #include <type_traits>
        #include <utility>
        #include <beman/emplace_from/detail/deduce.hpp>
    #endif

namespace beman::emplace_from {

template <typename T>
struct deduce : std::type_identity<T> {};
template <typename T>
    requires detail::is_emplace_from<std::remove_cvref_t<T> >::value && requires(T t) { std::forward<T>(t)(); }
struct deduce<T> : std::type_identity<decltype(std::declval<T>()())> {};

template <typename T>
using deduce_t = deduce<T>::type;

} // namespace beman::emplace_from

#endif // BEMAN_EMPLACE_FROM_USE_MODULES() &&
       // !defined(BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_EMPLACE_FROM_DEDUCE_HPP
