#if !defined(AFX_DLGUSERIP_H__4E9A3CFA_F894_4AD7_8151_F66122960021__INCLUDED_)
#define AFX_DLGUSERIP_H__4E9A3CFA_F894_4AD7_8151_F66122960021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUserIP dialog

class CDlgUserIP : public CDialog
{
// Construction
public:
	char m_sername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
public:
	CDlgUserIP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUserIP)
	enum { IDD = IDD_DLG_USERIPSHOW };
	CListCtrl	m_ListCam;
	int		m_usernums;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUserIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUserIP)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeChann();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CONNECTIP m_connectip;
	DWORD     m_channs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERIP_H__4E9A3CFA_F894_4AD7_8151_F66122960021__INCLUDED_)
