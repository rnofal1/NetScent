QT       += core gui location quickwidgets positioning concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += file_copies
CONFIG -= debug_and_release debug_and_release_target

CONFIG += embed_manifest_exe
QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include
INCLUDEPATH += "C:\CPPINCLUDE\nlohmann\json\single_include"
INCLUDEPATH += "C:\CPPINCLUDE\curl-8.3.0\include"
INCLUDEPATH += "C:\CPPINCLUDE\npcap-sdk-1.13\Include"
INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22000.0\um"
INCLUDEPATH += "C:\CPPINCLUDE\boost_1_82_0"

LIBS += -L"C:\CPPINCLUDE\npcap-sdk-1.13\Lib\x64"
LIBS += -lwpcap
LIBS += -L"C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib"
LIBS += -llibcurl
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64"
LIBS += -lWS2_32
LIBS += -liphlpapi

SOURCES += \
    src/ComboCheckBox.cpp \
    src/CustomApplication.cpp \
    src/CustomButton.cpp \
    src/CustomCaptureTab.cpp \
    src/CustomCentralWidget.cpp \
    src/CustomMapTab.cpp \
    src/CustomTabWidget.cpp \
    src/InfoPane.cpp \
    src/LoadingLabel.cpp \
    src/MainWindow.cpp \
    src/NetworkAdapter.cpp \
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
    include/CustomMapTab.h \
    include/CustomTabWidget.h \
    include/InfoPane.h \
    include/LoadingLabel.h \
    include/MainWindow.h \
    include/NetworkAdapter.h \
    include/Packet.h \
    include/PacketCap.h \
    include/PacketLabel.h \
    include/PacketScrollArea.h \
    include/ProtocolHeaders.h \
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

# Copy over files
COPIES += copyFolders
copyFolders.files = $$files($$PWD/icons)
copyFolders.files += $$files($$PWD/style)
copyFolders.path = $$OUT_PWD

COPIES += copyFiles
copyFiles.files += $$files("C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib\libcurl.dll")
copyFiles.path = $$OUT_PWD

RESOURCES += \
    resources/Map.qrc
