// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/Subcell/TciOnDgGrid.hpp"

#include <algorithm>

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Evolution/DgSubcell/PerssonTci.hpp"
#include "Evolution/DgSubcell/Projection.hpp"
#include "Evolution/DgSubcell/RdmpTci.hpp"

namespace BurgersVariant::subcell {
std::tuple<bool, evolution::dg::subcell::RdmpTciData> TciOnDgGrid::apply(
    const Scalar<DataVector>& dg_v, const Mesh<1>& dg_mesh,
    const Mesh<1>& subcell_mesh,
    const evolution::dg::subcell::RdmpTciData& past_rdmp_tci_data,
    const evolution::dg::subcell::SubcellOptions& subcell_options,
    double persson_exponent, [[maybe_unused]] const bool element_stays_on_dg) {
  // Don't use buffer since we have only one memory allocation right now (until
  // persson_tci can use a buffer)
  const Scalar<DataVector> subcell_v{::evolution::dg::subcell::fd::project(
      get(dg_v), dg_mesh, subcell_mesh.extents())};

  using std::max;
  using std::min;
  evolution::dg::subcell::RdmpTciData rdmp_tci_data{
      {max(max(get(dg_v)), max(get(subcell_v)))},
      {min(min(get(dg_v)), min(get(subcell_v)))}};

  const bool cell_is_troubled =
      static_cast<bool>(evolution::dg::subcell::rdmp_tci(
          rdmp_tci_data.max_variables_values,
          rdmp_tci_data.min_variables_values,
          past_rdmp_tci_data.max_variables_values,
          past_rdmp_tci_data.min_variables_values,
          subcell_options.rdmp_delta0(), subcell_options.rdmp_epsilon())) or
      ::evolution::dg::subcell::persson_tci(dg_v, dg_mesh, persson_exponent);

  return {cell_is_troubled, std::move(rdmp_tci_data)};
}
}  // namespace BurgersVariant::subcell
