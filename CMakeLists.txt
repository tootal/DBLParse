cmake_minimum_required(VERSION 3.12)

project(DBLParse VERSION 3.1.0
        DESCRIPTION "An application base on DBLP data"
        HOMEPAGE_URL "https://tootal.xyz/DBLParse/"
        LANGUAGES C CXX)

add_subdirectory(bignumber)
add_subdirectory(pivoter)
add_subdirectory(src)
