#pragma once
#include "afxwin.h"


// CDLGTab dialog

class CDLGTab : public CDialog
{
	DECLARE_DYNAMIC(CDLGTab)

public:
	CDLGTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGTab();

	CRect printf_Rect;	//È«¾Ö

	BOOL	OnInitDialog();
	void AutoSize();
// Dialog Data
	enum { IDD = IDD_TAB_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBVideo();
	afx_msg void OnBnClickedBCar();
	afx_msg void OnBnClickedBFace();
	CBitmapButton m_b_video;
	CBitmapButton m_b_car;
	CBitmapButton m_b_face;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
