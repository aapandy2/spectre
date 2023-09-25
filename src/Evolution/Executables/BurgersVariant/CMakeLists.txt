# Distributed under the MIT License.
# See LICENSE.txt for details.

set(LIBS_TO_LINK
  BurgersVariant
  BurgersVariantAnalyticData
  BurgersVariantSolutions
  DgSubcell
  DiscontinuousGalerkin
  DomainCreators
  EventsAndDenseTriggers
  EventsAndTriggers
  Evolution
  FiniteDifference
  Informer
  Limiters
  LinearOperators
  Observer
  Options
  Parallel
  PhaseControl
  Serialization
  Time
  Utilities
  )

add_spectre_parallel_executable(
  "EvolveBurgersVariant"
  EvolveBurgersVariant
  Evolution/Executables/BurgersVariant
  "EvolutionMetavars"
  "${LIBS_TO_LINK}"
  )
