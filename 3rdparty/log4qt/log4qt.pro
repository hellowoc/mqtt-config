
include(src/Log4Qt.pri)

TEMPLATE = lib

QT += core xml network
CONFIG += c++11

DEFINES += LOG4QT_EXPORTS

CONFIG(debug,debug|release):TARGET = log4qtd
CONFIG(release,debug|release):TARGET = log4qt
win32:DESTDIR = lib/win32
unix:DESTDIR = lib/unix
MOC_DIR = build/moc
OBJECTS_DIR = build/obj

LOG4QT_VERSION_MAJOR = 1
LOG4QT_VERSION_MINOR = 0
LOG4QT_VERSION_RELEASE = 0

LOG4QT_VERSION = '\\"$${LOG4QT_VERSION_MAJOR}.$${LOG4QT_VERSION_MINOR}.$${LOG4QT_VERSION_RELEASE}\\"'
DEFINES += LOG4QT_VERSION_STR=\"$${LOG4QT_VERSION}\"
DEFINES += LOG4QT_VERSION=$${LOG4QT_VERSION}

#VERSION = 1.0.0.0

#QMAKE_LFLAGS_SONAME = ""
#QMAKE_LFLAGS_SONAME += -install_name$${LITERAL_WHITESPACE}"@executable_path/"


DEPENDPATH += src src/helpers src/spisrc/ varia
INCLUDEPATH += src src/helpers src/spi src/varia

unix:!symbian {
     maemo5 {
         target.path = /opt/usr/lib
     } else {
         target.path = /usr/lib
     }
     INSTALLS += target
}

