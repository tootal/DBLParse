TEMPLATE = subdirs

SUBDIRS = \
    src \
    pivoter \
    tst_pivoter

src.depends = pivoter
tst_pivoter.depends = pivoter
