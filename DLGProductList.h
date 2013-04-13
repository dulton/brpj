#if !defined(AFX_DLGPRODUCTLIST_H__859504E4_6A6A_45FE_9383_7B9FD6FCCCA1__INCLUDED_)
#define AFX_DLGPRODUCTLIST_H__859504E4_6A6A_45FE_9383_7B9FD6FCCCA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGProductList.h : header file
//
#include <vector>
#include <list>
using namespace::std;

#include "IO.h"

#define SET_PRODUCT_PAGE_MAX_NUM 40

//产品字段
struct PRODUCT_INFO_ST
{
	unsigned long int nid;
	char barcode[64];	//一维码
	char name[256];		//名称
	char spec[2048];	//规格 其他信息
	char path1[260];		//图片路径
	char path2[260];		//图片路径
	char path3[260];		//图片路径
};

/////////////////////////////////////////////////////////////////////////////
// CDLGProductList dialog

class CDLGProductList : public CDialog
{
// Construction
public:
	CDLGProductList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGProductList)
	enum { IDD = IDD_PRODUCTLIST };
	CListCtrl	m_list;
	CString	m_image1;
	CString	m_image2;
	CString	m_image3;
	CString	m_product_name;
	CString	m_search_name;
	CString	m_spec;
	CString	m_search_barcode;
	CString	m_product_barcode;
	int		m_page;
	//}}AFX_DATA

	BOOL OnInitDialog();

	list<struct PRODUCT_INFO_ST> list_product;
	struct PRODUCT_INFO_ST data;

	int ListTotal;
	int ListNow;
	int ListChoose;

	void DisplayerList(void);
	void reflush();
	void Clear();

	void Struct2M(void);
	void M2Struct(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGProductList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGProductList)
	virtual void OnOK();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonImage1();
	afx_msg void OnButtonImage2();
	afx_msg void OnButtonImage3();
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

#endif // !defined(AFX_DLGPRODUCTLIST_H__859504E4_6A6A_45FE_9383_7B9FD6FCCCA1__INCLUDED_)
