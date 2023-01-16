QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SparkAssistant
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    mainwindow.h
