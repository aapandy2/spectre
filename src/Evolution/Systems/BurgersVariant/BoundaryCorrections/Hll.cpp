// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/BoundaryCorrections/Hll.hpp"

#include <memory>
#include <optional>
#include <pup.h>

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tags/TempTensor.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "DataStructures/Variables.hpp"
#include "NumericalAlgorithms/DiscontinuousGalerkin/Formulation.hpp"
#include "NumericalAlgorithms/DiscontinuousGalerkin/NormalDotFlux.hpp"
#include "Utilities/Gsl.hpp"

namespace BurgersVariant::BoundaryCorrections {
Hll::Hll(CkMigrateMessage* msg) : BoundaryCorrection(msg) {}

std::unique_ptr<BoundaryCorrection> Hll::get_clone() const {
  return std::make_unique<Hll>(*this);
}

void Hll::pup(PUP::er& p) { BoundaryCorrection::pup(p); }

double Hll::dg_package_data(
    const gsl::not_null<Scalar<DataVector>*> packaged_v,
    const gsl::not_null<Scalar<DataVector>*> packaged_normal_dot_flux_v,
    const gsl::not_null<Scalar<DataVector>*> packaged_char_speed,
    const Scalar<DataVector>& v,
    const tnsr::I<DataVector, 1, Frame::Inertial>& flux_v,
    const tnsr::i<DataVector, 1, Frame::Inertial>& normal_covector,
    const std::optional<tnsr::I<DataVector, 1, Frame::Inertial>>&
    /*mesh_velocity*/,
    const std::optional<Scalar<DataVector>>& normal_dot_mesh_velocity) {
  get(*packaged_char_speed) = sign(get<0>(normal_covector)) * get(v);
  if (normal_dot_mesh_velocity.has_value()) {
    get(*packaged_char_speed) -= get(*normal_dot_mesh_velocity);
  }
  *packaged_v = v;
  normal_dot_flux(packaged_normal_dot_flux_v, normal_covector, flux_v);
  return max(abs(get(*packaged_char_speed)));
}

void Hll::dg_boundary_terms(
    const gsl::not_null<Scalar<DataVector>*> boundary_correction_v,
    const Scalar<DataVector>& v_int,
    const Scalar<DataVector>& normal_dot_flux_v_int,
    const Scalar<DataVector>& char_speed_int, const Scalar<DataVector>& v_ext,
    const Scalar<DataVector>& normal_dot_flux_v_ext,
    const Scalar<DataVector>& char_speed_ext,
    const dg::Formulation dg_formulation) {
  Variables<tmpl::list<::Tags::TempScalar<0>, ::Tags::TempScalar<1>>> temps{
      get(v_int).size()};
  get(get<::Tags::TempScalar<0>>(temps)) =
      min(0.0, get(char_speed_int), -get(char_speed_ext));
  get(get<::Tags::TempScalar<1>>(temps)) =
      max(0.0, get(char_speed_int), -get(char_speed_ext));
  const DataVector& lambda_min = get(get<::Tags::TempScalar<0>>(temps));
  const DataVector& lambda_max = get(get<::Tags::TempScalar<1>>(temps));

  get(*boundary_correction_v) =
      ((lambda_max * get(normal_dot_flux_v_int) +
        lambda_min * get(normal_dot_flux_v_ext)) +
       lambda_max * lambda_min * (get(v_ext) - get(v_int))) /
      (lambda_max - lambda_min);
  if (dg_formulation == dg::Formulation::StrongInertial) {
    get(*boundary_correction_v) -= get(normal_dot_flux_v_int);
  }
}
}  // namespace BurgersVariant::BoundaryCorrections

// NOLINTNEXTLINE
PUP::able::PUP_ID BurgersVariant::BoundaryCorrections::Hll::my_PUP_ID = 0;
