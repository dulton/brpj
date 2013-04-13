// BarcodeRecordDlg.h : header file
//

#if !defined(AFX_BARCODERECORDDLG_H__802831DD_C75D_4DE1_A00A_5A7A6D1A5B32__INCLUDED_)
#define AFX_BARCODERECORDDLG_H__802831DD_C75D_4DE1_A00A_5A7A6D1A5B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DLGControl.h"
#include "DLGPlaywin.h"
#include "DLGProductInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CBarcodeRecordDlg dialog

class CBarcodeRecordDlg : public CDialog
{
// Construction
public:
	CBarcodeRecordDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeRecordDlg)
	enum { IDD = IDD_BARCODERECORD_DIALOG };
	CTreeCtrl	m_tree;
	//}}AFX_DATA

	CDLGControl DlgControl;
	CDLGPlaywin	DlgPlaywin;
	CDLGProductInfo	DlgProductInfo;

	CRect		m_clientRect;		//程序界面区域位置
	//切换位置
	void UpdatePannelPosition();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeRecordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBarcodeRecordDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODERECORDDLG_H__802831DD_C75D_4DE1_A00A_5A7A6D1A5B32__INCLUDED_)
