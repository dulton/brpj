#if !defined(AFX_DLGCHOOSEIP_H__A3891FF7_564F_4A12_822A_A8422C0271B8__INCLUDED_)
#define AFX_DLGCHOOSEIP_H__A3891FF7_564F_4A12_822A_A8422C0271B8__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGChooseIP.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDLGChooseIP dialog
#include "IOoracle.h"
#include "DlgPlanRecord.h"
#include "LAUMp4TestDlg.h"

typedef struct 
{
	HTREEITEM Parent_item; //父指针
	HTREEITEM item; //本指针
	CString policeID;
	long int camID;
	CString sername;//服务器名
	CString ip;
	CString user;
	CString psw;
	CString channel;
	CString port;
	BOOL view;
}IPLIST;

class CDLGChooseIP : public CDialog
{
	// Construction
public:
	CDLGChooseIP(CWnd* pParent = NULL);   // standard constructor

	void BuildTree(void);
	// Dialog Data
	//{{AFX_DATA(CDLGChooseIP)
	enum { IDD = IDD_DLG_CHOOSEIP };
	CListCtrl	m_listchoose;
	CTreeCtrl	m_tree;
	BOOL	m_view;
	//}}AFX_DATA
	IOoracle *poracle;
	
	int chooseip_total;
	CDlgPlanRecord *PlanRecorddlg;
	CDlgPlanRecord PlanModedlg;
	CLAUMp4TestDlg *testdlg;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGChooseIP)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	IPLIST iplist[1024];
	int iptotal;

	CString curIP;
	long int curCamID;
	CString curPID;

	CString curUser;
	CString curPsw;
	int curChannel;
	int curPort;

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGChooseIP)
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONSetPlanrecordS();
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONSetPlanrecordN();
	afx_msg void OnBUTTONPlanDayStart();
	afx_msg void OnBUTTONPlanWeekStart();
	afx_msg void OnCheckView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	void AlarmMassage(long int camID,CString ip,CString user,CString psw,
								CString channel,CString port,BOOL view,int winno);
	virtual BOOL OnInitDialog();
	void OnCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHOOSEIP_H__A3891FF7_564F_4A12_822A_A8422C0271B8__INCLUDED_)
