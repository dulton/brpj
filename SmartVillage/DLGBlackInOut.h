#if !defined(AFX_DLGBLACKINOUT_H__B5396FE7_79DE_4A06_8531_BDE070024281__INCLUDED_)
#define AFX_DLGBLACKINOUT_H__B5396FE7_79DE_4A06_8531_BDE070024281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGBlackInOut.h : header file
//
#define BLACK_INOUT_INPUT	1
#define BLACK_INOUT_OUTPUT	2
/////////////////////////////////////////////////////////////////////////////
// CDLGBlackInOut dialog

class CDLGBlackInOut : public CDialog
{
// Construction
public:
	CDLGBlackInOut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGBlackInOut)
	enum { IDD = IDD_BLACK_INOUT };
	CProgressCtrl	m_progress;
	CString	m_info;
	//}}AFX_DATA

	int flag;
	char path[ZOG_MAX_PATH_STR];

	BOOL OnInitDialog();
	//屏蔽操作
	void OnOK();

	//线程句柄
	HANDLE pthread;
	//线程在用=false  线程未用=ture
	bool ThreadFlag;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGBlackInOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGBlackInOut)
	virtual void OnCancel();
	afx_msg void OnButtonStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLACKINOUT_H__B5396FE7_79DE_4A06_8531_BDE070024281__INCLUDED_)
