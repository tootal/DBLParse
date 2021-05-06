cmake_minimum_required(VERSION 3.12)

file(READ scripts/VERSION_TWEAK VERSION_TWEAK)
project(DBLParse VERSION 3.2.0.${VERSION_TWEAK}
        DESCRIPTION "An application base on DBLP data."
        HOMEPAGE_URL "https://tootal.xyz/DBLParse/"
        LANGUAGES C CXX)
set(PROJECT_AUTHOR "tootal")
set(CMAKE_PROJECT_AUTHOR "${PROJECT_AUTHOR}")
set(PROJECT_LICENSE "GPL v3")
set(CMAKE_PROJECT_LICENSE "${PROJECT_LICENSE}")

add_subdirectory(pivoter)
add_subdirectory(src)
