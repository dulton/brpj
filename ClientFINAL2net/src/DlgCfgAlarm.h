#if !defined(AFX_DLGCFGALARM_H__3173EFCF_9ECD_4287_BCA3_83A54FD0AB8F__INCLUDED_)
#define AFX_DLGCFGALARM_H__3173EFCF_9ECD_4287_BCA3_83A54FD0AB8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgAlarm dialog

class CDlgCfgAlarm : public CDialog
{
// Construction
public:
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	int m_chnums,m_alarminnums,m_outputnums;
public:
	CDlgCfgAlarm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgAlarm)
	enum { IDD = IDD_DLGCFG_ALARM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_cursenno,m_curptzno;
	// Generated message map functions
	//{{AFX_MSG(CDlgCfgAlarm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnSet();
	afx_msg void OnSelchangeComboChsel();
	afx_msg void OnSelchangeComboPtzchsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	ALARMPARAM m_alarmpara;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGALARM_H__3173EFCF_9ECD_4287_BCA3_83A54FD0AB8F__INCLUDED_)
