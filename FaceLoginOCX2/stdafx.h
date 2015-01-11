#pragma once

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define POINTER_64 __ptr64

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Comon Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#ifndef _WIN64

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#endif // _WIN64
#include <afxwin.h>

//#define USE_CHTTP
//超时宏定义 秒
#define OCX_TIMEOUT_MAX				120

//人脸注册、识别OCX返回值
#define OCX_ERROR_NO_ERROR			0		//识别或注册成功
#define OCX_ERROR_NO_CAMERA			1		//未检测到USB摄像头
#define OCX_ERROR_ENROLL_FAIL		2		//注册失败
#define OCX_ERROR_MATCH_FAIL		2		//识别失败
#define OCX_ERROR_UNALLOW_USER		3		//非法用户
#define OCX_ERROR_TIME_OUT			4		//超时
#define OCX_ERROR_FACESERVER_DOWN	5		//服务器异常
#define OCX_ERROR_USER_CANCEL		6		//用户取消
#define	OCX_ERROR_VERSION_ERROR		7		//OCX版本不对
#define OCX_ERROR_UNREGIST_USER		8		//当前用户未注册人脸  //旧版该号码为8
#define	OCX_ERROR_LIVEFACE_INIT_FAIL		9		//活体检测初始化失败
#define	OCX_ERROR_LIVEFACE_WIDGET_FAIL		10		//活体检测WIDGET初始化失败
#define	OCX_ERROR_LIVEFACE_FAIL		11		//活体检测判断为非活体
#define	OCX_ERROR_NO_PERSION_ID		12		//注册人脸出现FACEID 为空。一般是没PERSION ID引起的


#define CURENT_OCX_VER	"v1.1.1"			//当前程序版本号
#define OCX_LOG_PATH	"C:\\Users\\FaceLoginOCX\\FLlog.dll"
//开启和关闭活体检测 以及设置的路径
#define LIVE_FACE_TEST  1
#define MD5_OPEN		0					//V1.1.0取消识别密码加密
#define LIVE_FACE_MODEL_PATH "C:/Users/FaceLoginModel/"


//新皮肤对应的颜色
#define COMBO_COLOR_OLD (RGB(0,41,41)) 
#define COMBO_COLOR_A (RGB(95,157,153))
//替换完新皮肤。这里要改成 COMBO_COLOR_A
#define SKIN_COMBO_COLOR COMBO_COLOR_OLD


