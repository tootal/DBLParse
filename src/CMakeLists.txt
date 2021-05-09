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
        Qt5::Widgets
        Qt5::WebEngineWidgets
        Qt5::Network
        Qt5::Svg
        Pivoter)

if(WIN32)
    configure_file("${PROJECT_SOURCE_DIR}/resources/DBLParse.rc.in"
        "${PROJECT_BINARY_DIR}/DBLParse.rc")
    target_sources(DBLParse
        PRIVATE 
            "${PROJECT_BINARY_DIR}/DBLParse.rc")
elseif(APPLE)
    set(MACOSX_ICON_NAME "Icon.icns")
    set(MACOSX_ICON_FILE "${CMAKE_SOURCE_DIR}/src/resources/${MACOSX_ICON_NAME}")
    set(MACOSX_IDENTIFIER "xyz.tootal.DBLParse")
    configure_file("${PROJECT_SOURCE_DIR}/resources/Info.plist.in"
        "${PROJECT_BINARY_DIR}/Info.plist")
    set(MACOSX_BUNDLE_INFO_PLIST "${PROJECT_BINARY_DIR}/Info.plist")
    target_sources(DBLParse
        PRIVATE
            ${MACOSX_ICON_FILE})
    set_source_files_properties(${MACOSX_ICON_FILE} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(DBLParse PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
elseif(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()


qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
