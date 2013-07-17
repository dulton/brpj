#if !defined(AFX_DLGSERVERCFG_H__AEAF8833_1FAB_4086_B077_DFC30380047A__INCLUDED_)
#define AFX_DLGSERVERCFG_H__AEAF8833_1FAB_4086_B077_DFC30380047A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgServerCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgServerCfg dialog

class CDlgServerCfg : public CDialog
{
// Construction
public:
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
public:
	CDlgServerCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgServerCfg)
	enum { IDD = IDD_DIG_SERVERCFG };
	CString	m_sername;
	CString	m_serip;
	CString	m_submask;
	CString	m_dns;
	CString	m_getway;
	CString	m_multiip;
	int		m_serport;
	int		m_multiport;
	int		m_webport;
	CString	m_wldz;
	CString	m_version;
	BOOL	m_nndsenable;
	int		m_ispal;
	int		m_launage;
	CString	m_adminname;
	CString	m_adminpass;
	CString	m_ddnsip;
	int		m_ddnsport;
	int		m_ddnsmapport;
	CString	m_hipass;
	CString	m_hiusername;
	CString	m_opername;
	CString	m_operpass;
	UINT	m_alarmport;
	CString	m_alarmurl;
	UINT	m_recordport;
	CString	m_recordurl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgServerCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgServerCfg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnSettime();
	afx_msg void OnBtnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERVERCFG_H__AEAF8833_1FAB_4086_B077_DFC30380047A__INCLUDED_)
