
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets Network
             WebEngineWidgets
             LinguistTools REQUIRED)
add_compile_definitions(VERSION_STR="3.1.0")

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

target_link_libraries(DBLParse ${PROJECT_BINARY_DIR}/../pivoter/pivoter.lib)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
