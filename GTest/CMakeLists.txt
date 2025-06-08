list(APPEND GTest_headers ${CMAKE_CURRENT_SOURCE_DIR}/TestUtils.hpp)

list(APPEND GTest_includes ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND PlatonicSolids_headers ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils.hpp)
list(APPEND PlatonicSolids_sources ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils.cpp)

set(PlatonicSolids_sources ${GTest_sources} PARENT_SCOPE)
set(PlatonicSolids_headers ${GTest_headers} PARENT_SCOPE)
set(PlatonicSolids_includes ${GTest_includes} PARENT_SCOPE)