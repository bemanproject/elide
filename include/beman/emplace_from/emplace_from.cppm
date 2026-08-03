export module beman.emplace_from;

import std;

#define BEMAN_EMPLACE_FROM_INCLUDED_FROM_INTERFACE_UNIT
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#include <beman/emplace_from/detail/emplace_from.hpp>
export {
#include <beman/emplace_from/emplace_from.hpp>
}
#include <beman/emplace_from/detail/deduce.hpp>
export {
#include <beman/emplace_from/deduce.hpp>
}
#pragma clang diagnostic pop
