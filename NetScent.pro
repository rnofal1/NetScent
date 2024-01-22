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
            include/widgets \
            include/network \
            "C:\CPPINCLUDE\nlohmann\json\single_include" \                      # MODIFY to local path
            "C:\CPPINCLUDE\curl-8.3.0\include" \                                # MODIFY to local path
            "C:\CPPINCLUDE\npcap-sdk-1.13\Include" \                            # MODIFY to local path
            "C:\Program Files (x86)\Windows Kits\10\Include\10.0.22000.0\um" \  # MODIFY to local path
            "C:\CPPINCLUDE\boost_1_82_0" \                                      # MODIFY to local path

LIBS += -L"C:\CPPINCLUDE\npcap-sdk-1.13\Lib\x64" \                                              # MODIFY to local path
        -lwpcap \
        -L"C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib" \ # MODIFY to local path
        -llibcurl \
        -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64" \                    # MODIFY to local path
        -lWS2_32 \
        -liphlpapi

# Recursively add all .cpp files to sources
SOURCES += $$files("*.cpp", true) \
    src/util/AStar.cpp

# Recursively add all .h files to headers
HEADERS += $$files("*.h", true) \
    include/util/AStar.h

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
copyFiles.files += $$files("C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib\libcurl.dll") # MODIFY to local path
copyFiles.path = $$OUT_PWD
