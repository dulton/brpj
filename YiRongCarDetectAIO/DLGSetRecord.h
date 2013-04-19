#if !defined(AFX_DLGSETRECORD_H__6D570753_FDCB_415A_91FE_6C516D3E0062__INCLUDED_)
#define AFX_DLGSETRECORD_H__6D570753_FDCB_415A_91FE_6C516D3E0062__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetRecord.h : header file
//
#include <vector>
#include <list>
using namespace::std;

//命名太长会出错
struct SET_RECORD_DATA_ST
{
	//数据库的NID
	unsigned long int nid;
};
/////////////////////////////////////////////////////////////////////////////
// CDLGSetRecord dialog

class CDLGSetRecord : public CDialog
{
// Construction
public:
	CDLGSetRecord(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSetRecord)
	enum { IDD = IDD_SET_RECORD };
	CComboBox	m_cam;
	CComboBox	m_area;
	CListCtrl	m_list;
	CTime	m_StartMon;
	CTime	m_StartHour;
	CTime	m_EndMon;
	CTime	m_EndHour;
	BOOL	m_all;
	BOOL	m_date;
	BOOL	m_enable;
	BOOL	m_everyday;
	BOOL	m_week;
	int		m_endweek;
	int		m_startweek;
	//}}AFX_DATA

	BOOL OnInitDialog();
	//屏蔽操作
	void OnOK();

	list<struct SET_RECORD_DATA_ST> list_record;
	struct SET_RECORD_DATA_ST data;

	int ListTotal;
	int ListNow;
	int ListChoose;

	void DisplayerList(void);
	void Clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetRecord)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetRecord)
	afx_msg void OnButtonEnable();
	afx_msg void OnButtonDisable();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAll();
	afx_msg void OnButtonDeleteall();
	afx_msg void OnCloseupComboArea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETRECORD_H__6D570753_FDCB_415A_91FE_6C516D3E0062__INCLUDED_)
