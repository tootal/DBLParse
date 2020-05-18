QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $${PWD}/../bignumber

win32:CONFIG(debug, debug|release) {
    LIBS += $${OUT_PWD}/../bignumber/debug/bignumber.lib
} else:win32:CONFIG(release, debug|release) {
    LIBS += $${OUT_PWD}/../bignumber/release/bignumber.lib
} else:unix {
    LIBS += $${OUT_PWD}/../bignumber/libbignumber.a
}

SOURCES +=  tst_bignumber.cpp
