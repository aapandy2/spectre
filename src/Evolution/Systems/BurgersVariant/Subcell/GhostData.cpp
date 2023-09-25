// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/Subcell/GhostData.hpp"

#include "DataStructures/Variables.hpp"
#include "Evolution/Systems/BurgersVariant/Tags.hpp"
#include "Utilities/TMPL.hpp"

namespace BurgersVariant::subcell {
DataVector GhostVariables::apply(
    const Variables<tmpl::list<BurgersVariant::Tags::U>>& vars,
    const size_t rdmp_size) {
  DataVector buffer{vars.number_of_grid_points() + rdmp_size};
  DataVector var_view{buffer.data(), vars.number_of_grid_points()};
  var_view = get(get<BurgersVariant::Tags::U>(vars));
  return buffer;
}
}  // namespace BurgersVariant::subcell
