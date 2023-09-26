// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/BoundaryConditions/Dirichlet.hpp"

#include <memory>
#include <optional>
#include <pup.h>

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Domain/BoundaryConditions/BoundaryCondition.hpp"
#include "Evolution/Systems/BurgersVariant/Fluxes.hpp"
#include "Utilities/Gsl.hpp"

namespace BurgersVariant::BoundaryConditions {
Dirichlet::Dirichlet(const double v_value) : v_value_(v_value) {}

Dirichlet::Dirichlet(CkMigrateMessage* const msg) : BoundaryCondition(msg) {}

std::unique_ptr<domain::BoundaryConditions::BoundaryCondition>
Dirichlet::get_clone() const {
  return std::make_unique<Dirichlet>(*this);
}

void Dirichlet::pup(PUP::er& p) {
  BoundaryCondition::pup(p);
  p | v_value_;
}

std::optional<std::string> Dirichlet::dg_ghost(
    const gsl::not_null<Scalar<DataVector>*> v,
    const gsl::not_null<tnsr::I<DataVector, 1, Frame::Inertial>*> flux_v,
    const std::optional<
        tnsr::I<DataVector, 1, Frame::Inertial>>& /*face_mesh_velocity*/,
    const tnsr::i<DataVector, 1, Frame::Inertial>& /*normal_covector*/) const {
  get(*v) = v_value_;
  BurgersVariant::Fluxes::apply(flux_v, *v);
  return {};
}

void Dirichlet::fd_ghost(const gsl::not_null<Scalar<DataVector>*> v,
                         const Direction<1>& /*direction*/) const {
  get(*v) = v_value_;
}

// NOLINTNEXTLINE
PUP::able::PUP_ID Dirichlet::my_PUP_ID = 0;
}  // namespace BurgersVariant::BoundaryConditions
