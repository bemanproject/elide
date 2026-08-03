// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_EMPLACE_FROM_EMPLACE_FROM_HPP
#define BEMAN_EMPLACE_FROM_EMPLACE_FROM_HPP

#include <beman/emplace_from/config.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES() && !defined(BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT)

import beman.emplace_from;

#else

    #ifndef BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT
        #include <type_traits>
        #include <utility>
        #include <beman/emplace_from/detail/emplace_from.hpp>
    #endif

namespace beman::emplace_from {

template <typename Fun>
class emplace_from {
    Fun fun_;

    template <typename Self>
    using fun_t_ = decltype(std::forward_like<Self>(std::declval<Fun>()));

    template <typename Self>
    using result_t_ = decltype(std::declval<fun_t_<Self> >()());

    template <typename Self>
    static constexpr bool enabled_ = detail::emplace_from_concept<fun_t_<Self> >;

    template <typename Self>
    static constexpr bool nothrow_ = noexcept(std::declval<fun_t_<Self> >()());

  public:
    template <typename Self>
        requires enabled_<Self>
    constexpr result_t_<Self> operator()(this Self&& self) noexcept(nothrow_<Self>) {
        return std::forward<Self>(self).fun_();
    }

    template <typename Self>
        requires enabled_<Self>
    constexpr operator result_t_<Self>(this Self&& self) noexcept(nothrow_<Self>) {
        return std::forward<Self>(self)();
    }

    emplace_from() = default;
    explicit constexpr emplace_from(Fun f) noexcept(std::is_nothrow_move_constructible_v<Fun>) : fun_(std::move(f)) {}
};

} // namespace beman::emplace_from

#endif // BEMAN_EMPLACE_FROM_USE_MODULES() &&
       // !defined(BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_EMPLACE_FROM_EMPLACE_FROM_HPP
