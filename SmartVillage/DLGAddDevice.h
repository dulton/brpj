#if !defined(AFX_DLGADDDEVICE_H__17D456BA_68FE_40DD_8F3A_588C9B325DA6__INCLUDED_)
#define AFX_DLGADDDEVICE_H__17D456BA_68FE_40DD_8F3A_588C9B325DA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGAddDevice.h : header file
//
#include "YRSVMySQL.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDLGAddDevice dialog

class CDLGAddDevice : public CDialog
{
// Construction
public:
	struct DEVICE_AREA_ST AreaList[MAX_AREA];
	int  AreaCount;
	int  AreaComboCur;

	int  VenderComboCur;
	int RTPComboCur;
	int DecodeTagComboCur;
public:
	CDLGAddDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGAddDevice)
	enum { IDD = IDD_ADD_DEVICE };
	CComboBox	m_AreaComboCtrl;
	CComboBox	m_camVender;
	CString	m_CamIpAddr;
	CString	m_CamName;
	CString	m_CamPsw;
	CString	m_CamUser;

	CString	m_CamArea;
	int		m_CamPort;
	int		m_CamChannel;

	CString m_CamRtspurl;
	CComboBox m_CamRTP;
	CComboBox m_CamDecodetag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGAddDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGAddDevice)
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	afx_msg void OnMenuitemEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnCloseupComboCamvender();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBitmapButton m_b_ok;
	CBitmapButton m_b_cancel;

	CString m_longitude;
	CString m_latitude;
	

	CComboBox m_camDirection;
	CComboBox m_comSVmode;
	int m_Direction;
	int m_SVmode;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDDEVICE_H__17D456BA_68FE_40DD_8F3A_588C9B325DA6__INCLUDED_)
