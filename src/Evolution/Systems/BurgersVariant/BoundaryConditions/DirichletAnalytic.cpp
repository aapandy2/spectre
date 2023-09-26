// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/BoundaryConditions/DirichletAnalytic.hpp"

#include <cstddef>
#include <memory>
#include <pup.h>

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Evolution/Systems/BurgersVariant/Fluxes.hpp"
#include "PointwiseFunctions/AnalyticData/BurgersVariant/Factory.hpp"
#include "PointwiseFunctions/AnalyticSolutions/BurgersVariant/Factory.hpp"
#include "Utilities/CallWithDynamicType.hpp"

namespace BurgersVariant::BoundaryConditions {
DirichletAnalytic::DirichletAnalytic(const DirichletAnalytic& rhs)
    : BoundaryCondition{dynamic_cast<const BoundaryCondition&>(rhs)},
      analytic_prescription_(rhs.analytic_prescription_->get_clone()) {}

DirichletAnalytic& DirichletAnalytic::operator=(const DirichletAnalytic& rhs) {
  if (&rhs == this) {
    return *this;
  }
  analytic_prescription_ = rhs.analytic_prescription_->get_clone();
  return *this;
}

DirichletAnalytic::DirichletAnalytic(
    std::unique_ptr<evolution::initial_data::InitialData> analytic_prescription)
    : analytic_prescription_(std::move(analytic_prescription)) {}

DirichletAnalytic::DirichletAnalytic(CkMigrateMessage* const msg)
    : BoundaryCondition(msg) {}

std::unique_ptr<domain::BoundaryConditions::BoundaryCondition>
DirichletAnalytic::get_clone() const {
  return std::make_unique<DirichletAnalytic>(*this);
}

std::optional<std::string> DirichletAnalytic::dg_ghost(
    const gsl::not_null<Scalar<DataVector>*> v,
    const gsl::not_null<tnsr::I<DataVector, 1, Frame::Inertial>*> flux_v,
    const std::optional<
        tnsr::I<DataVector, 1, Frame::Inertial>>& /*face_mesh_velocity*/,
    const tnsr::i<DataVector, 1, Frame::Inertial>& /*normal_covector*/,
    const tnsr::I<DataVector, 1, Frame::Inertial>& coords,
    [[maybe_unused]] const double time) const {
  call_with_dynamic_type<void, tmpl::append<BurgersVariant::Solutions::
                                     all_solutions,
                               BurgersVariant::AnalyticData::all_data>>(
      analytic_prescription_.get(),
      [&coords, &time, &v](const auto* const analytic_solution_or_data) {
        if constexpr (is_analytic_solution_v<
                          std::decay_t<decltype(*analytic_solution_or_data)>>) {
          *v = get<BurgersVariant::Tags::
               V>(analytic_solution_or_data->variables(
              coords, time, tmpl::list<BurgersVariant::Tags::V>{}));
        } else {
          *v = get<BurgersVariant::Tags::
               V>(analytic_solution_or_data->variables(
              coords, tmpl::list<BurgersVariant::Tags::V>{}));
          (void)time;
        }
      });
  flux_impl(flux_v, *v);
  return {};
}

void DirichletAnalytic::fd_ghost(
    const gsl::not_null<Scalar<DataVector>*> v, const Direction<1>& direction,
    const Mesh<1> subcell_mesh, const double time,
    const std::unordered_map<
        std::string, std::unique_ptr<domain::FunctionsOfTime::FunctionOfTime>>&
        functions_of_time,
    const ElementMap<1, Frame::Grid>& logical_to_grid_map,
    const domain::CoordinateMapBase<Frame::Grid, Frame::Inertial, 1>&
        grid_to_inertial_map,
    const fd::Reconstructor& reconstructor) const {
  const size_t ghost_zone_size{reconstructor.ghost_zone_size()};

  const auto ghost_logical_coords =
      evolution::dg::subcell::fd::ghost_zone_logical_coordinates(
          subcell_mesh, ghost_zone_size, direction);

  const auto ghost_inertial_coords = grid_to_inertial_map(
      logical_to_grid_map(ghost_logical_coords), time, functions_of_time);

  call_with_dynamic_type<void, tmpl::append<BurgersVariant::
                                    Solutions::all_solutions,
                                BurgersVariant::AnalyticData::all_data>>(
      analytic_prescription_.get(),
      [&ghost_inertial_coords, &time,
       &v](const auto* const analytic_solution_or_data) {
        if constexpr (is_analytic_solution_v<
                          std::decay_t<decltype(*analytic_solution_or_data)>>) {
          *v = get<BurgersVariant::Tags::
          V>(analytic_solution_or_data->variables(
              ghost_inertial_coords, time, tmpl::
              list<BurgersVariant::Tags::V>{}));
        } else {
          *v = get<BurgersVariant::Tags::
          V>(analytic_solution_or_data->variables(
              ghost_inertial_coords, tmpl::list<BurgersVariant::Tags::V>{}));
          (void)time;
        }
      });
}

void DirichletAnalytic::pup(PUP::er& p) {
  BoundaryCondition::pup(p);
  p | analytic_prescription_;
}

void DirichletAnalytic::flux_impl(
    const gsl::not_null<tnsr::I<DataVector, 1, Frame::Inertial>*> flux,
    const Scalar<DataVector>& v_analytic) {
  BurgersVariant::Fluxes::apply(flux, v_analytic);
}

// NOLINTNEXTLINE
PUP::able::PUP_ID DirichletAnalytic::my_PUP_ID = 0;
}  // namespace BurgersVariant::BoundaryConditions
