cmake_minimum_required(VERSION 3.20)
project(batbot)

set(BUILD_SHARED_LIBS true)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/c_lib/include)

add_subdirectory(c_lib)
