cmake_minimum_required(VERSION 3.9)

project(Polygonal LANGUAGES CXX C VERSION 1.0.0)

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
list(APPEND Polygonal_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND Polygonal_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND Polygonal_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND Polygonal_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND Polygonal_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND Polygonal_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(src)

list(APPEND Polygonal_SOURCES ${polygonal_sources})
list(APPEND Polygonal_HEADERS ${polygonal_headers})
list(APPEND Polygonal_INCLUDE ${polygonal_includes})

add_subdirectory(ExportParaview)

list(APPEND Polygonal_SOURCES ${src_paraview_sources})
list(APPEND Polygonal_HEADERS ${src_paraview_headers})
list(APPEND Polygonal_INCLUDE ${src_paraview_includes})

# Create executable
################################################################################
add_executable(${PROJECT_NAME}
      main.cpp
      ${Polygonal_SOURCES}
      ${Polygonal_HEADERS})

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Polygonal/Cell0Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/Polygonal/Cell1Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/Polygonal/Cell2Ds.csv
#       DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${Polygonal_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Polygonal_INCLUDE})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)
