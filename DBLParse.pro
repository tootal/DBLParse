QT       += core gui webenginewidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Enable message log in release build
DEFINES += QT_MESSAGELOGCONTEXT

TARGET = DBLParse
TEMPLATE = app

RC_ICONS = resources/DBLParse.ico
VERSION = 2.5
DEFINES += VERSION=$${VERSION}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
    
# Disables automatic conversions from C string literals and pointers to Unicode.
# Disables automatic conversion from QString to C strings.
#DEFINES += QT_NO_CAST_FROM_ASCII \
#             QT_NO_CAST_TO_ASCII

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculator.cpp \
    configmanager.cpp \
    detailpage.cpp \
    detailview.cpp \
    authorstacdialog.cpp \
    finder.cpp \
    loader.cpp \
    main.cpp \
    mainwindow.cpp \
    parsedialog.cpp \
    parser.cpp \
    record.cpp \
    settingsdialog.cpp \
    util.cpp \
    webpage.cpp \
    webview.cpp

HEADERS += \
    calculator.h \
    configmanager.h \
    detailpage.h \
    detailview.h \
    authorstacdialog.h \
    finder.h \
    loader.h \
    mainwindow.h \
    parsedialog.h \
    parser.h \
    record.h \
    settingsdialog.h \
    util.h \
    webpage.h \
    webview.h

TRANSLATIONS += \
    translations/DBLParse_zh_CN.ts

FORMS += \
    authorstacdialog.ui \
    mainwindow.ui \
    parsedialog.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    DBLParse.qrc \
    translations/translations.qrc
