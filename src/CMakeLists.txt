cmake_minimum_required(VERSION 3.10)

project(DBLParseSrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6
    COMPONENTS 
        Widgets 
        Network
        WebEngineWidgets
        LinguistTools 
        Svg
    REQUIRED)

file(GLOB_RECURSE HEADER_FILES *.h)
file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE QRC_FILES *.qrc)
file(GLOB_RECURSE TS_FILES *.ts)
file(GLOB_RECURSE UI_FILES *.ui)

add_executable(DBLParse MACOSX_BUNDLE WIN32)
target_sources(DBLParse
    PRIVATE
        ${SRC_FILES}
        ${HEADER_FILES}
        ${QRC_FILES}
        ${UI_FILES}
        ${TS_FILES})

target_compile_definitions(DBLParse 
    PRIVATE
        VERSION_STR="${CMAKE_PROJECT_VERSION}")

target_link_libraries(DBLParse 
    PRIVATE 
        Qt6::Widgets
        Qt6::WebEngineWidgets
        Qt6::Network
        Qt6::Svg
        Pivoter)

if(WIN32)
    configure_file(${PROJECT_SOURCE_DIR}/resources/DBLParse.rc.in 
        ${PROJECT_BINARY_DIR}/DBLParse.rc)
    target_sources(DBLParse
        PRIVATE 
            ${PROJECT_BINARY_DIR}/DBLParse.rc)
elseif(APPLE)
    set(MACOSX_ICON_NAME "DBLParse.icns")
    set(MACOSX_ICON_FILE "${CMAKE_SOURCE_DIR}/src/resources/${MACOSX_ICON_NAME}")
    target_sources(DBLParse
        PRIVATE
            ${MACOSX_ICON_FILE})
    set_target_properties(DBLParse PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_COPYRIGHT "Distributed under GPLv3 license. Copyright 2021 tootal"
        MACOSX_BUNDLE_GUI_IDENTIFIER "xyz.tootal.DBLParse"
        MACOSX_BUNDLE_ICON_FILE "${MACOSX_ICON_NAME}"
        MACOSX_BUNDLE_INFO_STRING "DBLParse is a set of high-efficiency document query and analysis system based on DBLP big data"
        MACOSX_BUNDLE_LONG_VERSION_STRING ${MACOSX_BUNDLE_BUNDLE_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR})
    set_source_files_properties(${MACOSX_ICON_FILE} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
elseif(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()


qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
