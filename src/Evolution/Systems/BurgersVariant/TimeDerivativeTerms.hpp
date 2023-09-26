// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "DataStructures/DataBox/Prefixes.hpp"  // IWYU pragma: keep
#include "DataStructures/Tensor/TypeAliases.hpp"
#include "Utilities/TMPL.hpp"

/// \cond
class DataVector;
// IWYU pragma: no_forward_declare Tensor
namespace BurgersVariant {
namespace Tags {
struct V;
}  // namespace Tags
}  // namespace BurgersVariant
namespace gsl {
template <typename T>
class not_null;
}  // namespace gsl

class DataVector;
/// \endcond

namespace BurgersVariant {
/// Computes the time derivative terms needed for the BurgersVariant system,
// which are just the fluxes.
struct TimeDerivativeTerms {
  using temporary_tags = tmpl::list<>;
  using argument_tags = tmpl::list<Tags::V>;

  static void apply(
      // Time derivatives returned by reference. No source terms or
      // nonconservative products, so not used. All the tags in the
      // variables_tag in the system struct.
      gsl::not_null<Scalar<DataVector>*> /*non_flux_terms_dt_vars*/,

      // Fluxes returned by reference. Listed in the system struct as
      // flux_variables.
      gsl::not_null<tnsr::I<DataVector, 1, Frame::Inertial>*> flux_v,

      // Arguments listed in argument_tags above
      const Scalar<DataVector>& v);
};
}  // namespace BurgersVariant
