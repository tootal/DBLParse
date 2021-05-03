cmake_minimum_required(VERSION 3.10)

project(DBLParseSrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 
    COMPONENTS 
        Widgets 
        Network
        WebEngineWidgets
        LinguistTools 
        Svg
    REQUIRED
)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE QRC_FILES *.qrc)
file(GLOB_RECURSE TS_FILES *.ts)
file(GLOB_RECURSE UI_FILES *.ui)

add_executable(DBLParse
    ${SRC_FILES}
    ${HEADER_FILES}
    ${QRC_FILES}
    ${UI_FILES}
    ${TS_FILES}
)

target_compile_definitions(DBLParse 
    PRIVATE
        VERSION_STR="3.1.0"
)

target_link_libraries(DBLParse 
    PRIVATE 
        Qt5::Widgets
        Qt5::WebEngineWidgets
        Qt5::Network
        Qt5::Svg
        Pivoter
        BigNumber
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
