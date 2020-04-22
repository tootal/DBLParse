CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off
CONFIG += c++11
INCLUDEPATH += $${PWD}/../bignumber

SOURCES += \
    LinkedList.cpp \
    MemoryManager.cpp \
    degeneracy_algorithm_cliques_A.cpp \
    degeneracy_cliques.cpp \
    degeneracy_helper.cpp \
    misc.cpp

HEADERS += \
    LinkedList.h \
    MemoryManager.h \
    degeneracy_algorithm_cliques_A.h \
    degeneracy_cliques.h \
    degeneracy_helper.h \
    misc.h
