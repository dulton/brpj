#if !defined(AFX_DLGCFGVIPARAM_H__7F5FD232_4E1E_4FE3_AFA7_5D5ABC13FD4E__INCLUDED_)
#define AFX_DLGCFGVIPARAM_H__7F5FD232_4E1E_4FE3_AFA7_5D5ABC13FD4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgViparam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgViparam dialog

class CDlgCfgViparam : public CDialog
{
// Construction
public:
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	char m_ch;
	LONG m_hchann;
	CHANNVIPARAM *m_pviparam;
	CDlgCfgViparam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgViparam)
	enum { IDD = IDD_DLGCFG_VIPARAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgViparam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCfgViparam)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGVIPARAM_H__7F5FD232_4E1E_4FE3_AFA7_5D5ABC13FD4E__INCLUDED_)
