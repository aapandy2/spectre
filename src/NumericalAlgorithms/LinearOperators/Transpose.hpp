// Distributed under the MIT License.
// See LICENSE.txt for details.

/// \file
/// Defines function transpose

#pragma once

#include <cstddef>

#include "ErrorHandling/Assert.hpp"
#include "Utilities/Gsl.hpp"
#include "Utilities/MakeWithValue.hpp"

// @{
/// \ingroup NumericalAlgorithmsGroup
/// \brief Function to compute transposed data.
///
/// The primary use of this function is to rearrange the memory layout so that
/// another function can operate on contiguous chunks of memory.
///
/// \requires `transpose.size()` to be the product of `number_of_chunks` and
/// `chunk_size`, `u.size()` to be equal or greater than `transpose.size()`,
/// and that both `transpose` and `u` have a `data()` member function.
///
/// \details The container `u` holds a contiguous array of data,
/// treated as a sequence of `number_of_chunks` contiguous sets of
/// entries of size `chunk_size`.  The output `transpose` has its data
/// arranged such that the first `number_of_chunks` elements in
/// `transpose` will be the first element of each chunk of `u`.  The
/// last `number_of_chunks` elements in `transpose` will be the last
/// (i.e.  `chunk_size`-th) element of each chunk of `u`.  If
/// `u.size()` is greater than `transpose.size()` the extra elements
/// of `u` are ignored.
///
/// \note This is equivalent to treating the first part of `u` as a matrix and
/// filling `transpose` (or the returned object) with the transpose of that
/// matrix.
///
/// \example
/// \snippet Test_Transpose.cpp transpose_matrix
/// \snippet Test_Transpose.cpp transpose_by_not_null_example
/// \snippet Test_Transpose.cpp return_transpose_example
/// \snippet Test_Transpose.cpp partial_transpose_example
///
/// \tparam U the type of data to be transposed
/// \tparam T the type of the transposed data
template <typename U, typename T>
void transpose(const gsl::not_null<T*> transpose,
               const U& u, const size_t chunk_size,
               const size_t number_of_chunks) {
  ASSERT(chunk_size * number_of_chunks == transpose->size(),
         "chunk_size = " << chunk_size
                         << ", number_of_chunks = " << number_of_chunks
                         << ", size = " << transpose->size());
  ASSERT(transpose->size() <= u.size(),
         "transpose.size = " << transpose->size() << ", u.size = " << u.size());
  for (size_t j = 0; j < number_of_chunks; ++j) {
    for (size_t i = 0; i < chunk_size; ++i) {
      // clang-tidy: pointer arithmetic
      transpose->data()[j + number_of_chunks * i] =  // NOLINT
          u.data()[i + chunk_size * j];              // NOLINT
    }
  }
}

template <typename U, typename T = U>
T transpose(const U& u, const size_t chunk_size,
            const size_t number_of_chunks) {
  T t = make_with_value<T>(u,0.0);
  transpose(make_not_null(&t), u, chunk_size, number_of_chunks);
  return t;
}
// @}
