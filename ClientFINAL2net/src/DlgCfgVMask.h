#if !defined(AFX_DLGCFGVMASK_H__2D987E38_F8E3_4042_9ADD_71A3B9DCAAEA__INCLUDED_)
#define AFX_DLGCFGVMASK_H__2D987E38_F8E3_4042_9ADD_71A3B9DCAAEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgVMask.h : header file
//
#include "MotionArea.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgCfgVMask dialog

class CDlgCfgVMask : public CDialog
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
	CHANNVIDEOMASK *pvmask;
	CDlgCfgVMask(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgVMask)
	enum { IDD = IDD_DLGCFG_VMASK };
	CMotionArea	m_videownd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgVMask)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CHANNVIDEOMASK m_vmask;
	// Generated message map functions
	//{{AFX_MSG(CDlgCfgVMask)
	afx_msg void OnBtnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGVMASK_H__2D987E38_F8E3_4042_9ADD_71A3B9DCAAEA__INCLUDED_)
