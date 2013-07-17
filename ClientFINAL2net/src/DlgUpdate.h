#if !defined(AFX_DLGUPDATE_H__3F69DD2E_5746_4122_9269_7F9A9613758C__INCLUDED_)
#define AFX_DLGUPDATE_H__3F69DD2E_5746_4122_9269_7F9A9613758C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUpdate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdate dialog

class CDlgUpdate : public CDialog
{
// Construction
public:
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	CDlgUpdate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUpdate)
	enum { IDD = IDD_DLGUPDATE };
	CString	m_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUpdate)
	virtual void OnCancel();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnFile();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LONG   m_updatesock;
	HANDLE m_hEndEvent;
	BOOL   m_bUpdateNow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUPDATE_H__3F69DD2E_5746_4122_9269_7F9A9613758C__INCLUDED_)
