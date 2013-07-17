#if !defined(AFX_DLGMOTIONAREA_H__7CAD5B89_B31C_442D_827E_12EB171F723B__INCLUDED_)
#define AFX_DLGMOTIONAREA_H__7CAD5B89_B31C_442D_827E_12EB171F723B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMotionArea.h : header file
//

#include "SetVideoWnd1.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMotionArea dialog

class CDlgMotionArea : public CDialog
{
// Construction
public:
	char m_servername[24];
	char m_url[40];
	char m_username[20];
	char m_password[20];
	WORD m_port;
	char m_ch;
	CHANNMOTION *m_ChanMotion;
	LONG m_hchann;
public:
	CDlgMotionArea(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMotionArea)
	enum { IDD = IDD_DLG_MOTIONAREA };
	CSliderCtrl	m_MotionVal;
	CSetVideoWnd	m_VideoWnd;
	BOOL	m_motiontest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMotionArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMotionArea)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnAll();
	afx_msg void OnBtnClean();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnOk();
	virtual void OnCancel();
	afx_msg void OnCheckTest();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOTIONAREA_H__7CAD5B89_B31C_442D_827E_12EB171F723B__INCLUDED_)
