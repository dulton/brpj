#if !defined(AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_)
#define AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGNewDevice.h : header file
//
#include <vector>
#include <list>
using namespace::std;

#include "SqliteOperate.h"
/////////////////////////////////////////////////////////////////////////////
// CDLGNewDevice dialog

class CDLGNewDevice : public CDialog
{
// Construction
public:
	CDLGNewDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGNewDevice)
	enum { IDD = IDD_NEWDEVICE };
	CListCtrl	m_list;
	CString	m_ip;
	CString	m_name;
	UINT	m_port;
	CString	m_pwd;
	CString	m_user;
	CString	m_playwin;
	//}}AFX_DATA
	BOOL OnInitDialog();

	list<struct CAMERA_INFO_ST> cameraList;

	int ListChoose;

	//对外读取此 数组0-3 为对应窗口号
	struct CAMERA_INFO_ST device[MAX_PLAYWIN];

	void display(int nItem,int i);
	void read();
	void ChooseEnable();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGNewDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGNewDevice)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonModify();
	afx_msg void OnButtonClean();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWDEVICE_H__F2669FA9_9106_44E1_8A0B_4E1ACD3B1E26__INCLUDED_)
