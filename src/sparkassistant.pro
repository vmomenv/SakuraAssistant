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
icon.files=$$PWD/icon/spark-store.png

desktop.path = /usr/share/applications/
desktop.files = $$PWD/sparkassistant.desktop
!isEmpty(target.path): INSTALLS += target desktop