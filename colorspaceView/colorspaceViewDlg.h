// colorspaceViewDlg.h : header file
//

#if !defined(AFX_COLORSPACEVIEWDLG_H__1E70661E_6E99_428A_8BE6_396C3F7332D0__INCLUDED_)
#define AFX_COLORSPACEVIEWDLG_H__1E70661E_6E99_428A_8BE6_396C3F7332D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CColorspaceViewDlg dialog
#include "DCCache.h"
#include "openglctrl.h"
#include "afxwin.h"

class CColorspaceViewDlg : public CDialog
{
// Construction
public:
	CColorspaceViewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CColorspaceViewDlg)
	enum { IDD = IDD_COLORSPACEVIEW_DIALOG };
	CStatic	m_gdi;
	COpenGLCtrl m_gl;
	COpenGLCtrl m_gl_wp;

	COpenGLCtrl m_gl_gamut;
	//}}AFX_DATA
	CRect m_clientRect;
	CDCCache m_Cache;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorspaceViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CColorspaceViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonWhitepoint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	 afx_msg BOOL OnEraseBkgnd( CDC* pDC ); 
	afx_msg void OnButtonLum();
	afx_msg void OnButtonGamut() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	CComboBox m_c_cie;
	CComboBox m_c_cp;
	CComboBox m_c_wp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSPACEVIEWDLG_H__1E70661E_6E99_428A_8BE6_396C3F7332D0__INCLUDED_)
