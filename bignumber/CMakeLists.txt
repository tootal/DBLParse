cmake_minimum_required(VERSION 3.5)
project(BigNumber LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bignumber STATIC
    bignumber.cpp
    bignumber.h
)

target_compile_definitions(bignumber PRIVATE BIGNUMBER_LIBRARY)

