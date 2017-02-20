QT += qml quick webengine websockets network xml core

CONFIG += c++14 c++11

SOURCES += main.cpp \
    service.cpp \
    twitch.cpp \
    socket.cpp \
    websocketserver.cpp \
    servicemanager.cpp \
    settings.cpp \
    myrequesthandler.cpp \
    requestmapper.cpp \
    simplecrypt.cpp \
    encryptedsettings.cpp \
    hexstring.cpp \
    helper.cpp \
    servicesmodel.cpp \
    styleparser.cpp

RESOURCES += qml.qrc \
    iconset.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    service.h \
    twitch.h \
    socket.h \
    websocketserver.h \
    servicemanager.h \
    settings.h \
    myrequesthandler.h \
    requestmapper.h \
    simplecrypt.h \
    encryptedsettings.h \
    hexstring.h \
    helper.h \
    servicesmodel.h \
    styleparser.h


DISTFILES += \
    web/index.html \

#---------------------------------------------------------------------------------------
# The following lines include the sources of the Web Server library
#---------------------------------------------------------------------------------------
include(../hobrasofthttp-1.0.7/src/hobrasofthttpd.pri)
#include(QtWebApp/logging/logging.pri)
#include(QtWebApp/httpserver/httpserver.pri)
#include(QtWebApp/templateengine/templateengine.pri)
# Not used: include(../QtWebApp/qtservice/qtservice.pri)

QTBINPATH = D:/Qt/Qt5.8.0_msvc2015/5.8/msvc2015_64/bin

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}



CONFIG( debug, debug|release ) {
    # debug
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
    DESTDIR = debug
} else {
    # release
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
    DESTDIR = release
}

win32 {
    web.path = $${OUT_PWD}/$${DESTDIR}/web
    web.files = web/*
    DEPLOY_COMMAND = windeployqt --qmldir $${PWD}
}
macx {
    DEPLOY_COMMAND = macdeployqt
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

# Use += instead of = if you use multiple QMAKE_POST_LINKs
QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}

INSTALLS += web
