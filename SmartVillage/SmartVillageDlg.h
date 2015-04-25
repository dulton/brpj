// SmartVillageDlg.h : header file
//

#if !defined(AFX_SmartVillageDLG_H__A09C780B_7883_4399_B0B2_38E203A2550D__INCLUDED_)
#define AFX_SmartVillageDLG_H__A09C780B_7883_4399_B0B2_38E203A2550D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DLGshortcut.h"
#include "DLGscreen.h"

#include "DLGdevicetree.h"
#include "DLGnormal.h"

#include "DLGSetRecord.h"

#include "DLGTab.h"
#include "DLGTabVideo.h"
#include "DLGTabFace.h"
#include "DLGTabCar.h"

#include "FaceRecg.h"
#include "FaceMng.h"
//WAV声音
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib") 


/////////////////////////////////////////////////////////////////////////////
// CSmartVillageDlg dialog

class CSmartVillageDlg : public CDialog
{
// Construction
public:
	CSmartVillageDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSmartVillageDlg)
	enum { IDD = IDD_SMARTVILLAGE_DIALOG };

	CDLGTabCar DlgTabCar;
	CDLGTabFace DlgTabFace;
	CDLGTabVideo DlgTabVideo;
	CDLGTab DlgTab;


	CRect		m_clientRect;		//程序界面区域位置

	//切换位置
	void UpdatePannelPosition();

	//屏蔽操作
	void OnOK();
	void OnCancel();

	//显示错误消息
	//windowflag =1 弹窗警告
	//windowflag =0 打印消息
	void ShowRuntimeMessage(char *msgStr,int windowflag);
	void NewLogMessage(char *msgStr);
	//摄像机消息提示
	void ShowCameraMessage(char *camName,char *msgStr,int windowflag);

	afx_msg void OnMenuitemHistoryVideo();
	afx_msg void OnMenuitemHistoryCar();
	afx_msg void OnMenuitemHistoryCarAlarm();
	afx_msg void OnMenuitemSetBlack();
	afx_msg void OnMenuitemHelp();

void FaceInit();

	void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);

#if OPEN_FACEDETECT_CODE
	CFaceRecg CFaceAlarm;
	CFaceMng FaceMng;
#endif
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartVillageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSmartVillageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuitemLoginExit();
	afx_msg void OnMenuitemLoginIn();
	afx_msg void OnMenuitemLoginLock();
	afx_msg void OnMenuitemCapbmp();
	afx_msg void OnMenuitem1mode();
	afx_msg void OnUpdateMenuitem1mode(CCmdUI* pCmdUI);
	afx_msg void OnMenuitem4mode();
	afx_msg void OnUpdateMenuitem4mode(CCmdUI* pCmdUI);
	afx_msg void OnMenuitem9mode();
	afx_msg void OnUpdateMenuitem9mode(CCmdUI* pCmdUI);
	afx_msg void OnMenuitem16mode();
	afx_msg void OnUpdateMenuitem16mode(CCmdUI* pCmdUI);



	afx_msg void OnMenuitemAbout();

	afx_msg void OnMenuitemSystemReport();
	afx_msg void OnMenuitemBlackIn();
	afx_msg void OnMenuitemBlackOut();
	afx_msg void OnMenuitemSetUser();
	afx_msg void OnMenuitemSetSystem();
	afx_msg void OnMenuitemSetSystemIn();
	afx_msg void OnMenuitemSetSystemOut();
	afx_msg void OnMenuitemSetRecord();
	afx_msg void OnMenuitemSetCar();
	afx_msg void OnMenuitemSetPtz();
	afx_msg void OnMenuitemCarstream();
	afx_msg void OnMenuitemLoginModify();

	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonTree();
	afx_msg void OnButtonNormal();
	afx_msg void OnButtonPtz();
	afx_msg void OnMenuitemSetrecord();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMenuitemSetFaceBlack();
	afx_msg void OnMenuitemHistoryFace();
	afx_msg void OnMenuitemHistoryFacealarm();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SmartVillageDLG_H__A09C780B_7883_4399_B0B2_38E203A2550D__INCLUDED_)
