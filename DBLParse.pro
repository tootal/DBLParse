TEMPLATE = subdirs

SUBDIRS = \
    bignumber \
    src \
    pivoter \
    test

pivoter.depends = bignumber
src.depends = pivoter
test.depends = bignumber
