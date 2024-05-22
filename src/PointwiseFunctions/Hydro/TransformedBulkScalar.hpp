
// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>

#include "DataStructures/Tensor/Tensor.hpp"
#include "PointwiseFunctions/Hydro/TagsDeclarations.hpp"
#include "Utilities/TMPL.hpp"
#include "Utilities/TaggedTuple.hpp"

namespace hydro {

/// @{
/*!
 * \brief Wrapper to add TransformedBulkScalar (set equal to 1,
 * corresponding to a fluid initially in thermodynamic equilibrium)
 * to the initial data.
 */

class TransformedBulkScalarInitialization {
 public:
  template <typename DataType, size_t Dim>
  auto variables(
      const tnsr::I<DataType, Dim>& x,
      tmpl::list<hydro::Tags::TransformedBulkScalar<DataType>> /*meta*/) const
      -> tuples::TaggedTuple<hydro::Tags::TransformedBulkScalar<DataType>> {
    return {make_with_value<Scalar<DataType>>(x, 1.0)};
  }

  template <typename DataType, size_t Dim>
  auto variables(
      const tnsr::I<DataType, Dim>& x, const double /*t*/,
      tmpl::list<hydro::Tags::TransformedBulkScalar<DataType>> /*meta*/) const
      -> tuples::TaggedTuple<hydro::Tags::TransformedBulkScalar<DataType>> {
    return {make_with_value<Scalar<DataType>>(x, 1.0)};
  }

  template <typename ExtraVars, typename DataType, size_t Dim, typename... Args>
  auto variables(
      ExtraVars& extra_variables, const tnsr::I<DataType, Dim>& x,
      Args&... extra_args,
      tmpl::list<hydro::Tags::TransformedBulkScalar<DataType>> /*meta*/) const
      -> tuples::TaggedTuple<hydro::Tags::TransformedBulkScalar<DataType>> {
    return {make_with_value<Scalar<DataType>>(x, 1.0)};
  }
};
/// @}

}  // namespace hydro
