# beman.emplace_from: Utility to enable emplacement of immovable objects returned by a function via guaranteed RVO.

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable line-length -->
[![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg)](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#the-beman-library-maturity-model)
[![Continuous Integration Tests](https://github.com/bemanproject/emplace_from/actions/workflows/ci_tests.yml/badge.svg)](https://github.com/bemanproject/emplace_from/actions/workflows/ci_tests.yml)
[![Lint Check (pre-commit)](https://github.com/bemanproject/emplace_from/actions/workflows/pre-commit-check.yml/badge.svg)](https://github.com/bemanproject/emplace_from/actions/workflows/pre-commit-check.yml)
[![Coverage](https://coveralls.io/repos/github/bemanproject/emplace_from/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/emplace_from?branch=main)
![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

<!-- markdownlint-restore -->

`beman.emplace_from` is provides a non-exposition-only version of the emplace-from helper used by [`std::execution`](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2300r10.html).
This helper can be used to construct immovable objects via emplace-style APIs by leveraging guaranteed RVO.

**Implements**: `std::emplace_from` and `std::deduce` proposed in P4337R0 and P4338R0, respectively.

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.emplace_from` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

Full runnable examples can be found in [`examples/`](examples/).

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_EMPLACE_FROM_BUILD_TESTS` to
`OFF` when configuring the project.

You can disable building examples by setting CMake option `BEMAN_EMPLACE_FROM_BUILD_EXAMPLES` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 16-14   | C++26, C++23  | libstdc++         |
| Clang      | 22-19   | C++26, C++23  | libstdc++, libc++ |
| Clang      | 18      | C++26, C++23  | libc++            |
| Clang      | 18      | C++23         | libstdc++         |
| AppleClang | latest  | C++26, C++23  | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.emplace_from into your project

### Build

You can build emplace_from using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.emplace_from without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

#### Vcpkg

The preferred way to install emplace_from is via vcpkg. To do so, after installing vcpkg
itself, you need to add support for the Beman project's [vcpkg
registry](https://github.com/bemanproject/vcpkg-registry) by configuring a
`vcpkg-configuration.json` file (which emplace_from [provides](vcpkg-configuration.json)).

Then, simply run `vcpkg install beman-emplace-from`.

#### Manual

To install beman.emplace_from globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

```txt
/opt/beman
├── include
│   └── beman
│       └── emplace_from
│           ├── emplace_from.hpp
│           └── ...
└── lib
    └── cmake
        └── beman.emplace_from
            ├── beman.emplace_from-config-version.cmake
            ├── beman.emplace_from-config.cmake
            └── beman.emplace_from-targets.cmake
```

### CMake Configuration

If you installed beman.emplace_from to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.emplace_from` package to define the `beman::emplace_from` CMake
target:

```cmake
find_package(beman.emplace_from REQUIRED)
```

You will then need to add `beman::emplace_from` to the link libraries of any libraries or
executables that include `beman.emplace_from` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::emplace_from)
```

### Using beman.emplace_from

To use `beman.emplace_from` in your C++ project,
include an appropriate `beman.emplace_from` header from your source code.

```c++
#include <beman/emplace_from/emplace_from.hpp>
```
