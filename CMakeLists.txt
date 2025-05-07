cmake_minimum_required(VERSION 3.9)

project(PlatonicSolids LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND PlatonicSolids_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND PlatonicSolids_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND PlatonicSolids_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND PlatonicSolids_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND PlatonicSolids_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND PlatonicSolids_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(src)

list(APPEND PlatonicSolids_SOURCES ${PlatonicSolids_sources})
list(APPEND PlatonicSolids_HEADERS ${PlatonicSolids_headers})
list(APPEND PlatonicSolids_INCLUDE ${PlatonicSolids_includes})

add_subdirectory(ExportParaview)

list(APPEND PlatonicSolids_SOURCES ${src_paraview_sources})
list(APPEND PlatonicSolids_HEADERS ${src_paraview_headers})
list(APPEND PlatonicSolids_INCLUDE ${src_paraview_includes})

# Create executable
################################################################################
add_executable(${PROJECT_NAME}
      main.cpp
      ${PlatonicSolids_SOURCES}
      ${PlatonicSolids_HEADERS})

target_link_libraries(${PROJECT_NAME} ${PlatonicSolids_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PlatonicSolids_INCLUDE})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)


add_executable(ManuExe
      main_Manu.cpp
      ${PlatonicSolids_SOURCES}
      ${PlatonicSolids_HEADERS}
      ${ManuExe_SOURCES}
      ${ManuExe_HEADERS})

target_link_libraries(ManuExe ${PlatonicSolids_LINKED_LIBRARIES})
target_include_directories(ManuExe PRIVATE ${PlatonicSolids_INCLUDE} ${ManuExe_INCLUDE})
target_compile_options(ManuExe PUBLIC -fPIC)

add_executable(MarcoExe
      main_Marco.cpp
      ${PlatonicSolids_SOURCES}
      ${PlatonicSolids_HEADERS}
      ${MarcoExe_SOURCES}
      ${MarcoExe_HEADERS})

target_link_libraries(MarcoExe ${PlatonicSolids_LINKED_LIBRARIES})
target_include_directories(MarcoExe PRIVATE ${PlatonicSolids_INCLUDE} ${MarcoExe_INCLUDE})
target_compile_options(MarcoExe PUBLIC -fPIC)

add_executable(AnnaExe
      main_Anna.cpp
      ${PlatonicSolids_SOURCES}
      ${PlatonicSolids_HEADERS}
      ${AnnaExe_SOURCES}
      ${AnnaExe_HEADERS})

target_link_libraries(AnnaExe ${PlatonicSolids_LINKED_LIBRARIES})
target_include_directories(AnnaExe PRIVATE ${PlatonicSolids_INCLUDE} ${AnnaExe_INCLUDE})
target_compile_options(AnnaExe PUBLIC -fPIC)

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/PlatonicSolids/Cell0Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/PlatonicSolids/Cell1Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/PlatonicSolids/Cell2Ds.csv
#       DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
