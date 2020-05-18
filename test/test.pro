QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG -= debug_and_release
TEMPLATE = app

INCLUDEPATH += $${PWD}/../bignumber

win32 {
    LIBS += $${OUT_PWD}/../bignumber/bignumber.lib
} else:unix {
    LIBS += $${OUT_PWD}/../bignumber/libbignumber.a
}

SOURCES +=  tst_bignumber.cpp
