project(jit)
cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "-O3 -fopenmp -g")
include_directories(".")

find_package(ROOT REQUIRED COMPONENTS Core)
include(${ROOT_USE_FILE})

# Dictionary
set(DICT_FILE "functor_dict")
REFLEX_GENERATE_DICTIONARY(${DICT_FILE} "functor.h" SELECTION "selection.xml")
add_library(functor_dictDict SHARED  ${DICT_FILE})
target_link_libraries(functor_dictDict ${ROOT_LIBRARIES})

add_executable(jit main.cc benchmark.cc)
target_link_libraries(jit functor_dictDict)
