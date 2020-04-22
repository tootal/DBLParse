QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $${PWD}/../bignumber

CONFIG(debug, debug|release) {
    LIBS += $${OUT_PWD}/../bignumber/debug/bignumber.lib
}

CONFIG(release, debug|release) {
    LIBS += $${OUT_PWD}/../bignumber/release/bignumber.lib
}

SOURCES +=  tst_bignumber.cpp
