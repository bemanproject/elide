// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/emplace_from/config.hpp>

#if BEMAN_EMPLACE_FROM_USE_MODULES()
import std;
#else
    #include <iostream>
    #include <optional>
    #include <string>
    #include <utility>
#endif

#include <beman/emplace_from/emplace_from.hpp>

class database {
  public:
    database(const database&) = delete;

    static database connect(std::string endpoint) { return database(std::move(endpoint)); }

    const std::string& endpoint() const noexcept { return endpoint_; }

  private:
    explicit database(std::string endpoint) : endpoint_(std::move(endpoint)) {}

    std::string endpoint_;
};

int main() {
    const std::optional<database> db{
        std::in_place,
        beman::emplace_from::emplace_from{[] { return database::connect("postgres://localhost/example"); }},
    };

    std::cout << "Connected to " << db->endpoint() << '\n';
}
