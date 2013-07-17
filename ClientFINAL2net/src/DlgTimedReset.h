#if !defined(AFX_DLGTIMEDRESET_H__6AEFA168_A352_4A75_8825_6E65D07447FC__INCLUDED_)
#define AFX_DLGTIMEDRESET_H__6AEFA168_A352_4A75_8825_6E65D07447FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTimedReset.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgTimedReset dialog

class CDlgTimedReset : public CDialog
{
// Construction
public:
	CDlgTimedReset(CWnd* pParent = NULL);   // standard constructor

	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	int  m_chnums;
	int  m_curchannel;
// Dialog Data
	//{{AFX_DATA(CDlgTimedReset)
	enum { IDD = IDD_DIG_TIMED_RESET };
	int		m_intervaltime;
	int		m_resettime_hour;
	int		m_resettime_minutes;
	BOOL	m_timedreset_enable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTimedReset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTimedReset)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboChsel();
	afx_msg void OnBtnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIMEDRESET_H__6AEFA168_A352_4A75_8825_6E65D07447FC__INCLUDED_)
