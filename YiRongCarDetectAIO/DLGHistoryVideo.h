#if !defined(AFX_DLGHISTORYVIDEO_H__D1F53712_7E82_49A7_9092_ED0DD671B05F__INCLUDED_)
#define AFX_DLGHISTORYVIDEO_H__D1F53712_7E82_49A7_9092_ED0DD671B05F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGHistoryVideo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGHistoryVideo dialog
#include <vector>
#include <list>
using namespace::std;

#define HISTORY_VIDEO_PAGE_MAX_NUM 50

//命名太长会出错
struct HISTORY_VIDEO_ST
{
	//起始时间
	int start_year;
	int start_mon;
	int start_day;
	int start_hour;
	int start_min;
	int start_sec;
	//结束时间
	int end_year;
	int end_mon;
	int end_day;
	int end_hour;
	int end_min;
	int end_sec;

	char name[260];	//设备名称
	char ip[32];	//IP地址
	unsigned long int filesize;	//文件大小
	//文件路径
	char path[260];
};


class CDLGHistoryVideo : public CDialog
{
// Construction
public:
	CDLGHistoryVideo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGHistoryVideo)
	enum { IDD = IDD_HISTORY_VIDEO };
	CListCtrl	m_list;
	int		m_page;
	CString	m_name;
	CString	m_ip;
	CTime	m_StartMon;
	CTime	m_StartHour;
	CTime	m_EndMon;
	CTime	m_EndHour;
	BOOL	m_CheckTime;
	//}}AFX_DATA


	list<struct HISTORY_VIDEO_ST> list_history_video;

	int ListTotal;
	int ListNow;
	int ListChoose;
	void DisplayerList(void);

	//用来存放SQL语句的函数
	char SqlStr[1024];

	BOOL OnInitDialog();
	//屏蔽操作
	void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGHistoryVideo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGHistoryVideo)
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTORYVIDEO_H__D1F53712_7E82_49A7_9092_ED0DD671B05F__INCLUDED_)
