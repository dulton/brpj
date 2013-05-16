#if !defined(AFX_DLGSCREEN_H__08A4A59F_824B_475D_B94C_9FA7A3CFCE9D__INCLUDED_)
#define AFX_DLGSCREEN_H__08A4A59F_824B_475D_B94C_9FA7A3CFCE9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGscreen.h : header file
//
#include "ScreenPannel.h"

#if ALLTAB_DETECT_CAR_MODE
#include "DLGSetCar.h"
#else
#include "DLGSetElecar.h"
#endif

#include "CarDetect.h"

/*
#if (ALLTAB_CAMERA_INC_TYPE == CAMERA_INC_DAHUA)
#include "DahuaSDK.h"
#else if(ALLTAB_CAMERA_INC_TYPE == CAMERA_INC_HAIKANG)
#include "HaiKangSDK.h"
#endif
*/
#include "myCamera.h"

/*
//0---海康,1---大华
enum DEVICE_VENDERID_ENUM
{
	VENDER_TYPE_HAIKANG=0, 
	VENDER_TYPE_DAHUA=1, 
	VENDER_TYPE_YAAN=2, 
};
*/
//设备信息
struct DEVICE_INFO 
{
	long int camID;
	CString area;				//区域
	CString name;				//摄像机名称
	CString ip;
	CString user;
	CString psw;
	long port;
	bool isplay;
	bool isRecord;
	int subtype;
	bool enableDetect;
	long playHandle;
	bool enableAlarm;
	int  venderID;				//0---海康,1---大华
	CString recordPath;
	CTime startTime;
};


/////////////////////////////////////////////////////////////////////////////
// CDLGscreen dialog

class CDLGscreen : public CDialog
{
// Construction
public:
	CDLGscreen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGscreen)
	enum { IDD = IDD_SCREEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	BOOL	OnInitDialog();
	void AutoSize();

	//屏蔽操作
	void OnOK();
	void OnCancel();

	//播放窗口
	CScreenPannel m_screenPannel;

	struct DEVICE_INFO m_videoInfo[MAX_DEVICE_NUM];
	//车牌识别
//	struct CarSetStruct CarSet[MAX_DEVICE_NUM];

#if OPEN_CARDETECT_CODE 
	CCarDetect	CarDetect[MAX_DEVICE_NUM];
#endif

/*
#if (ALLTAB_CAMERA_INC_TYPE == CAMERA_INC_DAHUA)
	CDahuaSDK     m_video;
#else if(ALLTAB_CAMERA_INC_TYPE == CAMERA_INC_HAIKANG)
	CHaikangSDK   m_video;
#endif
*/
	CMyCamera   m_video;
	UINT  m_recordtimer;

	//当前显示画面序号
	int m_curScreen;

	//设置选择的窗口
	void SetCurWindId(int nCuWinID);
	//获取当前选择的窗口
	int GetCurWindId(void);
	//获取当前窗口的摄像机播放状态
	bool GetCurWindPlayState(int nCuWinID);
	//获取当前窗口的摄像机录像状态
	bool GetCurWindRecordState(int nCuWinID);
	//获取当前窗口的播放句柄
	long GetCurWindPlayHandle(int nCuWinID);
	//获取当前窗口的摄像机的码流类型
	int GetCurWindSubType(int nCuWinID);
	//获取当前选择窗口的摄像机详细信息
	void GetCurWindCamInfo(int nCuWinID,struct DEVICE_INFO &Info);
	//开启/关闭车牌识别
	void EnableDetect(int nCuWinID,bool bEnable);
	//开启/关闭报警
	void EnableAlarm(int nCuWinID,bool bEnable);
	//开启/关闭录像
	void EnableRecord(int nCuWinID,bool bEnable);
	//获取识别状态
	bool GetDetectState(int nCuWinID);
	//获取报警状态
	bool GetAlarmState(int nCuWinID);
	//获取录像状态
	bool GetRecordState(int nCuWinID);
	//车牌识别设置
	void CarDetectSet(void);

	//删除设备
	void DeleteDevice(CString sip);

	void SwitchMultiWnd(int nSplit);
	bool StartPlay(int id,char *area,char *name,char *ip,int port,
		char *user,char *psw,int subtype,int screenNo,int venderID);
	void StopPlay(int screenNo);
	void Capture(char *filename);

	//开始录像
	int StartRecord(int screenNo,char *filename);
	//停止录像
	void StopRecord(int screenNo);
	//录像定时时间
	void RecordTimerEvent();

	//云台控制
	void PtzControl(int type, BOOL dwStop, int param);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGscreen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGscreen)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCREEN_H__08A4A59F_824B_475D_B94C_9FA7A3CFCE9D__INCLUDED_)
