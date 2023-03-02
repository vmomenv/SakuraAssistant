QT += core gui dtkwidget network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sparkassistant
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    weatherparse.cpp \
    iptoweathercity.cpp \
    todo.cpp \
    monitorthread.cpp \
    todoclassmanager.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    mainwindow.h \
    weatherparse.h \
    iptoweathercity.h \
    todo.h \
    monitorthread.h \
    todoclassmanager.h

target.path=/usr/bin


icon.path =/usr/share/icons
icon.files+= pkg/usr/share/icons/hicolor/scalable/apps/spark-store.svg

desktop.path = /usr/share/applications/
desktop.files+= pkg/usr/share/applications/sparkassistant.desktop
!isEmpty(target.path): INSTALLS += target icon desktop
