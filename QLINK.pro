QT       += core gui
QT += widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += C:\Users\chenz\vcpkg\installed\x64-windows\include
LIBS += -LC:\Users\chenz\vcpkg\installed\x64-windows\lib -ljsoncpp
LIBS += -LC:\Users\chenz\vcpkg\installed\x64-windows\bin

INCLUDEPATH += C:/QT_PRACTICE/QLINK


SOURCES += \
    blocks.cpp \
    constNum.cpp \
    main.cpp \
    mainwindow.cpp \
    map.cpp \
    player.cpp \
    scoreboard.cpp \
    simpletest.cpp \
    timer.cpp

HEADERS += \
    blocks.h \
    constNum.h \
    mainwindow.h \
    map.h \
    player.h \
    scoreboard.h \
    simpletest.h \
    timer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
