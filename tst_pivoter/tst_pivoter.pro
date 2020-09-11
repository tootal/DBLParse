QT       += core gui widgets

CONFIG += c++17
CONFIG -= debug_and_release

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000   

# Header Include
INCLUDEPATH += $${PWD}/../pivoter

win32: {
    LIBS += $${OUT_PWD}/../pivoter/pivoter.lib
} else:unix {
    LIBS += $${OUT_PWD}/../pivoter/libpivoter.a
}

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
