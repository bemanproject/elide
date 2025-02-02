<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# beman.elide: Implementation of Proposed `std::elide`

**Implements**: `std::elide` proposed in [`std::elide` (P3288R3)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3288r3.html).

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_LIBRARY_MATURITY_MODEL.md#under-development-and-not-yet-ready-for-production-use)

## Usage

`std::elide` is an object which:

- Wraps an invocable alongside the arguments thereto (by reference), and
- Implicitly converts to whatever that invocable yields

When combined with emplacement APIs this allows for the creation of instance of immovable types
within storage managed by `std::optional`, `std::list`, et cetera.

## Building beman.elide

### Dependencies

This project has no C or C++ dependencies,
however,
it requires **C++20** or above to compile.

Build-time dependencies:

- `cmake`
- `ninja`, `make`, or another CMake-supported build system
  - CMake defaults to "Unix Makefiles" on POSIX systems

### How to build beman.elide

```shell
cmake --workflow --preset gcc-debug
cmake --workflow --preset gcc-release
cmake --install build/gcc-release --prefix /opt/beman.elide
```

## Integrate beman.elide into your project

<details>
<summary> Use beman.elide directly from C++ </summary>
<!-- TODO Darius: rewrite section!-->

If you want to use `beman.elide` from your project,
you can include `beman/elide/*.hpp`  files from your C++ source files

```cpp
#include <beman/elide/elide.hpp>
```

```shell
# Assume /opt/beman.elide staging directory.
$ c++ -o identity_usage examples/identity_usage.cpp \
    -I /opt/beman.elide/include/
```

</details>

<details>
<summary> Use beman.elide directly from CMake </summary>

<!-- TODO Darius: rewrite section! Add examples. -->

For CMake based projects, you will need to use the `beman.elide` CMake module to define the `beman::elide` CMake target:

```cmake
find_package(beman.elide REQUIRED)
```

You will also need to add `beman::elide`
to the link libraries of any libraries or executables that include `beman/elide/*.hpp` in their source or header file.

```cmake
target_link_libraries(yourlib PUBLIC beman::elide)
```

</details>

<details>
<summary> Use beman.elide from other build systems </summary>

<!-- TODO Darius: rewrite section! Add examples. -->

Build systems that support `pkg-config` by providing a `beman.elide.pc` file.
Build systems that support interoperation via `pkg-config` should be able to detect `beman.elide` for you automatically.

</details>

## Contributing

Issues and pull requests are appreciated.
