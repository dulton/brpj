#if !defined(AFX_PLAYWND_H__7F3FC9E5_7C46_44F3_A700_447A968EF338__INCLUDED_)
#define AFX_PLAYWND_H__7F3FC9E5_7C46_44F3_A700_447A968EF338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayWnd.h : header file
//

/////////////////////////
//	POPUP MENU ID DEFINE

#define		VIDEO_MENU_BASE				WM_USER + 1979
#define		VIDEO_MENU_END				WM_USER + 1994
#define		VIDEO_MENU_FULLSCREEN		WM_USER + 1979
#define		VIDEO_MENU_MULTISCREEN		WM_USER + 1980
#define		VIDEO_MENU_AUTOADJUST		WM_USER + 1981
#define		VIDEO_MENU_RECORDVIDEO		WM_USER + 1982
#define		VIDEO_MENU_PRINTSCREEN		WM_USER + 1983
#define		VIDEO_MENU_EXITDECODE		WM_USER + 1984
#define		VIDEO_MENU_EXITCYCLE		WM_USER + 1985
#define		VIDEO_MENU_STARTPLAY		WM_USER + 1986
#define		VIDEO_MENU_STOPPLAY			WM_USER + 1987
#define		VIDEO_MENU_STARTDETECT		WM_USER + 1988
#define		VIDEO_MENU_STOPDETECT		WM_USER + 1989
#define		VIDEO_MENU_STARTRECORD		WM_USER + 1990
#define		VIDEO_MENU_STOPRECORD		WM_USER + 1991
#define		VIDEO_MENU_STARTALARM		WM_USER + 1992
#define		VIDEO_MENU_STOPALARM		WM_USER + 1993
#define		VIDEO_MENU_CAPTURE			WM_USER + 1994
#define		VIDEO_REPAINT				WM_USER + 1999

/////////////////////////
//	POPUP MENU NAME DEFINE
#define NAME_MENU_FULLSCREEN				"全屏显示"
#define NAME_MENU_MULTISCREEN				"多屏显示"
#define NAME_MENU_AUTOADJUST				"自动调整"
#define NAME_MENU_STARTPLAY					"启动预览"
#define NAME_MENU_STOPPLAY					"关闭预览"
#define NAME_MENU_STARTDETECT				"启动识别"
#define NAME_MENU_STOPDETECT				"关闭识别"
#define NAME_MENU_STARTRECORD				"启动录像"
#define NAME_MENU_STOPRECORD				"关闭录像"
#define NAME_MENU_STARTALARM				"启用报警"
#define NAME_MENU_STOPALARM					"关闭报警"
#define NAME_MENU_CAPTURE					"抓拍图像"

//	KeyColor

//#define		VIDEO_BACK_COLOR	RGB(111,104,160)
#define		VIDEO_BACK_COLOR	RGB(100,100,160)

/////////////////////////////////////////////////////////////////////////////
// CPlayWnd dialog

class CPlayWnd : public CWnd
{
// Construction
public:
	CPlayWnd();   // standard constructor


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	afx_msg void OnVideoMenu(UINT nID);
	afx_msg LRESULT OnRepaintWnd(WPARAM wParam, LPARAM lParam);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetWinID(int ID) {m_nWndID = ID;}
	int  GetWinID(void){return m_nWndID;}
	
private:
	CRITICAL_SECTION m_cs;
	BOOL m_bFullScreen;
	int m_nWndID;

	int m_nPreSplit;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYWND_H__7F3FC9E5_7C46_44F3_A700_447A968EF338__INCLUDED_)
