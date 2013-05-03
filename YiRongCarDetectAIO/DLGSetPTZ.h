#if !defined(AFX_DLGSETPTZ_H__A6A24F31_FAB0_41D9_9271_4C7831A28886__INCLUDED_)
#define AFX_DLGSETPTZ_H__A6A24F31_FAB0_41D9_9271_4C7831A28886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetPTZ.h : header file
//
#include "ExButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDLGSetPTZ dialog

class CDLGSetPTZ : public CDialog
{
// Construction
public:
	CDLGSetPTZ(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSetPTZ)
	enum { IDD = IDD_SET_PTZ };

	CExButton	m_auto;
	CExButton	m_go;
	CExButton	m_set;
	CExButton	m_down;
	CExButton	m_downleft;
	CExButton	m_downright;
	CExButton	m_focussub;
	CExButton	m_focusadd;
	CExButton	m_irisadd;
	CExButton	m_irissub;
	CExButton	m_left;
	CExButton	m_right;
	CExButton	m_speedadd;
	CExButton	m_speedsub;
	CExButton	m_up;
	CExButton	m_zoomsub;
	CExButton	m_zoomadd;
	CExButton	m_upright;
	CExButton	m_upleft;

	int		m_edit_speed;
	int		m_edit_set;
	//}}AFX_DATA
	BOOL	OnInitDialog();
	void AutoSize();
	void ButtonBMP();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetPTZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetPTZ)
	afx_msg void OnPaint();
	afx_msg void OnUpleft();
	afx_msg void OnUp();
	afx_msg void OnUpright();
	afx_msg void OnLeft();
	afx_msg void OnAuto();
	afx_msg void OnRight();
	afx_msg void OnDownleft();
	afx_msg void OnDown();
	afx_msg void OnDownright();
	afx_msg void OnFocusAdd();
	afx_msg void OnIrisAdd();
	afx_msg void OnZoomAdd();
	afx_msg void OnFocusSub();
	afx_msg void OnIrisSub();
	afx_msg void OnZoomSub();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonGo();
	afx_msg void OnSpeedSub();
	afx_msg void OnSpeedAdd();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPTZ_H__A6A24F31_FAB0_41D9_9271_4C7831A28886__INCLUDED_)
