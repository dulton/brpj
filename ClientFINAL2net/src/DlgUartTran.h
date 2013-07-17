#if !defined(AFX_DLGUARTTRAN_H__BA33E11E_FE6C_4548_A82C_67D966E0C607__INCLUDED_)
#define AFX_DLGUARTTRAN_H__BA33E11E_FE6C_4548_A82C_67D966E0C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUartTran.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUartTran dialog

class CDlgUartTran : public CDialog
{
// Construction
public:
	CDlgUartTran(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUartTran)
	enum { IDD = IDD_DLG_UARTTRAN };
	int		m_port;
	CString	m_password;
	CString	m_recvstring;
	CString	m_url;
	CString	m_username;
	//}}AFX_DATA
	BOOL	m_bHexView;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUartTran)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUartTran)
	afx_msg void OnBtnSerial();
	afx_msg void OnBtnSend();
	afx_msg void OnBtnCleanrevedit();
	afx_msg void OnCheckHexView();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT OnRecvMsg(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUARTTRAN_H__BA33E11E_FE6C_4548_A82C_67D966E0C607__INCLUDED_)
