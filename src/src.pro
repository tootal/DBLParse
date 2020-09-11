QT       += core \
            gui \
            widgets \
            webenginewidgets \
            network \
            svg

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
INCLUDEPATH += $${PWD}/../pivoter

win32: {
    LIBS += $${OUT_PWD}/../pivoter/pivoter.lib
} else:unix {
    LIBS += $${OUT_PWD}/../pivoter/libpivoter.a
}

DEFINES += QT_DEPRECATED_WARNINGS

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    application.cpp \
    dialogs/changelogdialog.cpp \
    dialogs/authorstacdialog.cpp \
    configmanager.cpp \
    dialogs/cliquecountdialog.cpp \
    dialogs/documentdialog.cpp \
    dialogs/statusdialog.cpp \
    dialogs/downloaddialog.cpp \
    finder.cpp \
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
    dialogs/cliquecountdialog.h \
    dialogs/documentdialog.h \
    dialogs/statusdialog.h \
    dialogs/downloaddialog.h \
    finder.h \
    hash.h \
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
    dialogs/cliquecountdialog.ui \
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
