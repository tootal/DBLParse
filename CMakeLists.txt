cmake_minimum_required(VERSION 3.12)

project(DBLParse VERSION 3.1.0
        DESCRIPTION "An application base on DBLP data"
        HOMEPAGE_URL "https://tootal.xyz/DBLParse/"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Widgets
    Network
    WebEngineWidgets
    LinguistTools    
)

add_subdirectory(bignumber)
add_subdirectory(pivoter)
add_subdirectory(src)
