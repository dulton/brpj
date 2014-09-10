#if !defined(AFX_DLGSETBLACK_H__0287AC76_30D4_46E3_A6CC_ADC0002FA4D4__INCLUDED_)
#define AFX_DLGSETBLACK_H__0287AC76_30D4_46E3_A6CC_ADC0002FA4D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetBlack.h : header file
//
#include <vector>
#include <list>
using namespace::std;

#include "IO.h"
#include "afxwin.h"

#define SET_BLACK_PAGE_MAX_NUM 35
/*
//命名太长会出错
struct BLACK_DATA_ST
{
	unsigned long int nid;
	char brand[256];	//品牌
	char name[256];	//失主姓名
	char Phone[256];	//失主电话
	char plate[256];	//车牌号
	char other[1024];	//其他信息

};
*/
/////////////////////////////////////////////////////////////////////////////
// CDLGSetBlack dialog

class CDLGSetBlack : public CDialog
{
// Construction
public:
	CDLGSetBlack(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSetBlack)
	enum { IDD = IDD_SET_BLACK };
	CListCtrl	m_List;
	CString	m_Brand;
	CString	m_Name;
	CString	m_Phone;
	CString	m_Plate;
	CString	m_Other;
	int		m_page;
	//}}AFX_DATA
	CRect printf_Rect;

	BOOL OnInitDialog();
	void OnOK();

	list<struct BLACK_DATA_ST> list_black;
	struct BLACK_DATA_ST data;

	long ListTotal;
	long ListNow;
	long ListChoose;

	void DisplayerList(void);
	void reflush();
	void Clear();

	void Struct2M(void);
	void M2Struct(void);

	 void BlackInput();
	 void BlackOuput();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetBlack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetBlack)
	afx_msg void OnButtonJump();
	afx_msg void OnButtonFirst();
	afx_msg void OnButtonPrevious();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonLast();
	afx_msg void OnButtonBlackAdd();
	afx_msg void OnButtonBlackModify();
	afx_msg void OnButtonBlackdelete();
	afx_msg void OnButtonBlackClean();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonBlackInput();
	afx_msg void OnButtonBlackOuput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CBitmapButton m_first_button;
	CBitmapButton m_pre_button;
	CBitmapButton m_next_button;
	CBitmapButton m_last_button;
	CBitmapButton m_jump_button;
	CBitmapButton m_b_add;
	CBitmapButton m_b_edit;
	CBitmapButton m_b_delete;
	CBitmapButton m_b_input;
	CBitmapButton m_b_output;
	CBitmapButton m_b_clear;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETBLACK_H__0287AC76_30D4_46E3_A6CC_ADC0002FA4D4__INCLUDED_)
