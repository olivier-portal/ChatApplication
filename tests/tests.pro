QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_clientmanager

CLIENT_DIR = $$clean_path($$PWD/../Client)

INCLUDEPATH += $$CLIENT_DIR/En-têtes

SOURCES += \
    Sources/tst_clientmanager.cpp \
    $$CLIENT_DIR/Sources/clientmanager.cpp \
    tst_clientmanager.cpp

HEADERS += \
    $$CLIENT_DIR/En-têtes/clientmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
