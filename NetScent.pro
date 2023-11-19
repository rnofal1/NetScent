# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT       += core gui location quickwidgets positioning concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 file_copies embed_manifest_exe

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT

QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

INCLUDEPATH += include \
            include/util \
            "C:\CPPINCLUDE\nlohmann\json\single_include" \
            "C:\CPPINCLUDE\curl-8.3.0\include" \
            "C:\CPPINCLUDE\npcap-sdk-1.13\Include" \
            "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22000.0\um" \
            "C:\CPPINCLUDE\boost_1_82_0" \

LIBS += -L"C:\CPPINCLUDE\npcap-sdk-1.13\Lib\x64" \
        -lwpcap \
        -L"C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib" \
        -llibcurl \
        -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64" \
        -lWS2_32 \
        -liphlpapi

# Qt Creator auto-handles SOURCES most of the time
SOURCES += \
    src/CustomApplication.cpp \
    src/CustomButton.cpp \
    src/CustomCaptureTab.cpp \
    src/CustomCentralWidget.cpp \
    src/CustomMapTab.cpp \
    src/CustomSettingsTab.cpp \
    src/CustomTabWidget.cpp \
    src/DeviceSelectBox.cpp \
    src/InfoPane.cpp \
    src/KeyDetectedLabel.cpp \
    src/LoadingLabel.cpp \
    src/MainWindow.cpp \
    src/NetworkAdapter.cpp \
    src/NumDisplay.cpp \
    src/Packet.cpp \
    src/PacketCap.cpp \
    src/PacketFilterCheckBox.cpp \
    src/PacketLabel.cpp \
    src/PacketTableView.cpp \
    src/StyleWidget.cpp \
    src/main.cpp \
    src/util/util.cpp

# Qt Creator auto-handles HEADERS most of the time
HEADERS += \
    include/CustomApplication.h \
    include/CustomButton.h \
    include/CustomCaptureTab.h \
    include/CustomCentralWidget.h \
    include/CustomMapTab.h \
    include/CustomSettingsTab.h \
    include/CustomTabWidget.h \
    include/DeviceSelectBox.h \
    include/InfoPane.h \
    include/KeyDetectedLabel.h \
    include/LoadingLabel.h \
    include/MainWindow.h \
    include/NetworkAdapter.h \
    include/NumDisplay.h \
    include/Packet.h \
    include/PacketCap.h \
    include/PacketFilterCheckBox.h \
    include/PacketLabel.h \
    include/PacketTableView.h \
    include/ProtocolHeaders.h \
    include/SharedQueue.h \
    include/StyleWidget.h \
    include/util/util.h

# Qt Creator auto-handles FORMS most of the time
FORMS += \
    mainwindow.ui

RESOURCES += \
    resources/Map.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    README.md \
    style/style.qss

# Copy over files
COPIES += copyFolders
copyFolders.files = $$files($$PWD/icons)
copyFolders.files += $$files($$PWD/style)
copyFolders.path = $$OUT_PWD

COPIES += copyFiles
copyFiles.files += $$files("C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib\libcurl.dll")
copyFiles.path = $$OUT_PWD
