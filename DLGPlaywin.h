#if !defined(AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_)
#define AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGPlaywin.h : header file
//
#include "ScreenPannel.h"
#include "YaanCamera.h"

/////////////////////////////////////////////////////////////////////////////
// CDLGPlaywin dialog

class CDLGPlaywin : public CDialog
{
// Construction
public:
	CDLGPlaywin(CWnd* pParent = NULL);   // standard constructor
	//播放窗口
	CScreenPannel m_screenPannel;
	//当前选择画面序号
	int m_curScreen;
	CYaanCamera m_video;

// Dialog Data
	//{{AFX_DATA(CDLGPlaywin)
	enum { IDD = IDD_PLAYWIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BOOL OnInitDialog();
	void AutoSize();
	//设置选择的窗口
	void SetCurWindId(int nCuWinID);
	//获取当前选择的窗口
	int GetCurWindId(void);
	//获取当前选择的窗口的播放状态
	BOOL GetCurWndPlayState();
	//获取当前选择的窗口的录像状态
	BOOL GetCurWndRecordState();
	//开始播放
	bool StartPlay(int screenNo,char *name,char *ip,WORD port,char *user,char *psw);
	//停止播放
	void StopPlay();
	//抓拍图像
	void CapturePic();
	//开始录像
	bool StartRecord();
	//停止录像
	void StopRecord();
	//获取当前窗口的录像时间
	void GetCurWndRecordTime(char *rtime);
	//更新窗口的产品信息
	void UpdateWndProductInfo();
	//屏蔽操作
	void OnOK();
	void OnCancel();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGPlaywin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGPlaywin)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_)
