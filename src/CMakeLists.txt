cmake_minimum_required(VERSION 3.5)

project(DBLParse VERSION 3.1.0
        DESCRIPTION "An application base on DBLP data"
        HOMEPAGE_URL "https://tootal.xyz/DBLParse/"
        LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network
             WebEngineWidgets
             LinguistTools REQUIRED)

include_directories(../bignumber ../pivoter)

file(GLOB HEADER_FILES *.h)
file(GLOB SRC_FILES *.cpp)
file(GLOB QRC_FILES *.qrc)
file(GLOB TS_FILES *.ts)
file(GLOB UI_FILES *.ui)

add_executable(DBLParse
    ${SRC_FILES}
    ${HEADER_FILES}
    ${QRC_FILES}
    ${UI_FILES}
    ${TS_FILES}
)

target_link_libraries(DBLParse PRIVATE 
    Qt5::Widgets
    Qt5::WebEngineWidgets
    Qt5::Network
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
