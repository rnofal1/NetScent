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
LIBS += -lboost_system
LIBS += -lboost_filesystem
LIBS += -lboost_iostreams
LIBS += -lboost_serialization

SOURCES += \
    src/ComboCheckBox.cpp \
    src/CustomApplication.cpp \
    src/CustomButton.cpp \
    src/CustomCaptureTab.cpp \
    src/CustomCentralWidget.cpp \
    src/CustomTabWidget.cpp \
    src/InfoPane.cpp \
    src/MainWindow.cpp \
    src/Packet.cpp \
    src/PacketCap.cpp \
    src/PacketLabel.cpp \
    src/PacketScrollArea.cpp \
    src/StyleWidget.cpp \
    src/main.cpp \
    src/util.cpp

HEADERS += \
    include/ComboCheckBox.h \
    include/CustomApplication.h \
    include/CustomButton.h \
    include/CustomCaptureTab.h \
    include/CustomCentralWidget.h \
    include/CustomTabWidget.h \
    include/InfoPane.h \
    include/MainWindow.h \
    include/Packet.h \
    include/PacketCap.h \
    include/PacketLabel.h \
    include/PacketScrollArea.h \
    include/StyleWidget.h \
    include/util.h

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
