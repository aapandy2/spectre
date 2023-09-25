// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/BoundaryCorrections/RegisterDerived.hpp"

#include "Evolution/Systems/BurgersVariant/BoundaryCorrections/Factory.hpp"
#include "Utilities/Serialization/RegisterDerivedClassesWithCharm.hpp"

namespace BurgersVariant::BoundaryCorrections {
void register_derived_with_charm() {
  register_derived_classes_with_charm<BoundaryCorrection>();
}
}  // namespace BurgersVariant::BoundaryCorrections
