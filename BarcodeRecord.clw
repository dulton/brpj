; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDLGVideoList
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "BarcodeRecord.h"

ClassCount=10
Class1=CBarcodeRecordApp
Class2=CBarcodeRecordDlg
Class3=CAboutDlg

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_BARCODERECORD_DIALOG
Resource4=IDD_VIDEOLIST
Resource5=IDD_PRODUCT_INFO
Resource6=IDD_CONTROL
Class4=CDLGProductList
Resource7=IDD_SETTINGS
Class5=CDLGVideoList
Class6=CDLGControl
Class7=CDLGPlaywin
Class8=CDLGProductInfo
Resource8=IDD_NEWDEVICE
Class9=CDLGSettings
Class10=CDLGNewDevice
Resource9=IDD_PLAYWIN

[CLS:CBarcodeRecordApp]
Type=0
HeaderFile=BarcodeRecord.h
ImplementationFile=BarcodeRecord.cpp
Filter=N

[CLS:CBarcodeRecordDlg]
Type=0
HeaderFile=BarcodeRecordDlg.h
ImplementationFile=BarcodeRecordDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TREE

[CLS:CAboutDlg]
Type=0
HeaderFile=BarcodeRecordDlg.h
ImplementationFile=BarcodeRecordDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_BARCODERECORD_DIALOG]
Type=1
Class=CBarcodeRecordDlg
ControlCount=2
Control1=IDC_TREE,SysTreeView32,1350631431
Control2=IDC_STATIC_TITLE,static,1342308352

[DLG:IDD_PRODUCT_INFO]
Type=1
Class=CDLGProductInfo
ControlCount=30
Control1=IDC_EDIT_BARCODE,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BUTTON_OK,button,1342242816
Control4=IDC_STATIC,static,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC_PIC1,static,1342177287
Control7=IDC_STATIC_PIC3,static,1342177287
Control8=IDC_STATIC_PIC2,static,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC_RUNNUMBER,static,1342308864
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC_TAG,static,1342308864
Control22=IDC_STATIC_MAINC,static,1342308864
Control23=IDC_STATIC_SUBC,static,1342308864
Control24=IDC_STATIC_METAFIELD,static,1342308864
Control25=IDC_STATIC_COLORDESC,static,1342308864
Control26=IDC_STATIC_COLOR,static,1342308864
Control27=IDC_STATIC_UNIT,static,1342308864
Control28=IDC_STATIC_FACTORYITEM,static,1342308864
Control29=IDC_STATIC_HMNUM,static,1342308864
Control30=IDC_STATIC_DESC,static,1342308352

[DLG:IDD_PLAYWIN]
Type=1
Class=CDLGPlaywin
ControlCount=1
Control1=IDC_STATIC_INFO,static,1342308352

[DLG:IDD_CONTROL]
Type=1
Class=CDLGControl
ControlCount=6
Control1=IDC_BUTTON_RECORD,button,1342242816
Control2=IDC_BUTTON_PAUSE,button,1342242816
Control3=IDC_BUTTON_STOP,button,1342242816
Control4=IDC_BUTTON_SHOTFRAME,button,1342242816
Control5=IDC_BUTTON_SETTINGS,button,1342242816
Control6=IDC_BUTTON_VIDEOLIST,button,1342242816

[CLS:CDLGProductList]
Type=0
HeaderFile=DLGProductList.h
ImplementationFile=DLGProductList.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_PAGE
VirtualFilter=dWC

[DLG:IDD_VIDEOLIST]
Type=1
Class=CDLGVideoList
ControlCount=24
Control1=IDC_CHECK_TIME,button,1342242819
Control2=IDC_DATETIMEPICKER_STARTMON,SysDateTimePick32,1342242852
Control3=IDC_DATETIMEPICKER_ENDMON,SysDateTimePick32,1342242852
Control4=IDC_DATETIMEPICKER_STARTHOUR,SysDateTimePick32,1342242857
Control5=IDC_DATETIMEPICKER_ENDHOUR,SysDateTimePick32,1342242857
Control6=IDC_EDIT_RUNNUM,edit,1350631552
Control7=IDC_EDIT_HMNUM,edit,1350631552
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_BUTTON_PREVIOUS,button,1342242816
Control11=IDC_BUTTON_FIRST,button,1342242816
Control12=IDC_BUTTON_NEXT,button,1342242816
Control13=IDC_BUTTON_LAST,button,1342242816
Control14=IDC_STATIC_INFO,static,1342308352
Control15=IDC_EDIT_PAGE,edit,1350639744
Control16=IDC_BUTTON_JUMP,button,1342242816
Control17=IDC_BUTTON_SEARCH,button,1342242816
Control18=IDC_BUTTON_PLAY,button,1342242816
Control19=IDC_BUTTON_DELETE,button,1342242816
Control20=IDC_LIST,SysListView32,1350631453
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_DESC,edit,1350631552
Control24=IDC_STATIC,static,1342308864

[CLS:CDLGVideoList]
Type=0
HeaderFile=DLGVideoList.h
ImplementationFile=DLGVideoList.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST
VirtualFilter=dWC

[CLS:CDLGControl]
Type=0
HeaderFile=DLGControl.h
ImplementationFile=DLGControl.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGControl
VirtualFilter=dWC

[CLS:CDLGPlaywin]
Type=0
HeaderFile=DLGPlaywin.h
ImplementationFile=DLGPlaywin.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGPlaywin
VirtualFilter=dWC

[CLS:CDLGProductInfo]
Type=0
HeaderFile=DLGProductInfo.h
ImplementationFile=DLGProductInfo.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGProductInfo
VirtualFilter=dWC

[DLG:IDD_SETTINGS]
Type=1
Class=CDLGSettings
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_SHOTFRAME,edit,1484849280
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BUTTON_SHOTFRAME,button,1342242816
Control6=IDC_EDIT_RECORD,edit,1484849280
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUTTON_RECORD,button,1342242816
Control9=IDC_COMBO_RECORD,combobox,1344339971
Control10=IDC_STATIC,static,1342308352
Control11=IDC_COMBO_SHOTFRAME,combobox,1344339971
Control12=IDC_STATIC,static,1342308352

[CLS:CDLGSettings]
Type=0
HeaderFile=DLGSettings.h
ImplementationFile=DLGSettings.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO_RECORD
VirtualFilter=dWC

[DLG:IDD_NEWDEVICE]
Type=1
Class=CDLGNewDevice
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_NAME,edit,1350631552
Control4=IDC_EDIT_IP,edit,1350631552
Control5=IDC_EDIT_PORT,edit,1350639744
Control6=IDC_EDIT_PWD,edit,1350631584
Control7=IDC_EDIT_USER,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:CDLGNewDevice]
Type=0
HeaderFile=DLGNewDevice.h
ImplementationFile=DLGNewDevice.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGNewDevice
VirtualFilter=dWC

