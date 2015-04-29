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

#define ZOG_MAX_PATH_STR 260

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
	VENDER_TYPE_STREAM=2, 
	VENDER_TYPE_VLC_RTSP=3, 
};

enum DEVICE_DECODETAG_ENUM
{
	DECODETAG_PS=0, //HAIKANG
	DECODETAG_SRC=1, //DAHUA
};


//摄像头开启
//-----------------------------------
//大华的SDK只能开启其中一个
#define OPEN_DAHUA_SDK 0
#define OPEN_DAHUA_SDK_NEW 1
//-----------------------------------
#define OPEN_HAIKANG_SDK 1

#define DB33_PLAYCTRL
#define OPEN_STREAM_CLIENT_SDK 1

#define OPEN_VLC_RTSP_SDK 1

///////
#define VIDEO_FORMAT_YV12 1
#define VIDEO_FORMAT_I420 2
#define VIDEO_FORMAT_RGB888 3
#define VIDEO_FORMAT_BGR888 4
//当 SystemTransform.dll 冲突 使用StreamClientSDK的覆盖HAIKANSDK的
////////////////////////////////////////////////////////////
//测试模式 
#define TEST_DEBUG 1
//测试 死循环版本 。停止时会卡死。
#define DEAD_WHILE 0
//发布版 程序启动 自动开启播放=1
#define AUTO_OPENALL (!TEST_DEBUG)


//必须汽车模式 该版本不支持电动车
//汽车模式 1 电动车模式 0
//切换电动车和汽车。
//A.记得换到IDD主窗口里换LOGO。 电动车用 IDB_LOGO_ELECAR 和汽车用 IDB_LOGO
//B.在rc文件里搜索IDR_MAINFRAME的路径 修改路径 IDR_MAINFRAME           ICON    DISCARDABLE     "res\\YiRongCarDetectAIO3.ico"
//C.到RES文件夹里 将 YiRongCarDetectAIO.ico 文件用对应的图标替换掉
#define ALLTAB_DETECT_CAR_MODE 1

//-----------------------------------
//车牌识别的SDK只能开启其中一个
//开启郎驰车牌识别代码=1 关闭=0  关闭就可以不用KEY也可以DEBUG
#define OPEN_LC_CARDETECT_CODE 1

//开启火眼臻睛车牌识别代码=1 关闭=0  关闭就可以不用KEY也可以DEBUG
#define OPEN_HYZJ_CARDETECT_CODE 0
//-----------------------------------

//开启录像代码 1 关闭录像代码 0
#define OPEN_RECORD 0

//开启人脸识别代码
#define OPEN_FACEDETECT_CODE 0

//人脸识别 几帧检测一次
//设为2就是不跳了！不要设成0
#define FACE_JUMP_NUM 5
//设为2就是不跳了！不要设成0
#define CAR_JUMP_NUM 3
//开启上传到京奥服务的接口
#define JING_AO_UPLOAD 1
//////////////////////////////////////////////////////
//使用新库设为1 旧库为0
#if 1

//新库
#include "recolibC\\recoCommon.h"
#include "recolibC\\recoFaceMng.h"
#include "recolibC\\recoVideoAnalyse.h"

#if OPEN_FACEDETECT_CODE
//#pragma comment必须放头文件  不然CRT 未初始化 R6030
#pragma comment(lib, "recolibC\\recoCommon.lib")
#pragma comment(lib, "recolibC\\recoFaceMng.lib")
#pragma comment(lib, "recolibC\\recoVideoAnalyse.lib")

#endif

#else

//旧库
#include "recolibA\\recoCommon.h"
#include "recolibA\\recoFaceMng.h"
#include "recolibA\\recoVideoAnalyse.h"

#if OPEN_FACEDETECT_CODE
//#pragma comment必须放头文件  不然CRT 未初始化 R6030
#pragma comment(lib, "recolibA\\recoCommon.lib")
#pragma comment(lib, "recolibA\\recoFaceMng.lib")
#pragma comment(lib, "recolibA\\recoVideoAnalyse.lib")

#endif

#endif


typedef struct
{
	HTREEITEM item;						//本指针
	long nid;
	long ncamera;					//仅抓拍临时库有效
	long RWdb_nid;
}DBLIST;

///////////////////////////
//修改名称 记得把数据库配置程序的名称也改了
//以及 关于 处的字符
// 搜索 智能视频车牌分析服务软件 未加其他标志的

#define MESSAGEBOX_TITLE "福建亿榕小区出入口管理系统"


//最多增加区域+摄像头的数量
#define MAX_AREA_ADD_CAM 16
//最多增加区域
#define MAX_AREA 4

//最大显示多少个摄像头窗口
#define MAX_DEVICE_NUM 16
//最大播放路数 以及增加多少摄像头
#define MAX_CAMERA 4

//亚安的RTSP地址 rtsp://10.142.50.249:554/0/888888:888888/main

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DA1368DB_F92F_4AAE_86E3_70C4BF842BE0__INCLUDED_)
