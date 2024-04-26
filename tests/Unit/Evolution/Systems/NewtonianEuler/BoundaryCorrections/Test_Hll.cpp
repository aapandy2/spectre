// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <array>
#include <cstddef>
#include <string>

#include "Evolution/Systems/NewtonianEuler/BoundaryCorrections/Factory.hpp"
#include "Evolution/Systems/NewtonianEuler/BoundaryCorrections/Hll.hpp"
#include "Evolution/Systems/NewtonianEuler/System.hpp"
#include "Framework/SetupLocalPythonEnvironment.hpp"
#include "Framework/TestCreation.hpp"
#include "Helpers/Evolution/DiscontinuousGalerkin/BoundaryCorrections.hpp"
#include "Helpers/Evolution/DiscontinuousGalerkin/Range.hpp"
#include "NumericalAlgorithms/Spectral/Basis.hpp"
#include "NumericalAlgorithms/Spectral/Mesh.hpp"
#include "NumericalAlgorithms/Spectral/Quadrature.hpp"
#include "PointwiseFunctions/Hydro/EquationsOfState/IdealFluid.hpp"
#include "PointwiseFunctions/Hydro/EquationsOfState/PolytropicFluid.hpp"
#include "PointwiseFunctions/Hydro/Tags.hpp"
#include "Utilities/TMPL.hpp"
#include "Utilities/TaggedTuple.hpp"

namespace {
struct ConvertIdeal {
  using unpacked_container = bool;
  using packed_container = EquationsOfState::EquationOfState<false, 2>;
  using packed_type = bool;

  static inline unpacked_container unpack(const packed_container& /*packed*/,
                                          const size_t /*grid_point_index*/) {
    return false;
  }

  [[noreturn]] static inline void pack(
      const gsl::not_null<packed_container*> /*packed*/,
      const unpacked_container& /*unpacked*/,
      const size_t /*grid_point_index*/) {
    ERROR("Should not be converting an EOS from an unpacked to a packed type");
  }

  static inline size_t get_size(const packed_container& /*packed*/) {
    return 1;
  }
};

struct DummyInitialData {
  using argument_tags = tmpl::list<>;
  struct source_term_type {
    using sourced_variables = tmpl::list<>;
    using argument_tags = tmpl::list<>;
  };
};

namespace helpers = TestHelpers::evolution::dg;

template <size_t Dim, typename EosType>
void test(const gsl::not_null<std::mt19937*> gen, const size_t num_pts,
          const EosType& equation_of_state) {
  const tuples::TaggedTuple<
      hydro::Tags::EquationOfState<false, EosType::thermodynamic_dim>>
      volume_data{equation_of_state.get_clone()};
  const tuples::TaggedTuple<
      helpers::Tags::Range<NewtonianEuler::Tags::MassDensityCons>,
      helpers::Tags::Range<hydro::Tags::SpecificInternalEnergy<DataVector>>>
      ranges{std::array{1.0e-30, 1.0}, std::array{1.0e-30, 1.0}};

  helpers::test_boundary_correction_conservation<NewtonianEuler::System<Dim>>(
      gen, NewtonianEuler::BoundaryCorrections::Hll<Dim>{},
      Mesh<Dim - 1>{num_pts, Spectral::Basis::Legendre,
                    Spectral::Quadrature::Gauss},
      volume_data, ranges);

  helpers::test_boundary_correction_with_python<NewtonianEuler::System<Dim>,
                                                tmpl::list<ConvertIdeal>>(
      gen, "Hll", "dg_package_data", "dg_boundary_terms",
      NewtonianEuler::BoundaryCorrections::Hll<Dim>{},
      Mesh<Dim - 1>{num_pts, Spectral::Basis::Legendre,
                    Spectral::Quadrature::Gauss},
      volume_data, ranges);

  const auto hll = TestHelpers::test_creation<std::unique_ptr<
      NewtonianEuler::BoundaryCorrections::BoundaryCorrection<Dim>>>("Hll:");

  helpers::test_boundary_correction_with_python<NewtonianEuler::System<Dim>,
                                                tmpl::list<ConvertIdeal>>(
      gen, "Hll", "dg_package_data", "dg_boundary_terms",
      dynamic_cast<const NewtonianEuler::BoundaryCorrections::Hll<Dim>&>(*hll),
      Mesh<Dim - 1>{num_pts, Spectral::Basis::Legendre,
                    Spectral::Quadrature::Gauss},
      volume_data, ranges);
}
}  // namespace

SPECTRE_TEST_CASE("Unit.NewtonianEuler.BoundaryCorrections.Hll",
                  "[Unit][Evolution]") {
  PUPable_reg(NewtonianEuler::BoundaryCorrections::Hll<1>);
  PUPable_reg(NewtonianEuler::BoundaryCorrections::Hll<2>);
  PUPable_reg(NewtonianEuler::BoundaryCorrections::Hll<3>);

  pypp::SetupLocalPythonEnvironment local_python_env{
      "Evolution/Systems/NewtonianEuler/BoundaryCorrections"};
  MAKE_GENERATOR(gen);

  test<1>(make_not_null(&gen), 1, EquationsOfState::IdealFluid<false>{1.3});
  test<2>(make_not_null(&gen), 5, EquationsOfState::IdealFluid<false>{1.3});
  test<3>(make_not_null(&gen), 5, EquationsOfState::IdealFluid<false>{1.3});
}
