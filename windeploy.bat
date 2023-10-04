:: This script creates a deployable folder containing the NetScent executable and its dependencies (SOME VARIABLES MUST BE MODIFIED TO WORK ON YOUR SYSTEM)
@echo off

::------------------------------------------
:: Modify these to point to the appropriate paths on your system
:: Path to directory containing NetScent.exe
set executableDirPath=C:\build-NetScent-Desktop_Qt_6_5_2_MSVC2019_64bit-Release

:: Path to libcurl.dll File (not Directory)
set libcurlBinPath=C:\CPPINCLUDE\curl-8.3.0\builds\libcurl-vc-x64-release-dll-ipv6-sspi-schannel\lib\libcurl.dll 

:: Path to directory where executable and dependencies will be copied
set releaseDirPath=C:\NetScentReleases\NetScent_Release_WIN_v0.0.1

:: List of paths to files that we want to directly copy over (not handled by windeployqt)
set srcFileList=%libCurlBinPath% %executableDirPath%\NetScent.exe

:: List of paths to subdirectories within the executable directory we want to directly copy over (not handled by windeployqt)
set srcSubDirList=style icons

::------------------------------------------
:: Create directory at releaseDirPath
if not exist %releaseDirPath% mkdir %releaseDirPath%

::------------------------------------------
:: Copy all files within srcFileList to releaseDirPath
(for %%a in (%srcFileList%) do ( 
   xcopy /s/y/i %%a %releaseDirPath% 
))

::------------------------------------------
:: Create directories in srcSubDirList within releaseDirPath
(for %%a in (%srcSubDirList%) do ( 
   if not exist %releaseDirPath%\%%a\ mkdir %releaseDirPath%\%%a
))

:: Copy all directories within srcSubDirList to releaseDirPath
 (for %%a in (%srcSubDirList%) do ( 
   xcopy /s/y/i %executableDirPath%\%%a %releaseDirPath%\%%a
))

::------------------------------------------
:: Run Qt Windows Deployment tool (windeployqt)
windeployqt %executableDirPath% --libdir %releaseDirPath% --plugindir %releaseDirPath%
