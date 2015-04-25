#if !defined(AFX_DLGHISTORYREPORT_H__841DE495_4E89_4AB1_9603_9011992A0FAE__INCLUDED_)
#define AFX_DLGHISTORYREPORT_H__841DE495_4E89_4AB1_9603_9011992A0FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDLGHistoryReport.h : header file
//
#include "YRSVMySQL.h"
#include <vector>
#include <list>
using namespace::std;



#define HISTORY_REPORT_PAGE_MAX_NUM 40

/*
//命名太长会出错
struct HISTORY_REPORT_ST
{
	//起始时间
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	char user[260];	//用户名
	char str[1024];	//系统信息
};
*/

/////////////////////////////////////////////////////////////////////////////
// DLGHistoryReport dialog

class CDLGHistoryReport : public CDialog
{
// Construction
public:
	CDLGHistoryReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGHistoryReport)
	enum { IDD = IDD_HISTORY_REPORT };
		// NOTE: the ClassWizard will add data members here
	CListCtrl	m_List;
	long		m_page;
	CString	m_user;
	COleDateTime	m_StartMon;
	COleDateTime	m_StartHour;
	COleDateTime	m_EndMon;
	COleDateTime	m_EndHour;
	BOOL	m_CheckTime;
	//}}AFX_DATA
CRect printf_Rect;

	list<struct HISTORY_REPORT_ST> list_history_report;

	int searchFlag;
	long ListTotal;
	long ListNow;
	void DisplayerList(void);

	BOOL OnInitDialog();
	//屏蔽操作
	void OnOK();
	char sqlstr[1024];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGHistoryReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGHistoryReport)
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	afx_msg void OnCheckTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CBitmapButton m_search;
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTORYREPORT_H__841DE495_4E89_4AB1_9603_9011992A0FAE__INCLUDED_)
