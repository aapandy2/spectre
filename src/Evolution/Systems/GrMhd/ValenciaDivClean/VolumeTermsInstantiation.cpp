// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "DataStructures/DataBox/PrefixHelpers.hpp"
#include "DataStructures/DataBox/Prefixes.hpp"
#include "Evolution/DiscontinuousGalerkin/Actions/VolumeTermsImpl.tpp"
#include "Evolution/Systems/GrMhd/ValenciaDivClean/System.hpp"
#include "Evolution/Systems/GrMhd/ValenciaDivClean/TimeDerivativeTerms.hpp"

namespace evolution::dg::Actions::detail {
template void volume_terms<::grmhd::ValenciaDivClean::TimeDerivativeTerms>(
    const gsl::not_null<Variables<db::wrap_tags_in<
        ::Tags::dt,
        typename ::grmhd::ValenciaDivClean::System::variables_tag::tags_list>>*>
        dt_vars_ptr,
    const gsl::not_null<Variables<db::wrap_tags_in<
        ::Tags::Flux,
        typename ::grmhd::ValenciaDivClean::System::flux_variables,
        tmpl::size_t<3>, Frame::Inertial>>*>
        volume_fluxes,
    const gsl::not_null<Variables<db::wrap_tags_in<
        ::Tags::deriv,
        typename ::grmhd::ValenciaDivClean::System::gradient_variables,
        tmpl::size_t<3>, Frame::Inertial>>*>
        partial_derivs,
    const gsl::not_null<
        Variables<typename ::grmhd::ValenciaDivClean::System::
                      compute_volume_time_derivative_terms::temporary_tags>*>
        temporaries,
    [[maybe_unused]] const gsl::not_null<Variables<db::wrap_tags_in<
        ::Tags::div, db::wrap_tags_in<::Tags::Flux,
                                      typename ::grmhd::ValenciaDivClean::
                                          System::flux_variables,
                                      tmpl::size_t<3>, Frame::Inertial>>>*>
        div_fluxes,
    const Variables<
        typename ::grmhd::ValenciaDivClean::System::variables_tag::tags_list>&
        evolved_vars,
    const ::dg::Formulation dg_formulation, const Mesh<3>& mesh,
    [[maybe_unused]] const tnsr::I<DataVector, 3, Frame::Inertial>&
        inertial_coordinates,
    const InverseJacobian<DataVector, 3, Frame::ElementLogical,
                          Frame::Inertial>&
        logical_to_inertial_inverse_jacobian,
    [[maybe_unused]] const Scalar<DataVector>* const det_inverse_jacobian,
    const std::optional<tnsr::I<DataVector, 3, Frame::Inertial>>& mesh_velocity,
    const std::optional<Scalar<DataVector>>& div_mesh_velocity,

    const Scalar<DataVector>& tilde_d, const Scalar<DataVector>& tilde_ye,
    const Scalar<DataVector>& tilde_vb, const Scalar<DataVector>& tilde_tau,
    const tnsr::i<DataVector, 3, Frame::Inertial>& tilde_s,
    const tnsr::I<DataVector, 3, Frame::Inertial>& tilde_b,
    const Scalar<DataVector>& tilde_phi, const Scalar<DataVector>& lapse,
    const tnsr::I<DataVector, 3, Frame::Inertial>& shift,
    const Scalar<DataVector>& sqrt_det_spatial_metric,
    const tnsr::ii<DataVector, 3, Frame::Inertial>& spatial_metric,
    const tnsr::II<DataVector, 3, Frame::Inertial>& inv_spatial_metric,
    const tnsr::i<DataVector, 3, Frame::Inertial>& d_lapse,
    const tnsr::iJ<DataVector, 3, Frame::Inertial>& d_shift,
    const tnsr::ijj<DataVector, 3, Frame::Inertial>& d_spatial_metric,
    const Scalar<DataVector>& pressure,
    const tnsr::I<DataVector, 3, Frame::Inertial>& spatial_velocity,
    const Scalar<DataVector>& lorentz_factor,
    const tnsr::I<DataVector, 3, Frame::Inertial>& magnetic_field,

    const Scalar<DataVector>& rest_mass_density,
    const Scalar<DataVector>& electron_fraction,
    const Scalar<DataVector>& transformed_bulk_scalar,
    const Scalar<DataVector>& specific_internal_energy,
    const tnsr::ii<DataVector, 3, Frame::Inertial>& extrinsic_curvature,
    const double& constraint_damping_parameter,
    const double& bulk_viscosity,
    const double& bulk_relaxation_time,
    const double& bulk_nonlinear_coupling);
}  // namespace evolution::dg::Actions::detail
