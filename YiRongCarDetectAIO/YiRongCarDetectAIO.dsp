# Microsoft Developer Studio Project File - Name="YiRongCarDetectAIO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=YiRongCarDetectAIO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "YiRongCarDetectAIO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "YiRongCarDetectAIO.mak" CFG="YiRongCarDetectAIO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "YiRongCarDetectAIO - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "YiRongCarDetectAIO - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "YiRongCarDetectAIO - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "YiRongCarDetectAIO - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "TIXML_USE_STL" /D "_CRT_SECURE_NO_WARNINGS" /D "TUNE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "YiRongCarDetectAIO - Win32 Release"
# Name "YiRongCarDetectAIO - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BSWndContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\CarDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\DahuaSDK.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGAddArea.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGAddDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGAnalyseFlowrate.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGBlackInOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGdevicetree.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGHistoryReport.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGHistoryVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGHistroyDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGnormal.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGOpenClose.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGptz.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetBlack.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetCar.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetElecar.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetPTZ.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGSetUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGshortcut.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGWarnning.cpp
# End Source File
# Begin Source File

SOURCE=.\ExButton.cpp
# End Source File
# Begin Source File

SOURCE=.\HaiKangSDK.cpp
# End Source File
# Begin Source File

SOURCE=.\IO.CPP
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\myCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenPannel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\xmlrw.cpp
# End Source File
# Begin Source File

SOURCE=.\YaAnSDK.cpp
# End Source File
# Begin Source File

SOURCE=.\YiRongCarDetectAIO.cpp
# End Source File
# Begin Source File

SOURCE=.\YiRongCarDetectAIO.rc
# End Source File
# Begin Source File

SOURCE=.\YiRongCarDetectAIODlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BSWndContainer.h
# End Source File
# Begin Source File

SOURCE=.\CarDetect.h
# End Source File
# Begin Source File

SOURCE=.\DahuaSDK.h
# End Source File
# Begin Source File

SOURCE=.\DLGAddArea.h
# End Source File
# Begin Source File

SOURCE=.\DLGAddDevice.h
# End Source File
# Begin Source File

SOURCE=.\DLGAnalyseFlowrate.h
# End Source File
# Begin Source File

SOURCE=.\DLGBlackInOut.h
# End Source File
# Begin Source File

SOURCE=.\DLGdevicetree.h
# End Source File
# Begin Source File

SOURCE=.\DLGHistoryReport.h
# End Source File
# Begin Source File

SOURCE=.\DLGHistoryVideo.h
# End Source File
# Begin Source File

SOURCE=.\DLGHistroyDetect.h
# End Source File
# Begin Source File

SOURCE=.\DLGLogin.h
# End Source File
# Begin Source File

SOURCE=.\DLGnormal.h
# End Source File
# Begin Source File

SOURCE=.\DLGOpenClose.h
# End Source File
# Begin Source File

SOURCE=.\DLGptz.h
# End Source File
# Begin Source File

SOURCE=.\DLGscreen.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetBlack.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetCar.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetElecar.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetPTZ.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetRecord.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetSystem.h
# End Source File
# Begin Source File

SOURCE=.\DLGSetUser.h
# End Source File
# Begin Source File

SOURCE=.\DLGshortcut.h
# End Source File
# Begin Source File

SOURCE=.\DLGWarnning.h
# End Source File
# Begin Source File

SOURCE=.\ExButton.h
# End Source File
# Begin Source File

SOURCE=.\HaiKangSDK.h
# End Source File
# Begin Source File

SOURCE=.\IO.H
# End Source File
# Begin Source File

SOURCE=.\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\myCamera.h
# End Source File
# Begin Source File

SOURCE=.\PlayWnd.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenPannel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\xmlrw.h
# End Source File
# Begin Source File

SOURCE=.\YaAnSDK.h
# End Source File
# Begin Source File

SOURCE=.\YiRongCarDetectAIO.h
# End Source File
# Begin Source File

SOURCE=.\YiRongCarDetectAIODlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\alarm_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\alarm_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancel_in.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancel_out.bmp
# End Source File
# Begin Source File

SOURCE=.\res\capbmp.BMP
# End Source File
# Begin Source File

SOURCE=.\res\capbmp_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\cleanalarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cleanalarm_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cleanalarm_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lock_background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login_background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logout_background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mode1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode16.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode16_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode16_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode1_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode1_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode4.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode4_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode4_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode9.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode9_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode9_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\mode_car.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mode_car_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mode_car_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\modify_background.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_close_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_closeall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_closeall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_ope.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_open_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_openall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_alarm_openall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_black_set.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_black_set_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_capbmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_capbmp_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_carset.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_carset_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_channel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_channel_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_alarm_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_recognize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_recognize_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_video.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_history_video_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_close_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_closeall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_closeall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_open_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_openall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_preview_openall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_close_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_closeall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_closeall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_open_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_openall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_recognize_openall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_close_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_closeall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_closeall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_open_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_openall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normal_record_openall_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\normalback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ok_in.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ok_out.bmp
# End Source File
# Begin Source File

SOURCE=.\res\preview.BMP
# End Source File
# Begin Source File

SOURCE=.\res\preview_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\preview_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ptzauto.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzauto_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzauto_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzdown_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzdown_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocusadd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocusadd_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocusadd_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocussub.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocussub_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzfocussub_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzgo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzgo_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzgo_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirisadd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirisadd_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirisadd_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirissub.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirissub_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzirissub_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleft_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleft_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftdown_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftdown_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftup_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzleftup_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzright.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzright_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzright_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightdown_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightdown_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightup_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzrightup_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzset.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzset_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzset_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedadd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedadd_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedadd_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedsub.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedsub_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzspeedsub_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzup_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzup_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomadd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomadd_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomadd_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomsub.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomsub_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ptzzoomsub_push.bmp
# End Source File
# Begin Source File

SOURCE=.\res\record.BMP
# End Source File
# Begin Source File

SOURCE=.\res\record_dis.BMP
# End Source File
# Begin Source File

SOURCE=.\res\record_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\screenback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set_car.BMP
# End Source File
# Begin Source File

SOURCE=.\res\set_car_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\set_car_push.BMP
# End Source File
# Begin Source File

SOURCE=.\res\setptz_back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shortcutback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_noraml_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_ptz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_ptz_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tab_tree_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\YiRongCarDetectAIO.ico
# End Source File
# Begin Source File

SOURCE=.\res\YiRongCarDetectAIO.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
