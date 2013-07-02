#if !defined(AFX_DLGLOGIN_H__B8B95CE2_A468_4D0A_8718_925B8AA84E30__INCLUDED_)
#define AFX_DLGLOGIN_H__B8B95CE2_A468_4D0A_8718_925B8AA84E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGLogin.h : header file
//
//定义
#define LOGIN_EXIT	1
#define LOGIN_IN	2
#define LOGIN_LOCK	3
#define LOGIN_MODIFY	4

/////////////////////////////////////////////////////////////////////////////
// CDLGLogin dialog
#include "IO.h"

class CDLGLogin : public CDialog
{
// Construction
public:
	CDLGLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGLogin)
	enum { IDD = IDD_LOGIN };
	CBitmapButton	m_ok;
	CBitmapButton	m_cancel;
	CString	m_user;
	CString	m_password;
	CString	m_newpassword;
	CString	m_newpsw_again;
	//}}AFX_DATA
	//带参数静默模式
	BOOL SilentMode;
	//定义标志
	int flag;

	BOOL OnInitDialog();
	//当前成功登陆的用户
	struct SET_USER_DATA_ST CurrentUser;
	//解密的密码
	char CurrentPsw[260];
	
	BOOL OnOK_Silent(); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGLogin)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__B8B95CE2_A468_4D0A_8718_925B8AA84E30__INCLUDED_)
