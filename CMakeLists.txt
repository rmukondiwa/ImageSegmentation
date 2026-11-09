cmake_minimum_required(VERSION 3.15)
project(ImageSegmentation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Core library: algorithm + image + modeling + visualization, image-agnostic
add_library(mflib
    src/maxflow.cpp
    src/image.cpp
    src/graph_builder.cpp
    src/segment.cpp
    src/visualize.cpp)
target_include_directories(mflib PUBLIC ${CMAKE_SOURCE_DIR})

# Main CLI
add_executable (ImageSegmentation src/main.cpp)
target_link_libraries(ImageSegmentation PRIVATE mflib)