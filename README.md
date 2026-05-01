# beman.elide: Implementation of Proposed `std::elide`

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/elide/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/elide/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/elide/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/elide?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

**Implements**: `std::elide` proposed in [`std::elide` (P3288R3)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3288r3.html).

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.elide` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

`std::elide` is an object which:

- Wraps an invocable alongside the arguments thereto (by reference), and
- Implicitly converts to whatever that invocable yields

When combined with emplacement APIs this allows for the creation of instances of immovable types
within storage managed by `std::optional`, `std::list`, et cetera.

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++20 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_ELIDE_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 15-13   | C++26-C++20   | libstdc++         |
| GCC        | 12-11   | C++23, C++20  | libstdc++         |
| Clang      | 22-19   | C++26-C++20   | libstdc++, libc++ |
| Clang      | 18      | C++26-C++20   | libc++            |
| Clang      | 18      | C++23, C++20  | libstdc++         |
| Clang      | 17      | C++26-C++20   | libc++            |
| Clang      | 17      | C++20         | libstdc++         |
| AppleClang | latest  | C++26-C++20   | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.elide into your project

### Build

You can build elide using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.elide without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

#### Vcpkg

The preferred way to install elide is via vcpkg. To do so, after installing vcpkg
itself, you need to add support for the Beman project's [vcpkg
registry](https://github.com/bemanproject/vcpkg-registry) by configuring a
`vcpkg-configuration.json` file (which elide [provides](vcpkg-configuration.json)).

Then, simply run `vcpkg install beman-elide`.

#### Manual

To install beman.elide globally after building with the `gcc-release` preset, you can
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
│       └── elide
│           ├── elide.hpp
│           └── ...
└── lib
    └── cmake
        └── beman.elide
            ├── beman.elide-config-version.cmake
            ├── beman.elide-config.cmake
            └── beman.elide-targets.cmake
```

### CMake Configuration

If you installed beman.elide to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.elide` package to define the `beman::elide` CMake
target:

```cmake
find_package(beman.elide REQUIRED)
```

You will then need to add `beman::elide` to the link libraries of any libraries or
executables that include `beman.elide` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::elide)
```

### Using beman.elide

To use `beman.elide` in your C++ project,
include an appropriate `beman.elide` header from your source code.

```c++
#include <beman/elide/elide.hpp>
```

> [!NOTE]
>
> `beman.elide` headers are to be included with the `beman/elide/` prefix.
> Altering include search paths to spell the include target another way (e.g.
> `#include <elide.hpp>`) is unsupported.
