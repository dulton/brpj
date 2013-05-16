#if !defined(AFX_DLGWARNNING_H__9B22F62A_65E8_4154_88A2_5016EBB8718E__INCLUDED_)
#define AFX_DLGWARNNING_H__9B22F62A_65E8_4154_88A2_5016EBB8718E__INCLUDED_

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
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWARNNING_H__9B22F62A_65E8_4154_88A2_5016EBB8718E__INCLUDED_)
