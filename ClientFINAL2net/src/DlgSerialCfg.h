#if !defined(AFX_DLGSERIALCFG_H__5EB4A374_7FD8_42A7_B753_95D6B2BB1EF2__INCLUDED_)
#define AFX_DLGSERIALCFG_H__5EB4A374_7FD8_42A7_B753_95D6B2BB1EF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSerialCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialCfg dialog

class CDlgSerialCfg : public CDialog
{
// Construction
public:
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
public:
	CDlgSerialCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSerialCfg)
	enum { IDD = IDD_DLG_SEAIAL };
	BYTE	m_addr;
	int		m_bitrate;
	int		m_checkbit;
	int		m_databit;
	int		m_overflow;
	int		m_stopbit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSerialCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSerialCfg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeChannum();
	afx_msg void OnBtnFile();
	afx_msg void OnBtnPtzfileset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	SERIAL485_PARAM s_param485[MAX_CHANNEL];
	char m_lpptzcmdname[MAX_CHANNEL][40];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIALCFG_H__5EB4A374_7FD8_42A7_B753_95D6B2BB1EF2__INCLUDED_)
