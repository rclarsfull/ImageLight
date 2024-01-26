QT  += core gui
QT  += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    converter.cpp \
    main.cpp \
    mainwindow.cpp \
    messurebox.cpp \
    perfomancetimer.cpp \
    serverconnetor.cpp \
    settingswindow.cpp
    converter.cpp

HEADERS += \
    canvas.h \
    converter.h \
    global.h \
    mainwindow.h \
    messurebox.h \
    serverconnetor.h \
    settingswindow.h \
    converter.h \
    drawable.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
