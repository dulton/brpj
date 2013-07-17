#if !defined(AFX_DLGVSTRANSUSER_H__0EF0D7B9_6387_4FDB_9E32_76269DB3192E__INCLUDED_)
#define AFX_DLGVSTRANSUSER_H__0EF0D7B9_6387_4FDB_9E32_76269DB3192E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVSTransUser.h : header file
///////////////////////////////////////////////////////////////////////////////
// CDlgVSTransUser dialog

class CDlgVSTransUser : public CDialog
{
// Construction
public:
	CDlgVSTransUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVSTransUser)
	enum { IDD = IDD_DIG_VSTRANS };
	CListCtrl	m_List_VSTransUser;
	//}}AFX_DATA

	void UpdateListLoop(void) ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVSTransUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVSTransUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnTransdel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVSTRANSUSER_H__0EF0D7B9_6387_4FDB_9E32_76269DB3192E__INCLUDED_)