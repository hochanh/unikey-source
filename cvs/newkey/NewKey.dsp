# Microsoft Developer Studio Project File - Name="NewKey" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NewKey - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NewKey.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NewKey.mak" CFG="NewKey - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NewKey - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NewKey - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "NewKey - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NewKey - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../keyhook" /I "../vnconv" /I "../rtfio" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"prehdr.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_ANSI"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 gdi32.lib user32.lib shell32.lib Advapi32.lib Comdlg32.lib /nologo /stack:0x4e20,0x1000 /subsystem:windows /machine:I386 /out:"../Release/UniKey.exe"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "NewKey - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../keyhook" /I "../vnconv" /I "../rtfio" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"prehdr.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_ANSI"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdi32.lib kernel32.lib user32.lib advapi32.lib shell32.lib Comdlg32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "NewKey - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NewKey___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "NewKey___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../URelease"
# PROP Intermediate_Dir "URelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../keyhook" /I "../vnconv" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"prehdr.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../keyhook" /I "../vnconv" /I "../rtfio" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"prehdr.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_UNICODE"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib Comdlg32.lib gdi32.lib user32.lib shell32.lib Advapi32.lib ../vnconv/Release/vnconv.lib ../SRelease/ukhook35.lib /nologo /stack:0x4e20,0x1000 /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"../RSUnicode/UniKey.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib Comdlg32.lib gdi32.lib user32.lib shell32.lib Advapi32.lib /nologo /stack:0x4e20,0x1000 /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /out:"../URelease/UniKeyNT.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NewKey - Win32 Release"
# Name "NewKey - Win32 Debug"
# Name "NewKey - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\about.cpp
# End Source File
# Begin Source File

SOURCE=.\button.cpp
# End Source File
# Begin Source File

SOURCE=.\encode.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\keydlg.cpp
# End Source File
# Begin Source File

SOURCE=.\label.cpp
# End Source File
# Begin Source File

SOURCE=.\macrodlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mactab.cpp
# End Source File
# Begin Source File

SOURCE=.\mainwnd.cpp
# End Source File
# Begin Source File

SOURCE=.\odmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\prehdr.cpp
# ADD CPP /Yc"prehdr.h"
# End Source File
# Begin Source File

SOURCE=.\smedit.cpp
# End Source File
# Begin Source File

SOURCE=.\tooldlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ukmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\userpref.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\about.h
# End Source File
# Begin Source File

SOURCE=.\button.h
# End Source File
# Begin Source File

SOURCE=.\encode.h
# End Source File
# Begin Source File

SOURCE=.\ExpDlg.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\keydlg.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\macrodlg.h
# End Source File
# Begin Source File

SOURCE=.\mactab.h
# End Source File
# Begin Source File

SOURCE=.\mainwnd.h
# End Source File
# Begin Source File

SOURCE=.\odmenu.h
# End Source File
# Begin Source File

SOURCE=.\preHdr.h
# End Source File
# Begin Source File

SOURCE=.\smedit.h
# End Source File
# Begin Source File

SOURCE=.\tooldlg.h
# End Source File
# Begin Source File

SOURCE=.\ukmenu.h
# End Source File
# Begin Source File

SOURCE=.\userpref.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\2arrow.ico
# End Source File
# Begin Source File

SOURCE=.\res\3keys.ico
# End Source File
# Begin Source File

SOURCE=.\res\3keys_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\Add.ico
# End Source File
# Begin Source File

SOURCE=.\res\ARW05DN.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW06DN.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW06UP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\convert.ico
# End Source File
# Begin Source File

SOURCE=.\res\Delete.ico
# End Source File
# Begin Source File

SOURCE=.\res\FACE01.ICO
# End Source File
# Begin Source File

SOURCE=.\res\FACE02.ICO
# End Source File
# Begin Source File

SOURCE=.\res\H_POINT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\help.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\RES\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\info.ico
# End Source File
# Begin Source File

SOURCE=.\res\Key.ico
# End Source File
# Begin Source File

SOURCE=.\newkey.rc
# End Source File
# Begin Source File

SOURCE=.\res\OK.ico
# End Source File
# Begin Source File

SOURCE=.\res\Openfold.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PC.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pen.ico
# End Source File
# Begin Source File

SOURCE=.\res\v.ico
# End Source File
# Begin Source File

SOURCE=.\res\v2.ico
# End Source File
# Begin Source File

SOURCE=".\res\viet-off.ico"
# End Source File
# Begin Source File

SOURCE=".\res\viet-on.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=.\gpl.txt
# End Source File
# Begin Source File

SOURCE=..\Release\vnconv.lib

!IF  "$(CFG)" == "NewKey - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Release\UKHook35.lib

!IF  "$(CFG)" == "NewKey - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Release\byteio.lib

!IF  "$(CFG)" == "NewKey - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Release\rtfio.lib

!IF  "$(CFG)" == "NewKey - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "NewKey - Win32 Unicode Release"

!ENDIF 

# End Source File
# End Target
# End Project
