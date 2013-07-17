#if !defined(AFX_DLGSET_H__3E16DD60_2538_4A49_A510_C0E8D924A3B5__INCLUDED_)
#define AFX_DLGSET_H__3E16DD60_2538_4A49_A510_C0E8D924A3B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSet.h : header file
//
#include "DlgServerCfg.h"
#include "DlgChannelCfg.h"
#include "DlgSerialCfg.h"
#include "DlgCfgAlarm.h"
#include "DlgTimedReset.h"
#include "DlgHostNotify.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSet dialog

class CDlgSet : public CDialog
{
// Construction
public:
	char m_sername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
public:
	CDlgSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSet)
	enum { IDD = IDD_DLGSET };
	CTabCtrl	m_settab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnCok();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnSave();
	afx_msg void OnBtnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CDlgServerCfg  m_sercfg;
	CDlgChannelCfg m_chancfg;
	CDlgSerialCfg  m_serialcfg;
	CDlgCfgAlarm   m_cfgalarm;
	CDlgTimedReset m_timedreset;
	CDlgHostNotify m_hostnotify;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSET_H__3E16DD60_2538_4A49_A510_C0E8D924A3B5__INCLUDED_)
