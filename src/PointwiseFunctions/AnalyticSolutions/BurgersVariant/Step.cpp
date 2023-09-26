// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "PointwiseFunctions/AnalyticSolutions/BurgersVariant/Step.hpp"

#include <pup.h>

#include "DataStructures/DataVector.hpp"  // IWYU pragma: keep
#include "DataStructures/Tensor/Tensor.hpp"
#include "Options/ParseError.hpp"
#include "Utilities/GenerateInstantiations.hpp"
#include "Utilities/MakeWithValue.hpp"
#include "Utilities/Math.hpp"  // IWYU pragma: keep

// IWYU pragma: no_forward_declare Tensor

namespace BurgersVariant::Solutions {

Step::Step(const double left_value, const double right_value,
           const double initial_shock_position, const Options::Context& context)
    : left_value_(left_value),
      right_value_(right_value),
      initial_shock_position_(initial_shock_position) {
  if (left_value <= right_value) {
    PARSE_ERROR(context, "Shock solution expects left_value > right_value");
  }
}

Step::Step(CkMigrateMessage* msg) : InitialData(msg) {}

std::unique_ptr<evolution::initial_data::InitialData> Step::get_clone() const {
  return std::make_unique<Step>(*this);
}

template <typename T>
Scalar<T> Step::v(const tnsr::I<T, 1>& x, const double t) const {
  const double current_shock_position =
      initial_shock_position_ + 0.5 * (left_value_ + right_value_) * t;
  return Scalar<T>(left_value_ -
                   (left_value_ - right_value_) *
                       step_function(get<0>(x) - current_shock_position));
}

template <typename T>
Scalar<T> Step::dv_dt(const tnsr::I<T, 1>& x, const double /*t*/) const {
  return make_with_value<Scalar<T>>(x, 0.0);
}

tuples::TaggedTuple<Tags::V> Step::variables(
    const tnsr::I<DataVector, 1>& x, const double t,
    tmpl::list<Tags::V> /*meta*/) const {
  return {v(x, t)};
}

tuples::TaggedTuple<::Tags::dt<Tags::V>> Step::variables(
    const tnsr::I<DataVector, 1>& x, const double t,
    tmpl::list<::Tags::dt<Tags::V>> /*meta*/) const {
  return {dv_dt(x, t)};
}

void Step::pup(PUP::er& p) {
  InitialData::pup(p);
  p | left_value_;
  p | right_value_;
  p | initial_shock_position_;
}

PUP::able::PUP_ID Step::my_PUP_ID = 0;
}  // namespace BurgersVariant::Solutions

#define DTYPE(data) BOOST_PP_TUPLE_ELEM(0, data)

#define INSTANTIATE(_, data)                                    \
  template Scalar<DTYPE(data)> BurgersVariant::Solutions::Step::v(     \
      const tnsr::I<DTYPE(data), 1>& x, double t) const;        \
  template Scalar<DTYPE(data)> BurgersVariant::Solutions::Step::dv_dt( \
      const tnsr::I<DTYPE(data), 1>& x, double t) const;

GENERATE_INSTANTIATIONS(INSTANTIATE, (double, DataVector))

#undef DTYPE
#undef INSTANTIATE
