TARGET = uikit
TEMPLATE = lib

DEFINES += UIKITS_SHARED_EXPORTS
DEFINES += OS_ARM335x
QT +=core gui widgets
CONFIG += c++11
CONFIG(debug,debug|release):TARGET = uikitsd
CONFIG(release,debug|release):TARGET = uikits

win32:DESTDIR = lib/win32
contains(DEFINES,OS_ARM335x){
unix:DESTDIR = lib/unix
}else{
unix:DESTDIR = lib/unix_x86
}
MOC_DIR = build/moc
OBJECTS_DIR = build/obj


include($$PWD/src/uiKits.pri)

unix:!symbian {
     maemo5 {
         target.path = /opt/usr/lib
     } else {
         target.path = /usr/lib
     }
     INSTALLS += target
}
