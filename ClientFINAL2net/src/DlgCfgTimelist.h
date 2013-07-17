#if !defined(AFX_DLGCFGTIMELIST_H__8BEFEDCB_48CE_43B4_BB1A_2E0D790310B0__INCLUDED_)
#define AFX_DLGCFGTIMELIST_H__8BEFEDCB_48CE_43B4_BB1A_2E0D790310B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCfgTimelist.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCfgTimelist dialog

class CDlgCfgTimelist : public CDialog
{
// Construction
public:
	TIMELIST *m_timelist;
	int  m_bIsReclist;
	BYTE m_brec;
	BYTE m_output[ALARM_OUT_NUM];
	CDlgCfgTimelist(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCfgTimelist)
	enum { IDD = IDD_DLGCFG_TIMELIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCfgTimelist)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCfgTimelist)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCFGTIMELIST_H__8BEFEDCB_48CE_43B4_BB1A_2E0D790310B0__INCLUDED_)
