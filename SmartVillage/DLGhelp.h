#if !defined(AFX_DLGHELP_H__D1F12E3B_373B_43C3_AF86_749129314550__INCLUDED_)
#define AFX_DLGHELP_H__D1F12E3B_373B_43C3_AF86_749129314550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGhelp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGhelp dialog

class CDLGhelp : public CDialog
{
// Construction
public:
	CDLGhelp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGhelp)
	enum { IDD = IDD_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

BOOL OnInitDialog() ;
virtual void OnOK();
virtual void OnCancel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGhelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGhelp)
	
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonUpdate();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHELP_H__D1F12E3B_373B_43C3_AF86_749129314550__INCLUDED_)
