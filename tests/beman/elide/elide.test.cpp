// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/elide/elide.hpp>

#include <concepts>
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace beman::elide {

namespace {

struct instrumented {
  struct state_type {
    state_type() = default;
    state_type(const state_type&) = delete;
    state_type& operator=(const state_type&) = delete;
    std::size_t constructed{0};
    std::size_t copy_constructed{0};
    std::size_t move_constructed{0};
  };
  explicit instrumented(state_type& state) noexcept
    : state_(state)
  {
    ++state_.constructed;
  }
  instrumented(const instrumented& other) noexcept
    : state_(other.state_)
  {
    ++state_.copy_constructed;
  }
  instrumented(instrumented&& other) noexcept
    : state_(other.state_)
  {
    ++state_.move_constructed;
  }
private:
  state_type& state_;
};

template<typename... Args>
struct factory {
  struct state_type {
    std::vector<std::tuple<Args...>> lvalue_arguments;
    std::vector<std::tuple<Args...>> rvalue_arguments;
  };
  explicit factory(state_type& state, instrumented::state_type& other_state)
    noexcept
    : state_(state),
      other_state_(other_state)
  {}
  template<typename... Us>
    requires (std::is_constructible_v<Args, Us> && ...)
  instrumented operator()(Us&&... us) const&& {
    state_.rvalue_arguments.emplace_back(std::forward<Us>(us)...);
    return instrumented(other_state_);
  }
  template<typename... Us>
    requires (std::is_constructible_v<Args, Us> && ...)
  instrumented operator()(Us&&... us) const& {
    state_.lvalue_arguments.emplace_back(std::forward<Us>(us)...);
    return instrumented(other_state_);
  }
private:
  state_type& state_;
  instrumented::state_type& other_state_;
};

struct split_factory {
  std::vector<int> operator()() &;
  std::vector<double> operator()() &&;
};

static_assert(
  std::is_convertible_v<
    elide<split_factory&&>,
    std::vector<double>>);
static_assert(
  std::is_convertible_v<
    elide<split_factory&&>&&,
    std::vector<double>>);
static_assert(
  std::is_convertible_v<
    elide<split_factory&&>&,
    std::vector<int>>);

}

TEST(Elide, no_arguments_lvalue_invocable) {
  instrumented::state_type state;
  factory<>::state_type factory_state;
  const factory<> f(factory_state, state);
  const elide e(std::move(f));
  static_assert(std::is_convertible_v<decltype(e)&&, instrumented>);
  static_assert(std::is_convertible_v<decltype(e)&, instrumented>);
  EXPECT_EQ(0, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  (instrumented)e;
  EXPECT_EQ(1, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(1, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  (instrumented)e;
  EXPECT_EQ(2, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(2, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  {
    instrumented res = e();
    (void)res;
  }
  EXPECT_EQ(3, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(3, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  (instrumented)std::move(e);
  EXPECT_EQ(4, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(3, factory_state.lvalue_arguments.size());
  EXPECT_EQ(1, factory_state.rvalue_arguments.size());
  {
    instrumented res = std::move(e)();
    (void)res;
  }
  EXPECT_EQ(5, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(3, factory_state.lvalue_arguments.size());
  EXPECT_EQ(2, factory_state.rvalue_arguments.size());
}

TEST(Elide, one_argument_lvalue_invocable) {
  instrumented::state_type state;
  factory<int>::state_type factory_state;
  const factory<int> f(factory_state, state);
  const int i = 5;
  const elide e(std::move(f), i);
  static_assert(std::is_convertible_v<decltype(e)&&, instrumented>);
  static_assert(std::is_convertible_v<decltype(e)&, instrumented>);
  EXPECT_EQ(0, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  (instrumented)e;
  EXPECT_EQ(1, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  ASSERT_EQ(1, factory_state.lvalue_arguments.size());
  EXPECT_EQ(5, std::get<0>(factory_state.lvalue_arguments.back()));
  (instrumented)e;
  EXPECT_EQ(2, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  EXPECT_EQ(2, factory_state.lvalue_arguments.size());
  EXPECT_EQ(5, std::get<0>(factory_state.lvalue_arguments.back()));
  (instrumented)std::move(e);
  EXPECT_EQ(3, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(2, factory_state.lvalue_arguments.size());
  ASSERT_EQ(1, factory_state.rvalue_arguments.size());
  EXPECT_EQ(5, std::get<0>(factory_state.rvalue_arguments.back()));
}

TEST(Elide, one_non_copyable_argument) {
  instrumented::state_type state;
  factory<std::unique_ptr<int>>::state_type factory_state;
  const factory<std::unique_ptr<int>> f(factory_state, state);
  auto ptr = std::make_unique<int>(5);
  const auto expected = ptr.get();
  const elide e(std::move(f), std::move(ptr));
  static_assert(std::is_convertible_v<decltype(e)&&, instrumented>);
  static_assert(!std::is_convertible_v<decltype(e)&, instrumented>);
  EXPECT_EQ(0, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.lvalue_arguments.size());
  EXPECT_EQ(0, factory_state.rvalue_arguments.size());
  (instrumented)std::move(e);
  EXPECT_EQ(1, state.constructed);
  EXPECT_EQ(0, state.copy_constructed);
  EXPECT_EQ(0, state.move_constructed);
  EXPECT_EQ(0, factory_state.lvalue_arguments.size());
  ASSERT_EQ(1, factory_state.rvalue_arguments.size());
  ASSERT_TRUE(std::get<0>(factory_state.rvalue_arguments.front()));
  EXPECT_EQ(
    expected,
    std::get<0>(factory_state.rvalue_arguments.front()).get());
}

}
