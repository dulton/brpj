; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CZogvmDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "zogvm.h"

ClassCount=4
Class1=CZogvmApp
Class2=CZogvmDlg

ResourceCount=5
Resource2=IDD_HDD
Resource3=IDD_ZOGVM_DIALOG
Resource1=IDR_MAINFRAME
Class3=CDLGFileType
Resource4=IDD_FILETYPE
Class4=CDLGHdd
Resource5=IDR_MENU

[CLS:CZogvmApp]
Type=0
HeaderFile=zogvm.h
ImplementationFile=zogvm.cpp
Filter=N

[CLS:CZogvmDlg]
Type=0
HeaderFile=zogvmDlg.h
ImplementationFile=zogvmDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_MENUITEM_SET_HDD



[DLG:IDD_ZOGVM_DIALOG]
Type=1
Class=CZogvmDlg
ControlCount=4
Control1=IDC_EDIT_SREACH,edit,1350631552
Control2=IDC_BUTTON_FIND,button,1342242816
Control3=IDC_BUTTON_BUILD,button,1342242816
Control4=IDC_LIST,SysListView32,1350631437

[MNU:IDR_MENU]
Type=1
Class=CZogvmDlg
Command1=ID_MENUITEM_SET_TYPE
Command2=ID_MENUITEM_SET_HDD
CommandCount=2

[DLG:IDD_FILETYPE]
Type=1
Class=CDLGFileType
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK_VIDEO,button,1342242819
Control4=IDC_LIST_VIDEO,SysListView32,1350631437
Control5=IDC_CHECK_MUSIC,button,1342242819
Control6=IDC_LIST_MUSIC,SysListView32,1350631437
Control7=IDC_CHECK_SUB,button,1342242819
Control8=IDC_LIST_SUB,SysListView32,1350631437
Control9=IDC_CHECK_OTHER,button,1342242819
Control10=IDC_LIST_OTHER,SysListView32,1350631437

[CLS:CDLGFileType]
Type=0
HeaderFile=DLGFileType.h
ImplementationFile=DLGFileType.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGFileType
VirtualFilter=dWC

[DLG:IDD_HDD]
Type=1
Class=CDLGHdd
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CHECK,button,1342242819
Control4=IDC_LIST,SysListView32,1350631437

[CLS:CDLGHdd]
Type=0
HeaderFile=DLGHdd.h
ImplementationFile=DLGHdd.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST
VirtualFilter=dWC

