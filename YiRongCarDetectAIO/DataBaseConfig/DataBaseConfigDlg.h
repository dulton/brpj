// DataBaseConfigDlg.h : header file
//

#if !defined(AFX_DATABASECONFIGDLG_H__589C9A2F_6967_4223_8AD1_1E2D99091C75__INCLUDED_)
#define AFX_DATABASECONFIGDLG_H__589C9A2F_6967_4223_8AD1_1E2D99091C75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDataBaseConfigDlg dialog

class CDataBaseConfigDlg : public CDialog
{
// Construction
public:
	CDataBaseConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDataBaseConfigDlg)
	enum { IDD = IDD_DATABASECONFIG_DIALOG };
	CBitmapButton	m_cancel;
	CBitmapButton	m_ok;
	CString	m_ipaddr;
	CString	m_name;
	CString	m_port;
	CString	m_psw;
	CString	m_user;
	CString	m_pswagain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataBaseConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDataBaseConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATABASECONFIGDLG_H__589C9A2F_6967_4223_8AD1_1E2D99091C75__INCLUDED_)
