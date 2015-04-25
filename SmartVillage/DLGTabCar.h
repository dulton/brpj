#pragma once
#include "DLGTabPic.h"

#include <vector>
#include <list>
using namespace::std;

#include "YRSVMySQL.h"
#include "afxwin.h"

#define TAB_CAR_FLAG_CAR 0
#define TAB_CAR_FLAG_CARALARM 1

#define TAB_CAR_PAGE_MAX_NUM 40


// CDLGTabCar dialog

class CDLGTabCar : public CDialog
{
	DECLARE_DYNAMIC(CDLGTabCar)

public:
	CDLGTabCar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGTabCar();

	CListCtrl	m_List;
	CString	m_ip;
	CString	m_name;
	long		m_page;
	CString	m_plate;
	COleDateTime	m_StartMon;
	COleDateTime	m_StartHour;
	COleDateTime	m_EndMon;
	COleDateTime	m_EndHour;
	BOOL	m_CheckTime;
	int		m_platetype;
	int		m_platecolor;
	int		m_direction;
	int		m_carcolor;
	int m_lib;
	//}}AFX_DATA

	CRect printf_Rect;

	//是否显示缩略图界面
	bool PicdisplayFlag;

	list<struct HISTORY_CarDetect_ST> list_history_CarDetect;

	int searchFlag;
	long ListTotal;
	long ListNow;
	void DisplayerList(void);

	//用来存放SQL语句的函数
	char SqlStr[1024];


	CDLGTabPic DlgTabPic;

	BOOL	OnInitDialog();
	void AutoSize();
// Dialog Data
	enum { IDD = IDD_TAB_CAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonJump();
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCheckTime();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBitmapButton m_search;
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;

	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
