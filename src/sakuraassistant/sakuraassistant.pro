QT += core gui dtkwidget network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sakuraassistant
TEMPLATE = app

HEADERS += \
    mainwindow.h \
    weatherparse.h \
    iptoweathercity.h \
    todo.h \
    monitorthread.h \
    todoclassmanager.h \
    passbook.h \
    aesni/aesni-enc-cbc.h \
    aesni/aesni-enc-ecb.h \
    aesni/aesni-key-exp.h \
    qaesencryption.h \
    labelbutton.h \
    passbookpassword.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    weatherparse.cpp \
    iptoweathercity.cpp \
    todo.cpp \
    monitorthread.cpp \
    todoclassmanager.cpp \
    passbook.cpp \
    qaesencryption.cpp \
    labelbutton.cpp \
    passbookpassword.cpp

RESOURCES += \
    resources.qrc


target.path=/usr/bin


icon.path =/usr/share/icons
icon.files+= pkg/usr/share/icons/hicolor/scalable/apps/sakura.svg

desktop.path = /usr/share/applications/
desktop.files+= pkg/usr/share/applications/sakura-assistant.desktop
!isEmpty(target.path): INSTALLS += target icon desktop

FORMS +=
