#if !defined(AFX_DLGAUDIOTALK_H__C4239995_283A_477A_BEE4_9AED1DB6C555__INCLUDED_)
#define AFX_DLGAUDIOTALK_H__C4239995_283A_477A_BEE4_9AED1DB6C555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAudioTalk.h : header file
//
#include "AudioInput.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAudioTalk dialog

class CDlgAudioTalk : public CDialog
{
// Construction
public:
	CDlgAudioTalk(CWnd* pParent = NULL);   // standard constructor
	BOOL	DeleteHandle(LONG hTalkHandle);

// Dialog Data
	//{{AFX_DATA(CDlgAudioTalk)
	enum { IDD = IDD_DIG_AUDIOTALK };
	CListCtrl	m_list;
	CString	m_addr;
	CString	m_password;
	int		m_port;
	CString	m_username;
	CString	m_sername;
	//}}AFX_DATA
	CAudioInput			m_input;
	CArray<LONG,LONG>	m_arrTalkHandle;
	HANDLE				m_hTalkMutex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAudioTalk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAudioTalk)
	afx_msg void OnBtnRev();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListTalk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnCaptureAudio(WPARAM,LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUDIOTALK_H__C4239995_283A_477A_BEE4_9AED1DB6C555__INCLUDED_)
