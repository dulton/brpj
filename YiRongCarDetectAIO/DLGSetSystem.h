#include "afxwin.h"
#if !defined(AFX_DLGSETSYSTEM_H__53ABD703_817F_4073_B3FA_34144C551BEA__INCLUDED_)
#define AFX_DLGSETSYSTEM_H__53ABD703_817F_4073_B3FA_34144C551BEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetSystem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGSetSystem dialog

class CDLGSetSystem : public CDialog
{
// Construction
public:
	CDLGSetSystem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSetSystem)
	enum { IDD = IDD_SET_SYSTEM };
	BOOL	m_check_alarmpic;
	BOOL	m_check_alarmwav;
	BOOL	m_check_ftp;
	int		m_recordfull;
	CString	m_path_alarmwav;
	CString	m_path_capbmp;
	CString	m_path_detect;
	CString	m_path_record;
	int		m_record_cuttime;
	CString	m_ftp_ip;
	CString	m_ftp_port;
	CString	m_ftp_psw;
	CString	m_ftp_user;
	BOOL	m_check_savenet;
	CString	m_path_dahua;
	CString	m_path_haikang;
	CString	m_path_yaan;
	CString	m_ftp_path;
	CString	m_tomcat_dir;
	CString	m_tomcat_url;
	//}}AFX_DATA
	TCHAR SystemIniPath[ZOG_MAX_PATH_STR];
  CBrush m_BgBrush;           // ±³¾°É«»­Ë¢

	BOOL OnInitDialog();


	void readini(char *path);
	void writeini(char *path);

	void Read2Dlg();
	void Dlg2Write();

	void InputIni();
	void OutputIni();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetSystem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetSystem)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonAlarmWav();
	afx_msg void OnButtonDetectDir();
	afx_msg void OnButtonRecordDir();
	afx_msg void OnButtonCapbmpDir();
	afx_msg void OnButtonHaikangPath();
	afx_msg void OnButtonDahuaPath();
	afx_msg void OnButtonYaanPath();
	afx_msg void OnButtonTomcatDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_display_preview;
	CString m_update_url;
	BOOL m_check_update;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBitmapButton m_b_ok;
	CBitmapButton m_b_cancel;
	CBitmapButton m_b_alarmwav;
	CBitmapButton m_b_detectdir;
	CBitmapButton m_b_recorddir;
	CBitmapButton m_b_cambmpdir;
	CBitmapButton m_b_haikangpath;
	CBitmapButton m_b_dahuapath;
	CBitmapButton m_b_yaanpath;
	CBitmapButton m_b_tomcatdir;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETSYSTEM_H__53ABD703_817F_4073_B3FA_34144C551BEA__INCLUDED_)
