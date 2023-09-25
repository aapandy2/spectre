// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/Burgers/FiniteDifference/RegisterDerivedWithCharm.hpp"

#include "Evolution/Systems/Burgers/FiniteDifference/Factory.hpp"
#include "Evolution/Systems/Burgers/FiniteDifference/Reconstructor.hpp"
#include "Utilities/Serialization/RegisterDerivedClassesWithCharm.hpp"

namespace Burgers::fd {
void register_derived_with_charm() {
  register_derived_classes_with_charm<Reconstructor>();
}
}  // namespace Burgers::fd
