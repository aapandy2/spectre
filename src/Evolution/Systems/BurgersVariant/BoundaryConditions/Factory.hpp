// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "Domain/BoundaryConditions/Periodic.hpp"
#include "Evolution/Systems/BurgersVariant/BoundaryConditions/BoundaryCondition.hpp"
#include "Evolution/Systems/BurgersVariant/BoundaryConditions/DemandOutgoingCharSpeeds.hpp"
#include "Evolution/Systems/BurgersVariant/BoundaryConditions/Dirichlet.hpp"
#include "Evolution/Systems/BurgersVariant/BoundaryConditions/DirichletAnalytic.hpp"
#include "Utilities/TMPL.hpp"

namespace BurgersVariant::BoundaryConditions {
/// Typelist of standard BoundaryConditions
using standard_boundary_conditions =
    tmpl::list<DemandOutgoingCharSpeeds, Dirichlet, DirichletAnalytic,
               domain::BoundaryConditions::Periodic<BoundaryCondition>>;
}  // namespace BurgersVariant::BoundaryConditions
