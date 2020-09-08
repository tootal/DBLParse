# https://bitbucket.org/sjain12/pivoter

QT       += core

TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off
CONFIG += c++17
CONFIG -= debug_and_release

SOURCES += \
    LinkedList.cpp \
    degeneracy_algorithm_cliques_A.cpp \
    degeneracy_helper.cpp \
    misc.cpp \
    bignumber.cpp

HEADERS += \
    LinkedList.h \
    degeneracy_algorithm_cliques_A.h \
    degeneracy_helper.h \
    misc.h \
    bignumber.h
