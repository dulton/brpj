#if !defined(AFX_DLGANALYSEFLOWRATE_H__2B53EE5C_048A_452F_9CEA_4A009C70C58F__INCLUDED_)
#define AFX_DLGANALYSEFLOWRATE_H__2B53EE5C_048A_452F_9CEA_4A009C70C58F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGAnalyseFlowrate.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CDLGAnalyseFlowrate dialog

class CDLGAnalyseFlowrate : public CDialog
{
// Construction
public:
	CDLGAnalyseFlowrate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGAnalyseFlowrate)
	enum { IDD = IDD_ANALYSE_FLOWRATE };
	CListCtrlEx	m_List;
	CString	m_ip;
	COleDateTime 	m_StartMon;
	COleDateTime 	m_StartHour;
	COleDateTime 	m_EndMon;
	COleDateTime 	m_EndHour;
	COleDateTime 	m_Day;
	//}}AFX_DATA

	BOOL OnInitDialog();
	unsigned long int GetCount(unsigned long int camid,char *stime,char*etime);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGAnalyseFlowrate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGAnalyseFlowrate)
	virtual void OnOK();
	afx_msg void OnButtonSearchAll();
	afx_msg void OnButtonSearchSingle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALYSEFLOWRATE_H__2B53EE5C_048A_452F_9CEA_4A009C70C58F__INCLUDED_)
