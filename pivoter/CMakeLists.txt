# https://bitbucket.org/sjain12/pivoter

cmake_minimum_required(VERSION 3.5)

project(Pivoter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories(../bignumber)

add_library(pivoter STATIC
    LinkedList.cpp
    LinkedList.h
    degeneracy_algorithm_cliques_A.cpp
    degeneracy_algorithm_cliques_A.h
    degeneracy_helper.cpp
    degeneracy_helper.h
    misc.cpp
    misc.h
)

target_link_libraries(pivoter PRIVATE Qt5::Core)

target_compile_definitions(pivoter PRIVATE CMAKETEST4_LIBRARY)
