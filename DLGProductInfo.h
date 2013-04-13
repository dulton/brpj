#if !defined(AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_)
#define AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGProductInfo.h : header file
//

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
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	BOOL OnInitDialog();
	void AutoSize();
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
	afx_msg void OnButtonOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRODUCTINFO_H__ED5016EC_9A6B_414D_AB0A_7C151DB9D75E__INCLUDED_)
