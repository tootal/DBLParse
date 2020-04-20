CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off

SOURCES += \
    LinkedList.c \
    MemoryManager.c \
    degeneracy_algorithm_cliques_A.c \
    degeneracy_algorithm_cliques_E.c \
    degeneracy_algorithm_cliques_V.c \
    degeneracy_cliques.c \
    degeneracy_helper.c \
    misc.c

HEADERS += \
    LinkedList.h \
    MemoryManager.h \
    degeneracy_algorithm_cliques_A.h \
    degeneracy_algorithm_cliques_E.h \
    degeneracy_algorithm_cliques_V.h \
    degeneracy_cliques.h \
    degeneracy_helper.h \
    misc.h
