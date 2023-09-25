// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/BurgersVariant/FiniteDifference/RegisterDerivedWithCharm.hpp"

#include "Evolution/Systems/BurgersVariant/FiniteDifference/Factory.hpp"
#include "Evolution/Systems/BurgersVariant/FiniteDifference/Reconstructor.hpp"
#include "Utilities/Serialization/RegisterDerivedClassesWithCharm.hpp"

namespace BurgersVariant::fd {
void register_derived_with_charm() {
  register_derived_classes_with_charm<Reconstructor>();
}
}  // namespace BurgersVariant::fd
