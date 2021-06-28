# https://bitbucket.org/sjain12/pivoter

cmake_minimum_required(VERSION 3.10)

project(Pivoter LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)

add_library(${PROJECT_NAME} STATIC 
    ${SOURCE_FILES} 
    ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Qt6::Core
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}
)

