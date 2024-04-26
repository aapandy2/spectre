// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>

#include "DataStructures/DataBox/Tag.hpp"
#include "DataStructures/Tensor/Tensor.hpp"
#include "Evolution/Systems/NewtonianEuler/Tags.hpp"
#include "PointwiseFunctions/Hydro/EquationsOfState/EquationOfState.hpp"
#include "PointwiseFunctions/Hydro/Tags.hpp"
#include "Utilities/Gsl.hpp"
#include "Utilities/TMPL.hpp"

namespace NewtonianEuler {
/// @{
/*!
 * Compute the Newtonian sound speed squared
 * \f$c_s^2 = \chi + p\kappa / \rho^2\f$, where
 * \f$p\f$ is the fluid pressure, \f$\rho\f$ is the mass density,
 * \f$\chi = (\partial p/\partial\rho)_\epsilon\f$ and
 * \f$\kappa = (\partial p/ \partial \epsilon)_\rho\f$, where
 * \f$\epsilon\f$ is the specific internal energy.
 */
template <typename DataType, size_t ThermodynamicDim>
void sound_speed_squared(
    gsl::not_null<Scalar<DataType>*> result,
    const Scalar<DataType>& mass_density,
    const Scalar<DataType>& specific_internal_energy,
    const EquationsOfState::EquationOfState<false, ThermodynamicDim>&
        equation_of_state);

template <typename DataType, size_t ThermodynamicDim>
Scalar<DataType> sound_speed_squared(
    const Scalar<DataType>& mass_density,
    const Scalar<DataType>& specific_internal_energy,
    const EquationsOfState::EquationOfState<false, ThermodynamicDim>&
        equation_of_state);
/// @}

namespace Tags {
/// Compute item for the sound speed \f$c_s\f$.
///
/// Can be retrieved using `NewtonianEuler::Tags::SoundSpeed`
template <typename DataType>
struct SoundSpeedCompute : SoundSpeed<DataType>, db::ComputeTag {
  using argument_tags = tmpl::list<hydro::Tags::SoundSpeedSquared<DataType>>;

  using return_type = Scalar<DataType>;

  static void function(const gsl::not_null<Scalar<DataType>*> result,
                       const Scalar<DataType>& sound_speed_squared) {
    get(*result) = sqrt(get(sound_speed_squared));
  }

  using base = SoundSpeed<DataType>;
};

/// Compute item for the sound speed squared \f$c_s^2\f$.
/// \see NewtonianEuler::sound_speed_squared
///
/// Can be retrieved using `NewtonianEuler::Tags::SoundSpeedSquared`
template <typename DataType>
struct SoundSpeedSquaredCompute : hydro::Tags::SoundSpeedSquared<DataType>,
                                  db::ComputeTag {
  using argument_tags =
      tmpl::list<hydro::Tags::RestMassDensity<DataType>,
                 hydro::Tags::SpecificInternalEnergy<DataType>,
                 hydro::Tags::EquationOfState<false, 2>>;

  using return_type = Scalar<DataType>;

  template <typename EquationOfStateType>
  static void function(const gsl::not_null<Scalar<DataType>*> result,
                       const Scalar<DataType>& mass_density,
                       const Scalar<DataType>& specific_internal_energy,
                       const EquationOfStateType& equation_of_state) {
    sound_speed_squared(result, mass_density, specific_internal_energy,
                        equation_of_state);
  }

  using base = hydro::Tags::SoundSpeedSquared<DataType>;
};
}  // namespace Tags
}  // namespace NewtonianEuler
