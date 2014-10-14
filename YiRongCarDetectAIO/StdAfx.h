// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_)
#define AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	////改菜单背景色要的定义 放到STDAFX.H
	//#ifndef WINVER
	//#define WINVER 0x0501 
	//#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxdisp.h>        // MFC Automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT



// 云台控制命令
typedef enum _PTZ_Command
{
	PTZ_CONTROL_UP = 0,							//上
	PTZ_CONTROL_DOWN = 1,								// 下
	PTZ_CONTROL_LEFT = 2,								// 左
	PTZ_CONTROL_RIGHT = 3,								// 右
	PTZ_CONTROL_ZOOM_ADD = 4,							// 变倍+
	PTZ_CONTROL_ZOOM_SUB = 5,						// 变倍-
	PTZ_CONTROL_FOCUS_ADD = 6,							// 调焦+
	PTZ_CONTROL_FOCUS_SUB = 7,						// 调焦-
	PTZ_CONTROL_IRIS_ADD = 8,							// 光圈+
	PTZ_CONTROL_IRIS_SUB = 9,							// 光圈-
	PTZ_CONTROL_POINT_MOVE = 10,						// 跳转到预设点
	PTZ_CONTROL_POINT_SET = 11,						// 预设点设置
	PTZ_CONTROL_UPLEFT = 12,							// 左上
	PTZ_CONTROL_UPRIGHT = 13,							// 右上
	PTZ_CONTROL_DOWNLEFT = 14,							// 左下
	PTZ_CONTROL_DOWNRIGHT = 15,							// 右下
	PTZ_CONTROL_SPEED_ADD = 16,						// 云台速度+
	PTZ_CONTROL_SPEED_SUB = 17,							// 云台速度-
	PTZ_CONTROL_AUTO = 18,							// 自动
} PTZ_Command;

//0---海康,1---大华
enum DEVICE_VENDERID_ENUM
{
	VENDER_TYPE_HAIKANG=0, 
	VENDER_TYPE_DAHUA=1, 
	VENDER_TYPE_YAAN=2, 
	VENDER_TYPE_YAAN_NEW=3, 
	VENDER_TYPE_STREAM=4, 
};

enum DEVICE_DECODETAG_ENUM
{
	DECODETAG_PS=0, //HAIKANG
	DECODETAG_SRC=1, //DAHUA
};


//摄像头开启
#define OPEN_YAAN_NEW_SDK 0
#define OPEN_YAAN_SDK 0
#define OPEN_DAHUA_SDK 1
#define OPEN_HAIKANG_SDK 1

#define DB33_PLAYCTRL
#define OPEN_STREAM_CLIENT_SDK 1

//当 SystemTransform.dll 冲突 使用StreamClientSDK的覆盖HAIKANSDK的



//工程 使用ushow2的库
/*************************************
//发布指南，目前分5种

//汽车单机版
ALLTAB_DETECT_CAR_MODE=1
YRVM_PINGTAI_MODE=0

//汽车联YRVM平台版
ALLTAB_DETECT_CAR_MODE=1
YRVM_PINGTAI_MODE=1

//电动车单机版
ALLTAB_DETECT_CAR_MODE=0
YRVM_PINGTAI_MODE=0

//电动联YRVM平台 写入电动车表 版
ALLTAB_DETECT_CAR_MODE=0
YRVM_PINGTAI_MODE=1
YRVM_PINGTAI_ELECAR_MIX_MODE=0

//电动联YRVM平台 写入机动车表 版
ALLTAB_DETECT_CAR_MODE=0
YRVM_PINGTAI_MODE=1
YRVM_PINGTAI_ELECAR_MIX_MODE=1

*************************************/

//客户端模式 请设置 车牌识别代码为 OPEN_CARDETECT_CODE 0
#define ALLTAB_CLIENT_MODE 0
//汽车模式 1 电动车模式 0
//切换电动车和汽车。
//A.记得换到IDD主窗口里换LOGO。 电动车用 IDB_LOGO_ELECAR 和汽车用 IDB_LOGO
//B.在rc文件里搜索IDR_MAINFRAME的路径 修改路径 IDR_MAINFRAME           ICON    DISCARDABLE     "res\\YiRongCarDetectAIO3.ico"
//C.到RES文件夹里 将 YiRongCarDetectAIO.ico 文件用对应的图标替换掉
#define ALLTAB_DETECT_CAR_MODE 0

//开启车牌识别代码=1 关闭=0  关闭就可以不用KEY也可以DEBUG
#define OPEN_CARDETECT_CODE 0

//海康 市局设备树
#define IVMS_ORACLE_DEVICETREE 1

//海康 卡口系统 需要启动设备树
#define IVMS_KAKOU_SOAP 1


//弹出框登录 1 不弹出窗登录0 //预留 请务必为1
//免登陆已经不用这个了 
//改为	DlgLogin.SilentMode
#define ALLTAB_LOGIN_WIN_MODE 1

//开启POCO-FTP功能 需要2008编译
#define OPEN_VS2008_POCO_FTP	0

//TOMCAT 模式
#define OPEN_TOMCAT_MODE 1


//服务+客户端 模式
#define OPEN_CS_MODE 1

//////////////////////////////////////////////////////

//结果输出到YRVM平台 电动车写入机动车表 =1  电动车写入电动车表=0 
#define YRVM_PINGTAI_ELECAR_MIX_MODE	0

//开启人脸识别代码=1 关闭=0  关闭就可以不用KEY也可以DEBUG
#define OPEN_FACEDETECT_CODE 0

//结果输出到YRVM平台 模式 =1  单机版=0 
#define YRVM_PINGTAI_MODE	0
///////////////////////////
//修改名称 记得把数据库配置程序的名称也改了
//以及 关于 处的字符
// 搜索 智能视频车牌分析服务软件 未加其他标志的
/*
#if OPEN_CARDETECT_CODE
	#if ALLTAB_DETECT_CAR_MODE
		//弹出框题目
		#define MESSAGEBOX_TITLE "智能视频机动车车牌分析服务软件 2.0版"
	#else
		//弹出框题目
		#define MESSAGEBOX_TITLE "智能视频电动车车牌分析服务软件 2.0版"
	#endif
#else
	#define MESSAGEBOX_TITLE "智能视频车牌分析服务软件 2.0版"
#endif
*/

#if OPEN_CARDETECT_CODE
	#if ALLTAB_DETECT_CAR_MODE
		//弹出框题目
		#define MESSAGEBOX_TITLE "福建亿榕机动车车牌识别分析应用软件 2.0版"
	#else
		//弹出框题目
		#define MESSAGEBOX_TITLE "福建亿榕电动车车牌识别分析应用软件 2.0版"
	#endif
#else
	#define MESSAGEBOX_TITLE "福建亿榕车牌识别分析应用软件 2.0版"
#endif

//最多增加区域
#define MAX_AREA 2048

//最大显示多少个摄像头窗口
#define MAX_DEVICE_NUM 16

//亚安的RTSP地址 rtsp://10.142.50.249:554/0/888888:888888/main

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_)
