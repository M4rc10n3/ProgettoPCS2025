list(APPEND GTest_headers ${CMAKE_CURRENT_SOURCE_DIR}/TestUtils.hpp)

list(APPEND GTest_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(PlatonicSolids_sources ${GTest_sources} PARENT_SCOPE)
set(PlatonicSolids_headers ${GTest_headers} PARENT_SCOPE)
set(PlatonicSolids_includes ${GTest_includes} PARENT_SCOPE)