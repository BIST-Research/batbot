cmake_minimum_required(VERSION 3.20)
project(CBatLib)

set(BUILD_SHARED_LIBS true)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${pybind11_INCLUDE_DIRS})

add_subdirectory(src)

pybind11_add_module(tendonhardware src/tendon_hardware_interface.cpp)
target_link_libraries(tendonhardware PUBLIC serial)
