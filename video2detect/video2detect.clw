; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVideo2detectDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "video2detect.h"

ClassCount=3
Class1=CVideo2detectApp
Class2=CVideo2detectDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VIDEO2DETECT_DIALOG

[CLS:CVideo2detectApp]
Type=0
HeaderFile=video2detect.h
ImplementationFile=video2detect.cpp
Filter=N

[CLS:CVideo2detectDlg]
Type=0
HeaderFile=video2detectDlg.h
ImplementationFile=video2detectDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST

[CLS:CAboutDlg]
Type=0
HeaderFile=video2detectDlg.h
ImplementationFile=video2detectDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VIDEO2DETECT_DIALOG]
Type=1
Class=CVideo2detectDlg
ControlCount=11
Control1=IDC_STATIC_PLAYWIN,static,1342177287
Control2=IDC_EDIT_FILE,edit,1484849280
Control3=IDC_PROGRESS_FILE,msctls_progress32,1350565889
Control4=IDC_BUTTON_FILE,button,1342242816
Control5=IDC_BUTTON_DETECT,button,1342242816
Control6=IDC_BUTTON_STOP,button,1342242816
Control7=IDC_STATIC_RATE,static,1342308352
Control8=IDC_LIST,SysListView32,1350631437
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT_IP,edit,1350631552

