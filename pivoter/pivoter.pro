CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off

SOURCES += \
    LinkedList.cpp \
    MemoryManager.cpp \
    degeneracy_algorithm_cliques_A.cpp \
    degeneracy_algorithm_cliques_E.cpp \
    degeneracy_algorithm_cliques_V.cpp \
    degeneracy_cliques.cpp \
    degeneracy_helper.cpp \
    misc.cpp

HEADERS += \
    LinkedList.h \
    MemoryManager.h \
    degeneracy_algorithm_cliques_A.h \
    degeneracy_algorithm_cliques_E.h \
    degeneracy_algorithm_cliques_V.h \
    degeneracy_cliques.h \
    degeneracy_helper.h \
    misc.h
