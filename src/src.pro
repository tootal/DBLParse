QT       += core gui webenginewidgets network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG -= debug_and_release

# Enable message log in release build
DEFINES += QT_MESSAGELOGCONTEXT

# Disable foreach keywords
DEFINES += QT_NO_FOREACH

TARGET = DBLParse
TEMPLATE = app

RC_ICONS = resources/DBLParse.ico

QTQUICK_COMPILER_SKIPPED_RESOURCES += DBLParse.qrc

# Version
VERSION_MAJOR = 3
VERSION_MINOR = 2
VERSION_PATCH = 0
win32 {
    VERSION_BUILD = $$system(git log --pretty=oneline | find /V \"\" /C)
    VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}.$${VERSION_BUILD}
    DEFINES += VERSION_STR=\\\"$${VERSION}\\\"
} else:unix {
    VERSION_BUILD = $$system(git log --pretty=oneline | wc -l)
    VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_PATCH}
    DEFINES += VERSION_STR=\\\"$${VERSION}.$${VERSION_BUILD}\\\"
}

# Header Include
INCLUDEPATH += $${PWD}/../bignumber
INCLUDEPATH += $${PWD}/../pivoter

win32: {
    LIBS += $${OUT_PWD}/../pivoter/pivoter.lib
    LIBS += $${OUT_PWD}/../bignumber/bignumber.lib
} else:unix {
    LIBS += $${OUT_PWD}/../pivoter/libpivoter.a
    LIBS += $${OUT_PWD}/../bignumber/libbignumber.a
}

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
    application.cpp \
    dialogs/changelogdialog.cpp \
    dialogs/authorstacdialog.cpp \
    configmanager.cpp \
    dialogs/documentdialog.cpp \
    dialogs/statusdialog.cpp \
    dialogs/downloaddialog.cpp \
    finder.cpp \
    loader.cpp \
    main.cpp \
    mainwindow.cpp \
    networker.cpp \
    dialogs/parsedialog.cpp \
    parser.cpp \
    record.cpp \
    dialogs/settingsdialog.cpp \
    stemmer.cpp \
    util.cpp \
    webpage.cpp \
    webview.cpp \
    widgets/loadinglabel.cpp \
    widgets/statuslabel.cpp

HEADERS += \
    application.h \
    dialogs/changelogdialog.h \
    dialogs/authorstacdialog.h \
    configmanager.h \
    dialogs/documentdialog.h \
    dialogs/statusdialog.h \
    dialogs/downloaddialog.h \
    finder.h \
    hash.h \
    loader.h \
    mainwindow.h \
    networker.h \
    dialogs/parsedialog.h \
    parser.h \
    reader.h \
    record.h \
    saver.h \
    dialogs/settingsdialog.h \
    stemmer.h \
    util.h \
    webpage.h \
    webview.h \
    widgets/loadinglabel.h \
    widgets/statuslabel.h

TRANSLATIONS += \
    DBLParse_zh_CN.ts

FORMS += \
    dialogs/changelogdialog.ui \
    dialogs/authorstacdialog.ui \
    dialogs/documentdialog.ui \
    dialogs/statusdialog.ui \
    dialogs/downloaddialog.ui \
    mainwindow.ui \
    dialogs/parsedialog.ui \
    dialogs/settingsdialog.ui \
    widgets/loadinglabel.ui \
    widgets/statuslabel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    DBLParse.qrc

DISTFILES += \
    resources/no.svg \
    resources/ok.svg
