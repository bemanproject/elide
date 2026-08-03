// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/emplace_from/config.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES()
import std;
#else
    #include <concepts>
    #include <iostream>
    #include <mutex>
    #include <type_traits>
    #include <utility>
#endif

#include <beman/emplace_from/emplace_from.hpp>
#include <beman/emplace_from/deduce.hpp>

class state {
    int value_;

  public:
    explicit state(int value) noexcept : value_(value) {}

    state(const state&) = delete;

    void advance() noexcept { ++value_; }
    int  value() const noexcept { return value_; }
};

state make_state() { return state{41}; }

template <typename T>
class synchronized_value {
    T          value_;
    std::mutex mutex_;

  public:
    template <typename U>
    explicit synchronized_value(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>)
        : value_(std::forward<U>(value)) {}

    template <typename Fun>
    void with_lock(Fun&& fun) {
        const std::lock_guard lock(mutex_);
        std::forward<Fun>(fun)(value_);
    }
};

template <typename T>
synchronized_value(T) -> synchronized_value<beman::emplace_from::deduce_t<T>>;

int main() {
    auto value = synchronized_value{
        // The result of make_state is constructed directly in synchronized_value::value_.
        beman::emplace_from::emplace_from{make_state},
    };

    static_assert(std::same_as<decltype(value), synchronized_value<state>>);

    value.with_lock([](state& current) {
        current.advance();
        std::cout << "State: " << current.value() << '\n';
    });
}
