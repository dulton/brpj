#if !defined(AFX_DLGHISTROYDETECT_H__7115D541_6815_41B6_899D_6C702EE804C3__INCLUDED_)
#define AFX_DLGHISTROYDETECT_H__7115D541_6815_41B6_899D_6C702EE804C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGHistroyDetect.h : header file
//
// CDLGHistoryVideo dialog
#include <vector>
#include <list>
using namespace::std;

#include "IO.h"

#define HISTORY_DETECT_FLAG_CAR 1
#define HISTORY_DETECT_FLAG_ALARM 2

#define HISTORY_DETECT_PAGE_MAX_NUM 40

/*
//命名太长会出错
struct HISTORY_DETECT_ST
{
	//nid
	unsigned long int nid;
	char area[260];	//区域名称
	char name[260];	//摄像头名称
	char ip[32];	//IP地址

	//时间
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;

	//车牌类型
	char	platetype[32];
	//车牌颜色
	char	platecolor[32];
	//车牌方向
	char	direction[32];
	//车身颜色
	char	carcolor[32];
	//车牌号
	char	plate[32];
	//置信度
	int reliability;
	
	//文件路径
	char path[260];
	//图像大小
	unsigned long int picsize;
};
*/
/////////////////////////////////////////////////////////////////////////////
// CDLGHistroyDetect dialog

class CDLGHistroyDetect : public CDialog
{
// Construction
public:
	CDLGHistroyDetect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGHistroyDetect)
	enum { IDD = IDD_HISTORY_DETECT };
	CListCtrl	m_list;
	CString	m_ip;
	CString	m_name;
	int		m_page;
	CString	m_plate;
	CTime	m_StartMon;
	CTime	m_StartHour;
	CTime	m_EndMon;
	CTime	m_EndHour;
	BOOL	m_CheckTime;
	int		m_platetype;
	int		m_platecolor;
	int		m_direction;
	int		m_carcolor;
	//}}AFX_DATA

	int flag;

	list<struct HISTORY_DETECT_ST> list_history_detect;


	int searchFlag;
	int ListTotal;
	int ListNow;
	void DisplayerList(void);

	//用来存放SQL语句的函数
	char SqlStr[1024];

	BOOL OnInitDialog();
	//屏蔽操作
	void OnOK();
	void DisplayNetPic(int iItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGHistroyDetect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGHistroyDetect)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTROYDETECT_H__7115D541_6815_41B6_899D_6C702EE804C3__INCLUDED_)
