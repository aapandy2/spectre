// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <pybind11/pybind11.h>

namespace py_bindings {
// NOLINTNEXTLINE(google-runtime-references)
void bind_info_at_compile(pybind11::module& m);
}  // namespace py_bindings
