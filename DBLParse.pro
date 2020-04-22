TEMPLATE = subdirs

SUBDIRS = \
    bignumber \
    src \
    pivoter

src.depends = pivoter
pivoter.depends = bignumber
