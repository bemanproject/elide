// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/elide/deduce.hpp>

#include <type_traits>
#include <utility>
#include <beman/elide/elide.hpp>

#include <gtest/gtest.h>

namespace beman::elide {

namespace {

struct factory {
    int operator()();
};

struct no_lvalue_factory {
    int operator()() &&;
};

struct split_factory {
    int    operator()() &;
    double operator()() &&;
};

template <typename T>
struct wrapper {
    template <typename U>
    explicit wrapper(U&& u) noexcept(std::is_nothrow_constructible_v<T, U>) : t_(std::forward<U>(u)) {}
    constexpr const T& get() const noexcept { return t_; }

  private:
    T t_;
};

template <typename T>
explicit wrapper(T&&) -> wrapper<deduce_t<T>>;

} // namespace

static_assert(!detail::deduce::is_elide<int>::value);
static_assert(detail::deduce::is_elide<elide<factory&>>::value);

static_assert(std::is_same_v<int, deduce_t<int>>);
static_assert(std::is_same_v<int, deduce_t<int&>>);
static_assert(std::is_same_v<int, deduce_t<const int&>>);
static_assert(std::is_same_v<int, deduce_t<int&&>>);
static_assert(std::is_same_v<int, deduce_t<const int&&>>);

static_assert(std::is_same_v<int, deduce_t<elide<factory&>>>);
static_assert(std::is_same_v<int, deduce_t<elide<factory&>&>>);
static_assert(std::is_same_v<int, deduce_t<const elide<factory&>&>>);
static_assert(std::is_same_v<int, deduce_t<elide<factory&>&&>>);
static_assert(std::is_same_v<int, deduce_t<const elide<factory&>&&>>);

static_assert(std::is_same_v<int, deduce_t<elide<no_lvalue_factory&&>>>);
static_assert(std::is_same_v<int, deduce_t<elide<no_lvalue_factory&&>&&>>);
static_assert(std::is_same_v<int, deduce_t<const elide<no_lvalue_factory&&>&&>>);
static_assert(std::is_same_v<elide<no_lvalue_factory&&>, deduce_t<const elide<no_lvalue_factory&&>&>>);
static_assert(std::is_same_v<elide<no_lvalue_factory&&>, deduce_t<elide<no_lvalue_factory&&>&>>);

static_assert(std::is_same_v<double, deduce_t<elide<split_factory&&>>>);
static_assert(std::is_same_v<double, deduce_t<elide<split_factory&&>&&>>);
static_assert(std::is_same_v<int, std::invoke_result_t<elide<split_factory&&>&>>);
static_assert(std::is_same_v<int, deduce_t<elide<split_factory&&>&>>);

TEST(Deduce, basic) {
    wrapper w(elide([]() noexcept { return 5; }));
    EXPECT_EQ(5, w.get());
}

} // namespace beman::elide
