#created by QtCreator 2019-07-25T14:17:36
#
#-------------------------------------------------

QT       -= gui

TARGET = qextserialport
TEMPLATE = lib

DEFINES += QEXTSERIALPORT_BUILD_SHARED

CONFIG += c++11
CONFIG(debug,debug|release):TARGET = qextserialportd
CONFIG(release,debug|release):TARGET = qextserialport

win32:DESTDIR = lib/win32
unix:DESTDIR = lib/unix
MOC_DIR = build/moc
OBJECTS_DIR = build/obj
INCLUDEPATH += $$MOC_DIR

include($$PWD/src/qextserialport.pri)


unix:!symbian {
     maemo5 {
         target.path = /opt/usr/lib
     } else {
         target.path = /usr/lib
     }
     INSTALLS += target
}


