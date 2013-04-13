#if !defined(AFX_DLGCONTROL_H__23A0A6E5_D465_4932_91F0_ED41E4BFC668__INCLUDED_)
#define AFX_DLGCONTROL_H__23A0A6E5_D465_4932_91F0_ED41E4BFC668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGControl dialog

class CDLGControl : public CDialog
{
// Construction
public:
	CDLGControl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGControl)
	enum { IDD = IDD_CONTROL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BOOL CDLGControl::OnInitDialog();
	void AutoSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGControl)
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonPause();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonShotframe();
	afx_msg void OnButtonSettings();
	afx_msg void OnButtonVideolist();
	afx_msg void OnButtonProductlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONTROL_H__23A0A6E5_D465_4932_91F0_ED41E4BFC668__INCLUDED_)
