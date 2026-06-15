#-------------------------------------------------
#
# Project created by QtCreator 2019-08-30T09:30:52
#
#-------------------------------------------------

QT       -= gui

CONFIG(debug,debug|release):TARGET = jsoncppd
CONFIG(release,debug|release):TARGET = jsoncpp
win32:DESTDIR = lib/win32
unix:DESTDIR = lib/unix

MOC_DIR += ./build/tmp
OBJECTS_DIR += ./build/tmp

TEMPLATE = lib
INCLUDEPATH += $$PWD/json
DEFINES += JSON_DLL_BUILD
CONFIG+=c++11
SOURCES += jsoncpp.cpp
HEADERS += \
    json/json-forwards.h \
    json/json.h

unix:!symbian {
     maemo5 {
         target.path = /opt/usr/lib
     } else {
         target.path = /usr/lib
     }
     INSTALLS += target
}
