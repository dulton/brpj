#pragma once
#include "afxcmn.h"

#include <list>

using namespace::std;

/*
struct HISTORY_STRUCT
{
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
	char ip[64];
	char carNumber[32];
	char path[260];
};
*/

// CHistoryDLG dialog

class CHistoryDLG : public CDialog
{
	DECLARE_DYNAMIC(CHistoryDLG)

public:
	CHistoryDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryDLG();

	int Flag;//1为全部 0为报警

	list<struct HISTORY_STRUCT> listhistoryAll;
	int ListTotal;
	int ListNow;

	int listchoice;

// Dialog Data
	enum { IDD = IDD_DIALOG_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonFirst();
	CListCtrl m_List;
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL OnInitDialog();
	void OnOK();
	void DisplayerList(void);
	afx_msg void OnBnClickedButtonCarnumber();
	afx_msg void OnBnClickedButtonTime();
	CString m_carnumber;
	CTime m_TimeStartMon;
	CTime m_TimeStartHour;
	CTime m_TimeEndMon;
	CTime m_TimeEndHour;
	afx_msg void OnBnClickedButtonSms();
	CString m_smsphone;
};
