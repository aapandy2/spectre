// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Framework/TestingFramework.hpp"

#include <string>

#include "DataStructures/DataBox/DataBox.hpp"
#include "DataStructures/DataBox/TagName.hpp"
#include "DataStructures/Tensor/EagerMath/Determinant.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Domain/CoordinateMaps/Affine.hpp"
#include "Domain/CoordinateMaps/CoordinateMap.hpp"
#include "Domain/CoordinateMaps/CoordinateMap.tpp"
#include "Domain/CoordinateMaps/Rotation.hpp"
#include "Domain/CoordinateMaps/Tags.hpp"
#include "Domain/Creators/DomainCreator.hpp"
#include "Domain/Creators/OptionTags.hpp"
#include "Domain/ElementMap.hpp"
#include "Domain/Structure/ElementId.hpp"
#include "Domain/Structure/ObjectLabel.hpp"
#include "Domain/Structure/SegmentId.hpp"
#include "Domain/Tags.hpp"
#include "Framework/TestCreation.hpp"
#include "Helpers/DataStructures/DataBox/TestHelpers.hpp"
#include "Helpers/Domain/BoundaryConditions/BoundaryCondition.hpp"
#include "Utilities/MakeArray.hpp"

namespace domain {
namespace {
template <size_t Dim>
void test_simple_tags() {
  TestHelpers::db::test_simple_tag<Tags::Element<Dim>>("Element");
  TestHelpers::db::test_simple_tag<Tags::Mesh<Dim>>("Mesh");
  TestHelpers::db::test_simple_tag<Tags::ElementMap<Dim>>(
      "ElementMap(Inertial)");
  TestHelpers::db::test_simple_tag<Tags::ElementMap<Dim, Frame::Grid>>(
      "ElementMap(Grid)");
  TestHelpers::db::test_simple_tag<Tags::Coordinates<Dim, Frame::Grid>>(
      "GridCoordinates");
  TestHelpers::db::test_simple_tag<
      Tags::Coordinates<Dim, Frame::ElementLogical>>(
      "ElementLogicalCoordinates");
  TestHelpers::db::test_simple_tag<Tags::Coordinates<Dim, Frame::Inertial>>(
      "InertialCoordinates");
  TestHelpers::db::test_simple_tag<
      Tags::InverseJacobian<Dim, Frame::ElementLogical, Frame::Inertial>>(
      "InverseJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_simple_tag<
      Tags::DetInvJacobian<Frame::ElementLogical, Frame::Inertial>>(
      "DetInvJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_simple_tag<
      Tags::DetJacobian<Frame::ElementLogical, Frame::Inertial>>(
      "DetJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_simple_tag<
      Tags::DetTimesInvJacobian<Dim, Frame::ElementLogical, Frame::Inertial>>(
      "DetTimesInvJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_simple_tag<Tags::InternalDirections<Dim>>(
      "InternalDirections");
  TestHelpers::db::test_simple_tag<Tags::BoundaryDirectionsInterior<Dim>>(
      "BoundaryDirectionsInterior");
  TestHelpers::db::test_simple_tag<Tags::BoundaryDirectionsExterior<Dim>>(
      "BoundaryDirectionsExterior");
  TestHelpers::db::test_simple_tag<Tags::Direction<Dim>>("Direction");
  TestHelpers::db::test_simple_tag<
      Tags::Jacobian<Dim, Frame::ElementLogical, Frame::Inertial>>(
      "Jacobian(ElementLogical,Inertial)");
}

template <size_t Dim>
ElementMap<Dim, Frame::Grid> element_map();

template <>
ElementMap<1, Frame::Grid> element_map() {
  constexpr size_t dim = 1;
  const auto segment_ids = std::array<SegmentId, dim>({{SegmentId(2, 3)}});
  const CoordinateMaps::Affine first_map{-3.0, 8.7, 0.4, 5.5};
  const CoordinateMaps::Affine second_map{1.0, 8.0, -2.5, -1.0};
  const ElementId<dim> element_id(0, segment_ids);
  return ElementMap<dim, Frame::Grid>{
      element_id, make_coordinate_map_base<Frame::BlockLogical, Frame::Grid>(
                      first_map, second_map)};
}

template <>
ElementMap<2, Frame::Grid> element_map() {
  constexpr size_t dim = 2;
  const auto segment_ids =
      std::array<SegmentId, dim>({{SegmentId(2, 3), SegmentId(1, 0)}});
  const CoordinateMaps::Rotation<dim> first_map(1.6);
  const CoordinateMaps::Rotation<dim> second_map(3.1);
  const ElementId<dim> element_id(0, segment_ids);
  return ElementMap<dim, Frame::Grid>{
      element_id, make_coordinate_map_base<Frame::BlockLogical, Frame::Grid>(
                      first_map, second_map)};
}

template <>
ElementMap<3, Frame::Grid> element_map() {
  constexpr size_t dim = 3;
  const auto segment_ids = std::array<SegmentId, dim>(
      {{SegmentId(2, 3), SegmentId(1, 0), SegmentId(2, 1)}});
  const CoordinateMaps::Rotation<dim> first_map{M_PI_4, M_PI_4, M_PI_2};
  const CoordinateMaps::Rotation<dim> second_map{M_PI_4, M_PI_2, M_PI_2};
  const ElementId<dim> element_id(0, segment_ids);
  return ElementMap<dim, Frame::Grid>{
      element_id, make_coordinate_map_base<Frame::BlockLogical, Frame::Grid>(
                      first_map, second_map)};
}

template <size_t Dim>
void test_compute_tags() {
  TestHelpers::db::test_compute_tag<Tags::InverseJacobianCompute<
      Tags::ElementMap<Dim>, Tags::Coordinates<Dim, Frame::ElementLogical>>>(
      "InverseJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_compute_tag<
      Tags::DetInvJacobianCompute<Dim, Frame::ElementLogical, Frame::Inertial>>(
      "DetInvJacobian(ElementLogical,Inertial)");
  TestHelpers::db::test_compute_tag<Tags::InternalDirectionsCompute<Dim>>(
      "InternalDirections");
  TestHelpers::db::test_compute_tag<
      Tags::BoundaryDirectionsInteriorCompute<Dim>>(
      "BoundaryDirectionsInterior");
  TestHelpers::db::test_compute_tag<
      Tags::BoundaryDirectionsExteriorCompute<Dim>>(
      "BoundaryDirectionsExterior");
  TestHelpers::db::test_compute_tag<Tags::MappedCoordinates<
      Tags::ElementMap<Dim>, Tags::Coordinates<Dim, Frame::ElementLogical>>>(
      "InertialCoordinates");
  TestHelpers::db::test_compute_tag<
      Tags::JacobianCompute<Dim, Frame::ElementLogical, Frame::Inertial>>(
      "Jacobian(ElementLogical,Inertial)");

  auto map = element_map<Dim>();
  const tnsr::I<DataVector, Dim, Frame::ElementLogical> logical_coords(
      make_array<Dim>(DataVector{-1.0, -0.5, 0.0, 0.5, 1.0}));
  const auto expected_inv_jacobian = map.inv_jacobian(logical_coords);
  const auto expected_jacobian = map.jacobian(logical_coords);

  const auto box = db::create<
      tmpl::list<Tags::ElementMap<Dim, Frame::Grid>,
                 Tags::Coordinates<Dim, Frame::ElementLogical>>,
      db::AddComputeTags<
          Tags::InverseJacobianCompute<
              Tags::ElementMap<Dim, Frame::Grid>,
              Tags::Coordinates<Dim, Frame::ElementLogical>>,
          Tags::DetInvJacobianCompute<Dim, Frame::ElementLogical, Frame::Grid>,
          Tags::JacobianCompute<Dim, Frame::ElementLogical, Frame::Grid>>>(
      std::move(map), logical_coords);
  CHECK_ITERABLE_APPROX(
      (db::get<Tags::InverseJacobian<Dim, Frame::ElementLogical, Frame::Grid>>(
          box)),
      expected_inv_jacobian);
  CHECK_ITERABLE_APPROX(
      (db::get<Tags::DetInvJacobian<Frame::ElementLogical, Frame::Grid>>(box)),
      determinant(expected_inv_jacobian));
  CHECK_ITERABLE_APPROX(
      (db::get<Tags::Jacobian<Dim, Frame::ElementLogical, Frame::Grid>>(box)),
      expected_jacobian);
}

SPECTRE_TEST_CASE("Unit.Domain.Tags", "[Unit][Domain]") {
  test_simple_tags<1>();
  test_simple_tags<2>();
  test_simple_tags<3>();

  test_compute_tags<1>();
  test_compute_tags<2>();
  test_compute_tags<3>();
}
}  // namespace
}  // namespace domain
