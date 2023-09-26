// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/BoundaryCorrections/Rusanov.hpp"

#include <memory>
#include <optional>
#include <pup.h>

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "NumericalAlgorithms/DiscontinuousGalerkin/Formulation.hpp"
#include "NumericalAlgorithms/DiscontinuousGalerkin/NormalDotFlux.hpp"
#include "Utilities/Gsl.hpp"

namespace BurgersVariant::BoundaryCorrections {
Rusanov::Rusanov(CkMigrateMessage* msg) : BoundaryCorrection(msg) {}

std::unique_ptr<BoundaryCorrection> Rusanov::get_clone() const {
  return std::make_unique<Rusanov>(*this);
}

void Rusanov::pup(PUP::er& p) { BoundaryCorrection::pup(p); }

double Rusanov::dg_package_data(
    const gsl::not_null<Scalar<DataVector>*> packaged_v,
    const gsl::not_null<Scalar<DataVector>*> packaged_normal_dot_flux_v,
    const gsl::not_null<Scalar<DataVector>*> packaged_abs_char_speed,
    const Scalar<DataVector>& v,
    const tnsr::I<DataVector, 1, Frame::Inertial>& flux_v,
    const tnsr::i<DataVector, 1, Frame::Inertial>& normal_covector,
    const std::optional<tnsr::I<DataVector, 1, Frame::Inertial>>&
    /*mesh_velocity*/,
    const std::optional<Scalar<DataVector>>& normal_dot_mesh_velocity) {
  if (normal_dot_mesh_velocity.has_value()) {
    get(*packaged_abs_char_speed) =
        abs(get(v) - get(*normal_dot_mesh_velocity));
  } else {
    get(*packaged_abs_char_speed) = abs(get(v));
  }
  *packaged_v = v;
  normal_dot_flux(packaged_normal_dot_flux_v, normal_covector, flux_v);
  return max(get(*packaged_abs_char_speed));
}

void Rusanov::dg_boundary_terms(
    const gsl::not_null<Scalar<DataVector>*> boundary_correction_v,
    const Scalar<DataVector>& v_int,
    const Scalar<DataVector>& normal_dot_flux_v_int,
    const Scalar<DataVector>& abs_char_speed_int,
    const Scalar<DataVector>& v_ext,
    const Scalar<DataVector>& normal_dot_flux_v_ext,
    const Scalar<DataVector>& abs_char_speed_ext,
    const dg::Formulation dg_formulation) {
  if (dg_formulation == dg::Formulation::WeakInertial) {
    get(*boundary_correction_v) =
        0.5 * (get(normal_dot_flux_v_int) - get(normal_dot_flux_v_ext)) -
        0.5 * max(get(abs_char_speed_int), get(abs_char_speed_ext)) *
            (get(v_ext) - get(v_int));
  } else {
    get(*boundary_correction_v) =
        -0.5 * (get(normal_dot_flux_v_int) + get(normal_dot_flux_v_ext)) -
        0.5 * max(get(abs_char_speed_int), get(abs_char_speed_ext)) *
            (get(v_ext) - get(v_int));
  }
}
}  // namespace BurgersVariant::BoundaryCorrections

// NOLINTNEXTLINE
PUP::able::PUP_ID BurgersVariant::BoundaryCorrections::Rusanov::my_PUP_ID = 0;
