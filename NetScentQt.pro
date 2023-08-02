QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

QMAKE_CXXFLAGS = -std=c++20

LIBS += -lpcap

SOURCES += \
    src/Packet.cpp \
    src/PacketCap.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    include/Packet.h \
    include/PacketCap.h \
    include/customlabel.h \
    include/mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore
