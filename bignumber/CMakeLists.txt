cmake_minimum_required(VERSION 3.12)

project(BigNumber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bignumber STATIC
    bignumber.cpp
    bignumber.h
)

