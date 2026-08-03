// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/emplace_from/config.hpp>
#include <catch2/catch_all.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES()
import std;
#else
    #include <concepts>
    #include <type_traits>
    #include <utility>
#endif

#include <beman/emplace_from/deduce.hpp>

namespace {

using beman::emplace_from::deduce_t;
using beman::emplace_from::emplace_from;

struct ordinary_function {
    int operator()() const;
};

struct ref_qualified_function {
    int&        operator()() &;
    const int&  operator()() const&;
    int&&       operator()() &&;
    const int&& operator()() const&&;
};

struct rvalue_only_function {
    int operator()() &&;
};

struct void_function {
    void operator()() const;
};

struct non_function {};

struct immovable {
    int value;

    explicit constexpr immovable(int value) noexcept : value(value) {}

    immovable(const immovable&) = delete;
};

template <typename T>
class box {
    T value_;

  public:
    template <typename U>
    explicit constexpr box(U&& value) noexcept(noexcept(T(std::forward<U>(value)))) : value_(std::forward<U>(value)) {}

    constexpr const T& value() const noexcept { return value_; }
};

template <typename T>
box(T&&) -> box<deduce_t<T>>;

using qualified_emplacer    = emplace_from<ref_qualified_function>;
using rvalue_emplacer       = emplace_from<rvalue_only_function>;
using void_emplacer         = emplace_from<void_function>;
using non_function_emplacer = emplace_from<non_function>;

static_assert(std::same_as<deduce_t<int>, int>);
static_assert(std::same_as<deduce_t<const int>, const int>);
static_assert(std::same_as<deduce_t<int&>, int&>);
static_assert(std::same_as<deduce_t<const int&&>, const int&&>);
static_assert(std::same_as<deduce_t<ordinary_function>, ordinary_function>);

static_assert(std::same_as<deduce_t<qualified_emplacer&>, int&>);
static_assert(std::same_as<deduce_t<const qualified_emplacer&>, const int&>);
static_assert(std::same_as<deduce_t<qualified_emplacer>, int&&>);
static_assert(std::same_as<deduce_t<const qualified_emplacer>, const int&&>);

static_assert(std::same_as<deduce_t<rvalue_emplacer>, int>);
static_assert(std::same_as<deduce_t<rvalue_emplacer&>, rvalue_emplacer&>);
static_assert(std::same_as<deduce_t<const rvalue_emplacer>, const rvalue_emplacer>);
static_assert(std::same_as<deduce_t<void_emplacer>, void_emplacer>);
static_assert(std::same_as<deduce_t<non_function_emplacer>, non_function_emplacer>);

static_assert(std::same_as<decltype(box{42}), box<int>>);
static_assert(std::same_as<decltype(box{emplace_from{[] { return immovable{42}; }}}), box<immovable>>);

} // namespace

TEST_CASE("deduce leaves ordinary deduction-guide arguments unchanged", "[deduce]") {
    const auto value = box{42};

    static_assert(std::same_as<decltype(value), const box<int>>);
    CHECK(value.value() == 42);
}

TEST_CASE("deduce obtains an immovable type for a deduction guide", "[deduce]") {
    int        call_count = 0;
    const auto value      = box{emplace_from{[&call_count] {
        ++call_count;
        return immovable{42};
    }}};

    static_assert(std::same_as<decltype(value), const box<immovable>>);
    CHECK(value.value().value == 42);
    CHECK(call_count == 1);
}
