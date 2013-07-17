# Microsoft Developer Studio Project File - Name="LAUMp4Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LAUMp4Test - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LAUMp4Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LAUMp4Test.mak" CFG="LAUMp4Test - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LAUMp4Test - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "LAUMp4Test - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LAUMp4Test - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /I "../../../h/NetCommdef" /I "../LUMP4Net" /I "../NetHostSDk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0500 /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "../../../h/NetCommdef" /I "../LUMP4Net" /I "../NetHostSDk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0500 /D "_MBCS" /D "HTTP_ONLY" /D "BUILDING_LIBCURL" /FR /Yu"stdafx.h" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Ws2_32.lib /nologo /subsystem:windows /incremental:no /debug /machine:IX86 /out:"..\bin\LAUMp4Test.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Ws2_32.lib winmm.lib wldap32.lib libcurld_imp.lib PlayerSDK.lib /nologo /subsystem:windows /incremental:no /debug /machine:IX86 /out:"..\bin\LAUMp4Test.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "LAUMp4Test - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LAUMp4Test___Win32_Release"
# PROP BASE Intermediate_Dir "LAUMp4Test___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LAUMp4Test___Win32_Release"
# PROP Intermediate_Dir "LAUMp4Test___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /ZI /Od /I "../../../h/NetCommdef" /I "../LUMP4Net" /I "../NetHostSDk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _WIN32_WINNT=0x0500 /D "_MBCS" /D "HTTP_ONLY" /D "BUILDING_LIBCURL" /FR /Yu"stdafx.h" /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /I "../../../h/NetCommdef" /I "../LUMP4Net" /I "../NetHostSDk" /D "WIN32" /D "_WINDOWS" /D _WIN32_WINNT=0x0500 /D "_MBCS" /D "HTTP_ONLY" /D "BUILDING_LIBCURL" /FR /Yu"stdafx.h" /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Ws2_32.lib winmm.lib wldap32.lib libcurld_imp.lib /nologo /subsystem:windows /incremental:no /debug /machine:IX86 /out:"..\bin\LAUMp4Test.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 Ws2_32.lib winmm.lib wldap32.lib libcurld_imp.lib PlayerSDK.lib /nologo /subsystem:windows /incremental:no /debug /machine:IX86 /out:"..\bin\LAUMp4Test.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "LAUMp4Test - Win32 Debug"
# Name "LAUMp4Test - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AudioInput.cpp
DEP_CPP_AUDIO=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\AudioInput.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=DlgAudioTalk.cpp
DEP_CPP_DLGAU=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\AudioInput.h"\
	".\DlgAudioTalk.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgCfgAlarm.cpp
DEP_CPP_DLGCF=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgAlarm.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgCfgTimelist.cpp
DEP_CPP_DLGCFG=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgTimelist.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgCfgViparam.cpp
DEP_CPP_DLGCFGV=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgViparam.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgCfgVMask.cpp
DEP_CPP_DLGCFGVM=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgVMask.h"\
	".\LAUMp4Test.h"\
	".\MotionArea.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgChannelCfg.cpp
DEP_CPP_DLGCH=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgTimelist.h"\
	".\DlgCfgViparam.h"\
	".\DlgCfgVMask.h"\
	".\DlgChannelCfg.h"\
	".\DlgMotionArea.h"\
	".\LAUMp4Test.h"\
	".\MotionArea.h"\
	".\SetVideoWnd1.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\DLGChooseIP.cpp
DEP_CPP_DLGCHO=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DLGChooseIP.h"\
	".\DlgPlanRecord.h"\
	".\IOoracle.h"\
	".\LAUMp4Test.h"\
	".\LAUMp4TestDlg.h"\
	".\ShowWnd.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=DlgHostNotify.cpp
DEP_CPP_DLGHO=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgHostNotify.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgMotionArea.cpp
DEP_CPP_DLGMO=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgMotionArea.h"\
	".\LAUMp4Test.h"\
	".\SetVideoWnd1.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\DlgPlanRecord.cpp
DEP_CPP_DLGPL=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\curl\curl.h"\
	".\curl\curlbuild.h"\
	".\curl\curlrules.h"\
	".\curl\curlver.h"\
	".\curl\easy.h"\
	".\curl\multi.h"\
	".\curl\typecheck-gcc.h"\
	".\DlgPlanRecord.h"\
	".\IOoracle.h"\
	".\LAUMp4Test.h"\
	".\LAUMp4TestDlg.h"\
	".\lauplaym4.h"\
	".\ShowWnd.h"\
	".\SignalDownload.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=DlgSerialCfg.cpp
DEP_CPP_DLGSE=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgSerialCfg.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\DlgSerialParam.cpp
DEP_CPP_DLGSER=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgSerialParam.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=DlgServerCfg.cpp
DEP_CPP_DLGSERV=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgAlarm.h"\
	".\DlgChannelCfg.h"\
	".\DlgHostNotify.h"\
	".\DlgSerialCfg.h"\
	".\DlgServerCfg.h"\
	".\DlgSet.h"\
	".\DlgTimedReset.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgSet.cpp
DEP_CPP_DLGSET=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgCfgAlarm.h"\
	".\DlgChannelCfg.h"\
	".\DlgHostNotify.h"\
	".\DlgSerialCfg.h"\
	".\DlgServerCfg.h"\
	".\DlgSet.h"\
	".\DlgTimedReset.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgTimedReset.cpp
DEP_CPP_DLGTI=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgTimedReset.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgUartTran.cpp
DEP_CPP_DLGUA=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgSerialParam.h"\
	".\DlgUartTran.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgUpdate.cpp
DEP_CPP_DLGUP=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgUpdate.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgUserIP.cpp
DEP_CPP_DLGUS=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgUserIP.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=DlgVSTransUser.cpp
DEP_CPP_DLGVS=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\DlgVSTransUser.h"\
	".\LAUMp4Test.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\IOoracle.cpp
DEP_CPP_IOORA=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\IOoracle.h"\
	".\ReadConfigTxt.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=LAUMp4Test.cpp
DEP_CPP_LAUMP=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\LAUMp4Test.h"\
	".\LAUMp4TestDlg.h"\
	".\ShowWnd.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=LAUMp4Test.rc
# End Source File
# Begin Source File

SOURCE=LAUMp4TestDlg.cpp
DEP_CPP_LAUMP4=\
	"..\lib\launet.h"\
	"..\lib\NetHostLib.h"\
	"..\lib\VSNETStructDef.h"\
	".\AudioInput.h"\
	".\curl\curl.h"\
	".\curl\curlbuild.h"\
	".\curl\curlrules.h"\
	".\curl\curlver.h"\
	".\curl\easy.h"\
	".\curl\multi.h"\
	".\curl\typecheck-gcc.h"\
	".\DlgAudioTalk.h"\
	".\DlgCfgAlarm.h"\
	".\DlgChannelCfg.h"\
	".\DLGChooseIP.h"\
	".\DlgHostNotify.h"\
	".\DlgPlanRecord.h"\
	".\DlgSerialCfg.h"\
	".\DlgServerCfg.h"\
	".\DlgSet.h"\
	".\DlgTimedReset.h"\
	".\DlgUartTran.h"\
	".\DlgUpdate.h"\
	".\DlgUserIP.h"\
	".\DlgVSTransUser.h"\
	".\IOoracle.h"\
	".\LAUMp4Test.h"\
	".\LAUMp4TestDlg.h"\
	".\ShowWnd.h"\
	".\SignalDownload.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /MD /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=MotionArea.cpp
DEP_CPP_MOTIO=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\LAUMp4Test.h"\
	".\MotionArea.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\ReadConfigTxt.cpp
DEP_CPP_READC=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\ReadConfigTxt.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=SetVideoWnd1.cpp
DEP_CPP_SETVI=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\LAUMp4Test.h"\
	".\SetVideoWnd1.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=ShowWnd.cpp
DEP_CPP_SHOWW=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\LAUMp4Test.h"\
	".\ShowWnd.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yu"stdafx.h" /GZ
# End Source File
# Begin Source File

SOURCE=.\SignalDownload.cpp
DEP_CPP_SIGNA=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\curl\curl.h"\
	".\curl\curlbuild.h"\
	".\curl\curlrules.h"\
	".\curl\curlver.h"\
	".\curl\easy.h"\
	".\curl\multi.h"\
	".\curl\typecheck-gcc.h"\
	".\SignalDownload.h"\
	".\StdAfx.h"\
	
# End Source File
# Begin Source File

SOURCE=StdAfx.cpp
DEP_CPP_STDAF=\
	"..\lib\launet.h"\
	"..\lib\VSNETStructDef.h"\
	".\StdAfx.h"\
	
# ADD CPP /nologo /GX /Od /FR /Yc"stdafx.h" /GZ
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AudioInput.h
# End Source File
# Begin Source File

SOURCE=DlgAudioTalk.h
# End Source File
# Begin Source File

SOURCE=DlgCfgAlarm.h
# End Source File
# Begin Source File

SOURCE=DlgCfgTimelist.h
# End Source File
# Begin Source File

SOURCE=DlgCfgViparam.h
# End Source File
# Begin Source File

SOURCE=DlgCfgVMask.h
# End Source File
# Begin Source File

SOURCE=DlgChannelCfg.h
# End Source File
# Begin Source File

SOURCE=.\DLGChooseIP.h
# End Source File
# Begin Source File

SOURCE=DlgHostNotify.h
# End Source File
# Begin Source File

SOURCE=DlgMotionArea.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlanRecord.h
# End Source File
# Begin Source File

SOURCE=DlgSerialCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSerialParam.h
# End Source File
# Begin Source File

SOURCE=DlgServerCfg.h
# End Source File
# Begin Source File

SOURCE=DlgSet.h
# End Source File
# Begin Source File

SOURCE=DlgTimedReset.h
# End Source File
# Begin Source File

SOURCE=DlgUartTran.h
# End Source File
# Begin Source File

SOURCE=DlgUpdate.h
# End Source File
# Begin Source File

SOURCE=DlgUserIP.h
# End Source File
# Begin Source File

SOURCE=DlgVSTransUser.h
# End Source File
# Begin Source File

SOURCE=.\IOoracle.h
# End Source File
# Begin Source File

SOURCE=LAUMp4Test.h
# End Source File
# Begin Source File

SOURCE=LAUMp4TestDlg.h
# End Source File
# Begin Source File

SOURCE=MotionArea.h
# End Source File
# Begin Source File

SOURCE=.\ReadConfigTxt.h
# End Source File
# Begin Source File

SOURCE=Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetVideoWnd.h
# End Source File
# Begin Source File

SOURCE=SetVideoWnd1.h
# End Source File
# Begin Source File

SOURCE=ShowWnd.h
# End Source File
# Begin Source File

SOURCE=.\SignalDownload.h
# End Source File
# Begin Source File

SOURCE=StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=res\LAUMp4Test.ico
# End Source File
# Begin Source File

SOURCE=res\LAUMp4Test.rc2
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=../lib/launet.h
# End Source File
# Begin Source File

SOURCE=..\lib\NetHostLib.h
# End Source File
# Begin Source File

SOURCE=..\lib\NetClient.lib
# End Source File
# Begin Source File

SOURCE=..\lib\NetHostSDK.lib
# End Source File
# End Group
# Begin Source File

SOURCE=ReadMe.txt
# End Source File
# End Target
# End Project
# Section LAUMp4Test : {3BB373E3-11C7-4173-9603-321D46F3F8F4}
# 	2:21:DefaultSinkHeaderFile:luserparamx.h
# 	2:16:DefaultSinkClass:CLUSerParamX
# End Section
# Section LAUMp4Test : {9A8B902A-9EEF-4BE6-8183-31EE668A69DF}
# 	2:5:Class:CLUSerParamX
# 	2:10:HeaderFile:luserparamx.h
# 	2:8:ImplFile:luserparamx.cpp
# End Section
