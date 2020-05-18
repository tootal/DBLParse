# https://bitbucket.org/sjain12/pivoter

CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off
CONFIG += c++11
CONFIG -= debug_and_release
INCLUDEPATH += $${PWD}/../bignumber

SOURCES += \
    LinkedList.cpp \
    degeneracy_algorithm_cliques_A.cpp \
    degeneracy_helper.cpp \
    misc.cpp

HEADERS += \
    LinkedList.h \
    degeneracy_algorithm_cliques_A.h \
    degeneracy_helper.h \
    misc.h
