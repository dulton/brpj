// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_)
#define AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
	PTZ_CONTROL_AUTO = 0,						// 自动
	PTZ_CONTROL_UP,								//上
	PTZ_CONTROL_DOWN,							// 下
	PTZ_CONTROL_LEFT,							// 左
	PTZ_CONTROL_RIGHT,							// 右
	PTZ_CONTROL_UPLEFT,							// 左上
	PTZ_CONTROL_UPRIGHT,						// 右上
	PTZ_CONTROL_DOWNLEFT,						// 左下
	PTZ_CONTROL_DOWNRIGHT,						// 右下
	PTZ_CONTROL_FOCUS_ADD,						// 调焦+
	PTZ_CONTROL_FOCUS_SUB,						// 调焦-
	PTZ_CONTROL_IRIS_ADD,						// 光圈+
	PTZ_CONTROL_IRIS_SUB,						// 光圈-
	PTZ_CONTROL_ZOOM_ADD,						// 变倍+
	PTZ_CONTROL_ZOOM_SUB,						// 变倍+
	PTZ_CONTROL_POINT_SET,						// 预设点设置
	PTZ_CONTROL_POINT_MOVE,						// 跳转到预设点
	PTZ_CONTROL_SPEED_ADD,						// 云台速度+
	PTZ_CONTROL_SPEED_SUB,						// 云台速度-
} PTZ_Command;

//摄像机厂商定义
#define CAMERA_INC_DAHUA 1
#define CAMERA_INC_HAIKANG 2
#define CAMERA_INC_YAAN 3

//当前摄像头厂家
#define ALLTAB_CAMERA_INC_TYPE CAMERA_INC_HAIKANG

//汽车模式 1 电动车模式 0
#define ALLTAB_DETECT_CAR_MODE 1

//开启识别代码=1 关闭=0  关闭就可以不用KEY也可以DEBUG
#define OPEN_CARDETECT_CODE 0

//结果输出到平台 模式 =1  单机版=0 
#define YRVM_PINGTAI_MODE	0

//弹出框登录 1 不弹出窗登录0
#define ALLTAB_LOGIN_WIN_MODE 1

//弹出框题目
#define MESSAGEBOX_TITLE "亿榕车牌识别系统"

//最多增加区域
#define MAX_AREA 1024

//最大显示多少个摄像头窗口
#define MAX_DEVICE_NUM 16


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_)
