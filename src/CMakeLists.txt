list(APPEND PlatonicSolids_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND PlatonicSolids_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND PlatonicSolids_headers ${CMAKE_CURRENT_SOURCE_DIR}/BuildPolyhedra.hpp)
list(APPEND PlatonicSolids_sources ${CMAKE_CURRENT_SOURCE_DIR}/BuildPolyhedra.cpp)

list(APPEND PlatonicSolids_headers ${CMAKE_CURRENT_SOURCE_DIR}/Polyhedra.hpp)

list(APPEND PlatonicSolids_headers ${CMAKE_CURRENT_SOURCE_DIR}/Sources.hpp)

list(APPEND PlatonicSolids_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(PlatonicSolids_sources ${PlatonicSolids_sources} PARENT_SCOPE)
set(PlatonicSolids_headers ${PlatonicSolids_headers} PARENT_SCOPE)
set(PlatonicSolids_includes ${PlatonicSolids_includes} PARENT_SCOPE)
