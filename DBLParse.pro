TEMPLATE = subdirs

SUBDIRS = \
    bignumber \
    src \
    pivoter

pivoter.depends = bignumber
src.depends = pivoter
