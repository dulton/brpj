#if !defined(AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_)
#define AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGPlaywin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGPlaywin dialog

class CDLGPlaywin : public CDialog
{
// Construction
public:
	CDLGPlaywin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGPlaywin)
	enum { IDD = IDD_PLAYWIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BOOL OnInitDialog();
	void AutoSize();

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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYWIN_H__37926D5A_99AC_43DD_914E_2A1540FD7890__INCLUDED_)
