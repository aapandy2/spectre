// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "PointwiseFunctions/AnalyticSolutions/BurgersVariant/Bump.hpp"
#include "PointwiseFunctions/AnalyticSolutions/BurgersVariant/Linear.hpp"
#include "PointwiseFunctions/AnalyticSolutions/BurgersVariant/Step.hpp"
#include "Utilities/TMPL.hpp"

namespace BurgersVariant::Solutions {
/// \brief List of all analytic solutions
using all_solutions = tmpl::list<Bump, Linear, Step>;
}  // namespace BurgersVariant::Solutions
