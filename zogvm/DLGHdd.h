#if !defined(AFX_DLGHDD_H__B4B03617_503F_48F6_9CF6_6F10012FC305__INCLUDED_)
#define AFX_DLGHDD_H__B4B03617_503F_48F6_9CF6_6F10012FC305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGHdd.h : header file
//
#include "SqliteOperate.h"
#include <vector>
#include <list>
using namespace::std;
/////////////////////////////////////////////////////////////////////////////
// CDLGHdd dialog

class CDLGHdd : public CDialog
{
// Construction
public:
	CDLGHdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGHdd)
	enum { IDD = IDD_HDD };
	CListCtrl	m_list;
	BOOL	m_check;
	//}}AFX_DATA

		list<struct HDD_ST> hddList;

DWORD CDLGHdd::GetHDDid(void);
static void CDLGHdd::Add27HDDid(void);
BOOL CDLGHdd::OnInitDialog();
// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGHdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGHdd)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHDD_H__B4B03617_503F_48F6_9CF6_6F10012FC305__INCLUDED_)
