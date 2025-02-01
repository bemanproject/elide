// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_ELIDE_ELIDE_HPP
#define BEMAN_ELIDE_ELIDE_HPP

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace beman::elide {

namespace detail::elide {

template<typename T>
concept result = !std::same_as<void, T>;

template<typename T, typename... Args>
concept c =
  std::invocable<T, Args...> &&
  result<std::invoke_result_t<T, Args...>> &&
  std::is_reference_v<T> &&
  (std::is_reference_v<Args> && ...);

template<typename T, typename... Args>
concept lvalue =
  std::invocable<T&, Args&...> &&
  result<std::invoke_result_t<T&, Args&...>>;

struct tombstone {};

template<typename T, typename... Args>
struct lvalue_result : std::type_identity<tombstone> {};
template<typename T, typename... Args>
  requires lvalue<T, Args...>
struct lvalue_result<T, Args...> : std::invoke_result<T&, Args&...> {};

}

template<typename T, typename... Args>
  requires detail::elide::c<T, Args...>
struct elide {
  elide() = delete;
  elide(const elide&) = delete;
  elide& operator=(const elide&) = delete;
  constexpr explicit elide(T t, Args... args) noexcept
    : t_(std::forward<T>(t)),
      args_(std::forward<Args>(args)...)
  {}
private:
  static constexpr bool rvalue_noexcept_ =
    std::is_nothrow_invocable_v<T, Args...>;
  static constexpr bool lvalue_noexcept_ =
    std::is_nothrow_invocable_v<T&, Args&...>;
  using rvalue_result_type_ = std::invoke_result_t<T, Args...>;
  using lvalue_result_type_ = typename detail::elide::lvalue_result<
    T,
    Args...>::type;
public:
  constexpr operator rvalue_result_type_() const&& noexcept(rvalue_noexcept_) {
    return std::apply(
      [&](auto&&... args) noexcept(rvalue_noexcept_) {
        return std::invoke(
          std::forward<T>(t_),
          std::forward<Args>(args)...);
      },
      args_);
  }
  constexpr rvalue_result_type_ operator()() const&& noexcept(rvalue_noexcept_)
  {
    return rvalue_result_type_(std::move(*this));
  }
  constexpr operator lvalue_result_type_() const& noexcept(lvalue_noexcept_) {
    if constexpr (detail::elide::lvalue<T, Args...>) {
      return std::apply(
        [&](auto&&... args) noexcept(lvalue_noexcept_) {
          return std::invoke(t_, args...);
        },
        args_);
    } else {
      return {};
    }
  }
  constexpr lvalue_result_type_ operator()() const& noexcept(lvalue_noexcept_)
    requires detail::elide::lvalue<T, Args...>
  {
    return rvalue_result_type_(*this);
  }
private:
  [[no_unique_address]]
  T t_;
  [[no_unique_address]]
  std::tuple<Args...> args_;
};

template<typename T, typename... Args>
explicit elide(T&&, Args&&...) -> elide<T&&, Args&&...>;

} // namespace beman::elide

#endif // BEMAN_ELIDE_ELIDE_HPP
