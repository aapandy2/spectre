// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>
#include <string>

#include "DataStructures/DataBox/Tag.hpp"
#include "DataStructures/Tensor/TypeAliases.hpp"
#include "Evolution/Systems/GrMhd/ValenciaDivClean/PrimitiveFromConservativeOptions.hpp"
#include "Evolution/Systems/GrMhd/ValenciaDivClean/TagsDeclarations.hpp"
#include "Evolution/Tags.hpp"
#include "Options/String.hpp"

/// \cond
class DataVector;
/// \endcond

namespace grmhd {
namespace ValenciaDivClean {
/// %Tags for the Valencia formulation of the ideal GRMHD equations
/// with divergence cleaning.
namespace Tags {

/// The characteristic speeds
struct CharacteristicSpeeds : db::SimpleTag {
  using type = std::array<DataVector, 9>;
};

/// The densitized rest-mass density \f${\tilde D}\f$
struct TildeD : db::SimpleTag {
  using type = Scalar<DataVector>;
};

/// The densitized electron number density times the baryon mass
/// \f$\tilde{Y}_e = {\tilde D} Y_e\f$
struct TildeYe : db::SimpleTag {
  using type = Scalar<DataVector>;
};

/// The densitized conserved variable corresponding to bulk viscosity
/// \f$\tilde{V}_B = \hat{Pi}_B W\f$
struct TildeVB : db::SimpleTag {
  using type = Scalar<DataVector>;
};

/// The densitized energy density \f${\tilde \tau}\f$
struct TildeTau : db::SimpleTag {
  using type = Scalar<DataVector>;
};

/// The densitized momentum density \f${\tilde S_i}\f$
template <typename Fr>
struct TildeS : db::SimpleTag {
  using type = tnsr::i<DataVector, 3, Fr>;
  static std::string name() { return Frame::prefix<Fr>() + "TildeS"; }
};

/// The densitized magnetic field \f${\tilde B^i}\f$
template <typename Fr>
struct TildeB : db::SimpleTag {
  using type = tnsr::I<DataVector, 3, Fr>;
  static std::string name() { return Frame::prefix<Fr>() + "TildeB"; }
};

/// The densitized divergence-cleaning field \f${\tilde \Phi}\f$
struct TildePhi : db::SimpleTag {
  using type = Scalar<DataVector>;
};

/// \brief Set to `true` if the variables needed fixing.
///
/// Used in DG-subcell hybrid scheme evolutions.
struct VariablesNeededFixing : db::SimpleTag {
  using type = bool;
};
}  // namespace Tags

namespace OptionTags {
/// \ingroup OptionGroupsGroup
/// Groups option tags related to the ValenciaDivClean evolution system.
struct ValenciaDivCleanGroup {
  static std::string name() { return "ValenciaDivClean"; }
  static constexpr Options::String help{"Options for the evolution system"};
  using group = evolution::OptionTags::SystemGroup;
};

/// \brief The constraint damping parameter
struct DampingParameter {
  static std::string name() { return "DampingParameter"; }
  using type = double;
  static constexpr Options::String help{
      "Constraint damping parameter for divergence cleaning"};
  using group = ValenciaDivCleanGroup;
};


/// \brief The bulk viscosity
struct BulkViscosity {
  static std::string name() { return "BulkViscosity"; }
  using type = double;
  static constexpr Options::String help{
      "Bulk viscosity zeta for the bulk viscous evolution"};
  using group = ValenciaDivCleanGroup;
};


/// \brief The bulk viscous relaxation time
struct BulkRelaxationTime {
  static std::string name() { return "BulkRelaxationTime"; }
  using type = double;
  static constexpr Options::String help{
      "Relaxation time for the bulk viscous evolution"};
  using group = ValenciaDivCleanGroup;
};


/// \brief Nonlinear coupling term in the bulk viscous EOM
struct BulkNonlinearCoupling {
  static std::string name() { return "BulkNonlinearCoupling"; }
  using type = double;
  static constexpr Options::String help{
      "Coupling constant on the Pi^2 term in the bulk viscous EOM"};
  using group = ValenciaDivCleanGroup;
};

struct PrimitiveFromConservativeOptions {
  static std::string name() { return "PrimitiveFromConservative"; }
  using type = grmhd::ValenciaDivClean::PrimitiveFromConservativeOptions;
  static constexpr Options::String help{
      "Value of density times Lorentz factor below which we skip conservative "
      "to primitive inversion."};
};

}  // namespace OptionTags

namespace Tags {
/// The constraint damping parameter for divergence cleaning
struct ConstraintDampingParameter : db::SimpleTag {
  using type = double;
  using option_tags = tmpl::list<OptionTags::DampingParameter>;

  static constexpr bool pass_metavariables = false;
  static double create_from_options(const double constraint_damping_parameter) {
    return constraint_damping_parameter;
  }
};

/// The bulk viscosity
struct BulkViscosity : db::SimpleTag {
  using type = double;
  using option_tags = tmpl::list<OptionTags::BulkViscosity>;

  static constexpr bool pass_metavariables = false;
  static double create_from_options(const double bulk_viscosity) {
    return bulk_viscosity;
  }
};

/// The bulk relaxation time
struct BulkRelaxationTime : db::SimpleTag {
  using type = double;
  using option_tags = tmpl::list<OptionTags::BulkRelaxationTime>;

  static constexpr bool pass_metavariables = false;
  static double create_from_options(const double bulk_relaxation_time) {
    return bulk_relaxation_time;
  }
};

/// The bulk nonlinear coupling constant
struct BulkNonlinearCoupling : db::SimpleTag {
  using type = double;
  using option_tags = tmpl::list<OptionTags::BulkNonlinearCoupling>;

  static constexpr bool pass_metavariables = false;
  static double create_from_options(const double bulk_nonlinear_coupling) {
    return bulk_nonlinear_coupling;
  }
};

struct PrimitiveFromConservativeOptions : db::SimpleTag {
  using type = grmhd::ValenciaDivClean::PrimitiveFromConservativeOptions;
  using option_tags = tmpl::list<OptionTags::PrimitiveFromConservativeOptions>;

  static constexpr bool pass_metavariables = false;
  static type create_from_options(const type& options) { return options; }
};

}  // namespace Tags
}  // namespace ValenciaDivClean
}  // namespace grmhd
