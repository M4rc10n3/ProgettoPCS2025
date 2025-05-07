list(APPEND polygonal_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND polygonal_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND polygonal_headers ${CMAKE_CURRENT_SOURCE_DIR}/Polyhedra.hpp)

list(APPEND polygonal_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(polygonal_sources ${polygonal_sources} PARENT_SCOPE)
set(polygonal_headers ${polygonal_headers} PARENT_SCOPE)
set(polygonal_includes ${polygonal_includes} PARENT_SCOPE)
