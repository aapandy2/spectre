// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/TimeDerivativeTerms.hpp"

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Utilities/Gsl.hpp"

namespace BurgersVariant {
void TimeDerivativeTerms::apply(
    const gsl::not_null<Scalar<DataVector>*> /*non_flux_terms_dt_vars*/,
    const gsl::not_null<tnsr::I<DataVector, 1, Frame::Inertial>*> flux_u,
    const Scalar<DataVector>& u) {
    get<0>(*flux_u) = 2./3. * pow(get(u), 3./2.);
}
}  // namespace BurgersVariant
