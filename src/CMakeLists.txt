cmake_minimum_required(VERSION 3.5)

project(DBLParse LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES CmakeQtTest_zh_CN.ts)

add_executable(CmakeQtTest
main.cpp
mainwindow.cpp
mainwindow.h
mainwindow.ui
${TS_FILES}
)

target_link_libraries(CmakeQtTest PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
