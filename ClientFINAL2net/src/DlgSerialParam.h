#if !defined(AFX_DLGSERIALPARAM_H__B254E648_1894_4ACB_A121_97B4208B34AF__INCLUDED_)
#define AFX_DLGSERIALPARAM_H__B254E648_1894_4ACB_A121_97B4208B34AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSerialParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialParam dialog

class CDlgSerialParam : public CDialog
{
// Construction
public:
	CDlgSerialParam(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSerialParam)
	enum { IDD = IDD_DLG_SERIALPARAM };
	CString	m_baud;
	int		m_data;
	int		m_SerialPort;
	int		m_stop;
	int		m_very;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSerialParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSerialParam)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIALPARAM_H__B254E648_1894_4ACB_A121_97B4208B34AF__INCLUDED_)
