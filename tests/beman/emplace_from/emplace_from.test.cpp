// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/emplace_from/config.hpp>
#include <catch2/catch_all.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES()
import std;
#else
    #include <concepts>
    #include <optional>
    #include <type_traits>
    #include <utility>
#endif

#include <beman/emplace_from/emplace_from.hpp>

namespace {

using beman::emplace_from::emplace_from;

struct immovable {
    int value;

    explicit constexpr immovable(int value) noexcept : value(value) {}

    immovable(const immovable&)            = delete;
    immovable(immovable&&)                 = delete;
    immovable& operator=(const immovable&) = delete;
    immovable& operator=(immovable&&)      = delete;
};

enum class call_kind { mutable_lvalue, const_lvalue, mutable_rvalue, const_rvalue };

struct ref_qualified_function {
    constexpr call_kind operator()() & noexcept { return call_kind::mutable_lvalue; }
    constexpr call_kind operator()() const& noexcept { return call_kind::const_lvalue; }
    constexpr call_kind operator()() && noexcept { return call_kind::mutable_rvalue; }
    constexpr call_kind operator()() const&& noexcept { return call_kind::const_rvalue; }
};

struct nothrow_function {
    constexpr int operator()() const noexcept { return 42; }
};

struct throwing_function {
    int operator()() const { return 42; }
};

struct rvalue_only_function {
    constexpr int operator()() && noexcept { return 42; }
};

struct void_function {
    constexpr void operator()() const noexcept {}
};

struct non_function {};

struct immovable_function {
    constexpr immovable operator()() const noexcept { return immovable{42}; }
};

struct move_only_function {
    move_only_function()                          = default;
    move_only_function(const move_only_function&) = delete;
    move_only_function(move_only_function&&)      = default;

    constexpr int operator()() const noexcept { return 42; }
};

struct mutable_only_function {
    constexpr int operator()() noexcept { return 42; }
};

struct non_default_function {
    explicit non_default_function(int) noexcept {}

    constexpr int operator()() const noexcept { return 42; }
};

struct throwing_default_function {
    throwing_default_function() noexcept(false) {}
    throwing_default_function(const throwing_default_function&)     = delete;
    throwing_default_function(throwing_default_function&&) noexcept = default;

    constexpr int operator()() const noexcept { return 42; }
};

struct expected_error {};

struct error_function {
    int operator()() const { throw expected_error{}; }
};

constexpr int converted_constant() {
    const int value = emplace_from{nothrow_function{}};
    return value;
}

using nothrow_emplacer          = emplace_from<nothrow_function>;
using throwing_emplacer         = emplace_from<throwing_function>;
using rvalue_only_emplacer      = emplace_from<rvalue_only_function>;
using void_emplacer             = emplace_from<void_function>;
using non_function_emplacer     = emplace_from<non_function>;
using immovable_emplacer        = emplace_from<immovable_function>;
using move_only_emplacer        = emplace_from<move_only_function>;
using mutable_only_emplacer     = emplace_from<mutable_only_function>;
using non_default_emplacer      = emplace_from<non_default_function>;
using throwing_default_emplacer = emplace_from<throwing_default_function>;

static_assert(emplace_from{nothrow_function{}}() == 42);
static_assert(converted_constant() == 42);
static_assert(std::same_as<decltype(emplace_from{nothrow_function{}}), nothrow_emplacer>);
static_assert(std::default_initializable<nothrow_emplacer>);
static_assert(!std::default_initializable<non_default_emplacer>);
static_assert(std::constructible_from<move_only_emplacer, move_only_function>);
static_assert(!std::copy_constructible<move_only_emplacer>);
static_assert(std::invocable<mutable_only_emplacer&>);
static_assert(!std::invocable<const mutable_only_emplacer&>);
static_assert(std::convertible_to<mutable_only_emplacer&, int>);
static_assert(!std::convertible_to<const mutable_only_emplacer&, int>);
static_assert(noexcept(throwing_default_emplacer{std::declval<throwing_default_function>()}));
static_assert(noexcept(std::declval<nothrow_emplacer&>()()));
static_assert(!noexcept(std::declval<throwing_emplacer&>()()));
static_assert(std::convertible_to<nothrow_emplacer&, int>);
static_assert(std::convertible_to<throwing_emplacer&, int>);
static_assert(std::is_nothrow_convertible_v<nothrow_emplacer&, int>);
static_assert(!std::is_nothrow_convertible_v<throwing_emplacer&, int>);
static_assert(!std::invocable<rvalue_only_emplacer&>);
static_assert(std::invocable<rvalue_only_emplacer&&>);
static_assert(!std::convertible_to<rvalue_only_emplacer&, int>);
static_assert(std::convertible_to<rvalue_only_emplacer&&, int>);
static_assert(!std::invocable<void_emplacer&>);
static_assert(!std::invocable<non_function_emplacer&>);
static_assert(!std::convertible_to<void_emplacer&, void>);
static_assert(!std::convertible_to<non_function_emplacer&, int>);
static_assert(std::convertible_to<immovable_emplacer, immovable>);

} // namespace

TEST_CASE("emplace_from invokes its function", "[emplace_from]") {
    int  call_count = 0;
    auto value      = emplace_from{[&call_count]() noexcept {
        ++call_count;
        return 42;
    }};

    CHECK(value() == 42);
    CHECK(call_count == 1);
}

TEST_CASE("emplace_from preserves the function's cvref qualification", "[emplace_from]") {
    auto       value       = emplace_from{ref_qualified_function{}};
    const auto const_value = emplace_from{ref_qualified_function{}};

    CHECK(value() == call_kind::mutable_lvalue);
    CHECK(std::as_const(value)() == call_kind::const_lvalue);
    CHECK(std::move(value)() == call_kind::mutable_rvalue);
    CHECK(std::move(const_value)() == call_kind::const_rvalue);
}

TEST_CASE("emplace_from conversion preserves the function's cvref qualification", "[emplace_from]") {
    auto            mutable_lvalue        = emplace_from{ref_qualified_function{}};
    const auto      const_lvalue          = emplace_from{ref_qualified_function{}};
    auto            mutable_rvalue        = emplace_from{ref_qualified_function{}};
    const auto      const_rvalue          = emplace_from{ref_qualified_function{}};
    const call_kind mutable_lvalue_result = mutable_lvalue;
    const call_kind const_lvalue_result   = const_lvalue;
    const call_kind mutable_rvalue_result = std::move(mutable_rvalue);
    const call_kind const_rvalue_result   = std::move(const_rvalue);

    CHECK(mutable_lvalue_result == call_kind::mutable_lvalue);
    CHECK(const_lvalue_result == call_kind::const_lvalue);
    CHECK(mutable_rvalue_result == call_kind::mutable_rvalue);
    CHECK(const_rvalue_result == call_kind::const_rvalue);
}

TEST_CASE("emplace_from preserves reference results", "[emplace_from]") {
    int        source = 42;
    auto       value  = emplace_from{[&source]() noexcept -> int& { return source; }};
    const int& result = value;

    CHECK(&result == &source);
}

TEST_CASE("emplace_from stores a move-only function", "[emplace_from]") {
    auto value = emplace_from{move_only_function{}};

    CHECK(value() == 42);
}

TEST_CASE("emplace_from converts to an immovable result", "[emplace_from]") {
    int             call_count = 0;
    const immovable value      = emplace_from{[&call_count] {
        ++call_count;
        return immovable{42};
    }};

    CHECK(value.value == 42);
    CHECK(call_count == 1);
}

TEST_CASE("emplace_from emplaces an immovable result", "[emplace_from]") {
    int                            call_count = 0;
    const std::optional<immovable> value{
        std::in_place,
        emplace_from{[&call_count] {
            ++call_count;
            return immovable{42};
        }},
    };

    REQUIRE(value.has_value());
    CHECK(value->value == 42);
    CHECK(call_count == 1);
}

TEST_CASE("emplace_from propagates exceptions", "[emplace_from]") {
    auto invoked   = emplace_from{error_function{}};
    auto converted = emplace_from{error_function{}};

    CHECK_THROWS_AS(invoked(), expected_error);
    CHECK_THROWS_AS(static_cast<int>(converted), expected_error);
}
