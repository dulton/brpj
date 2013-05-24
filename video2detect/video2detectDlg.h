// video2detectDlg.h : header file
//

#if !defined(AFX_VIDEO2DETECTDLG_H__0A66D1B2_2FD0_45E4_8514_0BAB9712C572__INCLUDED_)
#define AFX_VIDEO2DETECTDLG_H__0A66D1B2_2FD0_45E4_8514_0BAB9712C572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVideo2detectDlg dialog

class CVideo2detectDlg : public CDialog
{
// Construction
public:
	CVideo2detectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVideo2detectDlg)
	enum { IDD = IDD_VIDEO2DETECT_DIALOG };
	CListCtrl	m_ListCar;
	CStatic	m_playwin;
	CProgressCtrl	m_progress_file;
	CString	m_edit_file;
	CString	m_ip;
	//}}AFX_DATA

	//Ïß³Ì¾ä±ú
	HANDLE pthread;
	void EnablePlayState() ;
	char CurrentDir[MAX_PATH];
	char ResultDir[MAX_PATH];

	HDC playwin_hdc;
	RECT playwin_rect;

	int m_ListCarTotal;
	void ListMainInit(void);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideo2detectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVideo2detectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonDetect();
	afx_msg void OnButtonStop();
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEO2DETECTDLG_H__0A66D1B2_2FD0_45E4_8514_0BAB9712C572__INCLUDED_)
