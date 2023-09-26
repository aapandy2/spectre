// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/Subcell/SetInitialRdmpData.hpp"

#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "DataStructures/Variables.hpp"
#include "Evolution/DgSubcell/ActiveGrid.hpp"
#include "Evolution/DgSubcell/PerssonTci.hpp"
#include "Evolution/DgSubcell/Projection.hpp"
#include "Evolution/DgSubcell/TwoMeshRdmpTci.hpp"
#include "NumericalAlgorithms/Spectral/Mesh.hpp"
#include "Utilities/Gsl.hpp"

namespace BurgersVariant::subcell {
void SetInitialRdmpData::apply(
    const gsl::not_null<evolution::dg::subcell::RdmpTciData*> rdmp_tci_data,
    const Scalar<DataVector>& v,
    const evolution::dg::subcell::ActiveGrid active_grid,
    const Mesh<1>& dg_mesh, const Mesh<1>& subcell_mesh) {
  if (active_grid == evolution::dg::subcell::ActiveGrid::Subcell) {
    *rdmp_tci_data = {{max(get(v))}, {min(get(v))}};
  } else {
    using std::max;
    using std::min;
    const auto subcell_v = evolution::dg::subcell::fd::project(
        get(v), dg_mesh, subcell_mesh.extents());

    *rdmp_tci_data = {{max(max(get(v)), max(subcell_v))},
                      {min(min(get(v)), min(subcell_v))}};
  }
}
}  // namespace BurgersVariant::subcell
