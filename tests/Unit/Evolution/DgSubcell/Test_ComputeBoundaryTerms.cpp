// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include "DataStructures/DataBox/AsAccess.hpp"
#include "DataStructures/DataBox/DataBox.hpp"
#include "DataStructures/DataBox/Tag.hpp"
#include "DataStructures/DataVector.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "DataStructures/Variables.hpp"
#include "Evolution/DgSubcell/ComputeBoundaryTerms.hpp"
#include "Framework/TestHelpers.hpp"
#include "NumericalAlgorithms/DiscontinuousGalerkin/Formulation.hpp"
#include "Utilities/Gsl.hpp"

namespace {
struct Var1 : db::SimpleTag {
  using type = Scalar<DataVector>;
};

struct Number : db::SimpleTag {
  using type = int;
};

class BoundaryCorrection {
 public:
  explicit BoundaryCorrection(const double multiply) : multiply_(multiply) {}
  using dg_boundary_terms_volume_tags = tmpl::list<Number>;
  void dg_boundary_terms(
      const gsl::not_null<Scalar<DataVector>*> correction_var1,
      const Scalar<DataVector>& upper_var1,
      const Scalar<DataVector>& lower_var1,
      const dg::Formulation dg_formulation, const int number) const {
    get(*correction_var1) = get(upper_var1) + multiply_ * get(lower_var1);
    CHECK(dg_formulation == dg::Formulation::WeakInertial);
    CHECK(number == 10);
  }

 private:
  double multiply_ = std::numeric_limits<double>::signaling_NaN();
};
}  // namespace

SPECTRE_TEST_CASE("Unit.Evolution.Subcell.ComputeBoundaryTerms",
                  "[Evolution][Unit]") {
  const auto box =
      db::create<db::AddSimpleTags<Number>, db::AddComputeTags<>>(10);
  const BoundaryCorrection correction{5.0};
  const size_t num_pts = 5;
  Variables<tmpl::list<Var1>> corrections{num_pts};
  Variables<tmpl::list<Var1>> upper{num_pts, 1.3};
  Variables<tmpl::list<Var1>> lower{num_pts, 7.0};
  evolution::dg::subcell::compute_boundary_terms(
      make_not_null(&corrections), correction, upper, lower, db::as_access(box),
      typename BoundaryCorrection::dg_boundary_terms_volume_tags{});
  Variables<tmpl::list<Var1>> expected_corrections{num_pts, 1.3 + 5.0 * 7.0};
  CHECK_VARIABLES_APPROX(corrections, expected_corrections);
}
