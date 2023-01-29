QT += core gui dtkwidget network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkAssistant
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    weatherparse.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    mainwindow.h \
    weatherparse.h
