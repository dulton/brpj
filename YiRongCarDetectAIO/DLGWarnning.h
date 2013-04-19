#if !defined(AFX_DLGWARNNING_H__04EE7106_36F8_4FAE_A8CB_07C7DBA1CC61__INCLUDED_)
#define AFX_DLGWARNNING_H__04EE7106_36F8_4FAE_A8CB_07C7DBA1CC61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGWarnning.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGWarnning dialog

class CDLGWarnning : public CDialog
{
// Construction
public:
	CDLGWarnning(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGWarnning)
	enum { IDD = IDD_WARNNING };
	CString	m_warntxt;

	//}}AFX_DATA
	CString	m_wintxt;
	BOOL OnInitDialog();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGWarnning)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGWarnning)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWARNNING_H__04EE7106_36F8_4FAE_A8CB_07C7DBA1CC61__INCLUDED_)
