#if !defined(AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_)
#define AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGProductInfo.h : header file
//

#include "Picture.h"
#include "SqliteOperate.h"

/*
struct PRODUCT_INFO_ST
{
	unsigned long int nid;
	char RunningNumber[256];	
	char tag[256];		
	char MainCategory[256];
	char SubCategory[256];
	char MetaField[256];
	char ColourDesc[256];
	char Colour[256];
	char Unit[256];
	char FactoryItem[256];
	char HmNum[256];
	char Description[2560];
	char path1[260];		//Í¼Æ¬Â·¾¶
	char path2[260];		//Í¼Æ¬Â·¾¶
	char path3[260];		//Í¼Æ¬Â·¾¶
};
*/

/////////////////////////////////////////////////////////////////////////////
// CDLGProductInfo dialog

class CDLGProductInfo : public CDialog
{
// Construction
public:
	CDLGProductInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGProductInfo)
	enum { IDD = IDD_PRODUCT_INFO };
	CEdit	m_barcodeCtrl;
	CStatic	m_pic3;
	CStatic	m_pic2;
	CStatic	m_pic1;
	CString	m_barcode;
	//}}AFX_DATA
	CPicture pic1;
	CPicture pic2;
	CPicture pic3;
	CFont TextFont;
	UINT  m_recordtimer;
	CString old_barcode;

	BOOL OnInitDialog();
	void AutoSize();
	void Display(int i);
	void Clean(int i);
	void CleanTemp();
	void DisplayTemp();
	void temp2data(int i);
	void DisplayLite(struct PRODUCT_INFO_ST &input);
	void RecordTimerEvent();

	int curchoose;
	struct PRODUCT_INFO_ST data[MAX_PLAYWIN];
	struct PRODUCT_INFO_ST temp;

	HBITMAP bi;

	afx_msg void OnButtonOk();
	//ÆÁ±Î²Ù×÷
	void OnCancel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGProductInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGProductInfo)

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_)
