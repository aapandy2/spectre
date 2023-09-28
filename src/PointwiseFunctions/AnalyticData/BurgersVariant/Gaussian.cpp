// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "PointwiseFunctions/AnalyticData/BurgersVariant/Gaussian.hpp"

#include <array>
#include <cmath>

#include "DataStructures/DataVector.hpp"  // IWYU pragma: keep
#include "DataStructures/Tensor/Tensor.hpp"
#include "Utilities/ErrorHandling/Assert.hpp"
#include "Utilities/GenerateInstantiations.hpp"
#include "Utilities/Math.hpp"  // IWYU pragma: keep

// IWYU pragma: no_forward_declare Tensor

namespace BurgersVariant::AnalyticData {
template <typename T>
Scalar<T> Gaussian::v(const tnsr::I<T, 1>& x) const {
//  return Scalar<T>{sin(get<0>(x))};
  return Scalar<T>{square(1. + exp(-square(get<0>(x))))};
}

std::unique_ptr<evolution::initial_data::InitialData> Gaussian::get_clone()
    const {
  return std::make_unique<Gaussian>(*this);
}

Gaussian::Gaussian(CkMigrateMessage* msg) : InitialData(msg) {}

tuples::TaggedTuple<Tags::V> Gaussian::variables(
    const tnsr::I<DataVector, 1>& x, tmpl::list<Tags::V> /*meta*/) const {
  return {v(x)};
}

void Gaussian::pup(PUP::er& p) { InitialData::pup(p); }

PUP::able::PUP_ID Gaussian::my_PUP_ID = 0;

bool operator==(const Gaussian& /*lhs*/, const Gaussian& /*rhs*/) {
  return true;
}

bool operator!=(const Gaussian& lhs, const Gaussian& rhs) {
  return not(lhs == rhs);
}
}  // namespace BurgersVariant::AnalyticData

#define DTYPE(data) BOOST_PP_TUPLE_ELEM(0, data)

#define INSTANTIATE(_, data)                                       \
  template Scalar<DTYPE(data)> BurgersVariant::AnalyticData::Gaussian::v( \
      const tnsr::I<DTYPE(data), 1>& x) const;

GENERATE_INSTANTIATIONS(INSTANTIATE, (double, DataVector))

#undef DTYPE
#undef INSTANTIATE
