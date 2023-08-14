QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

QMAKE_CXXFLAGS = -std=c++20

LIBS += -lpcap
LIBS += -lcurl

SOURCES += \
    src/CustomButton.cpp \
    src/CustomLabel.cpp \
    src/MainWindow.cpp \
    src/Packet.cpp \
    src/PacketCap.cpp \
    src/main.cpp

HEADERS += \
    include/CustomButton.h \
    include/CustomLabel.h \
    include/MainWindow.h \
    include/Packet.h \
    include/PacketCap.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md

copydata_icons.commands = $(COPY_DIR) $$PWD/icons $$OUT_PWD
copydata_style.commands = $(COPY_DIR) $$PWD/style $$OUT_PWD

first.depends = $(first) copydata_icons copydata_style
export(first.depends)
export(copydata_icons.commands)
export(copydata_style.commands)

QMAKE_EXTRA_TARGETS += first copydata_icons copydata_style
