#if !defined(AFX_DLGVIDEOLIST_H__88EBD002_D91D_4EDE_8C43_D6E48DB549AB__INCLUDED_)
#define AFX_DLGVIDEOLIST_H__88EBD002_D91D_4EDE_8C43_D6E48DB549AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGVideoList.h : header file
//

#include "SqliteOperate.h"

#include <vector>
#include <list>
using namespace::std;

#define VIDEO_INFO_MAX_NUM 40


/////////////////////////////////////////////////////////////////////////////
// CDLGVideoList dialog

class CDLGVideoList : public CDialog
{
// Construction
public:
	CDLGVideoList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGVideoList)
	enum { IDD = IDD_VIDEOLIST };
	CListCtrl	m_list;
	int		m_page;
	CTime	m_StartMon;
	CTime	m_StartHour;
	CTime	m_EndMon;
	CTime	m_EndHour;
	BOOL	m_CheckTime;
	CString	m_runnum;
	CString	m_desc;
	CString	m_hmnum;
	//}}AFX_DATA

	list<struct VIDEO_INFO_ST> list_video;

	int ListTotal;
	int ListNow;
	int ListChoose;

	void DisplayerList(void);

	//用来存放SQL语句的函数
	int searchFlag;
	char SqlStr[1024];

	BOOL OnInitDialog();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGVideoList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGVideoList)
	virtual void OnOK();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEOLIST_H__88EBD002_D91D_4EDE_8C43_D6E48DB549AB__INCLUDED_)
