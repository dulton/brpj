#if !defined(AFX_DLGSETTINGS_H__4EE5CA2D_F23A_4BC5_A495_307E2EFCDD0E__INCLUDED_)
#define AFX_DLGSETTINGS_H__4EE5CA2D_F23A_4BC5_A495_307E2EFCDD0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSettings.h : header file
//

//视频/图片命名规则定义
#define HMNUM_FACTORYITEM_DATETIME	0		//货号\\工厂名称+日期
#define FACTORYITEM_HMNUM_DATETIME	1		//工厂名称\\货号+日期

/////////////////////////////////////////////////////////////////////////////
// CDLGSettings dialog

class CDLGSettings : public CDialog
{
// Construction
public:
	CDLGSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSettings)
	enum { IDD = IDD_SETTINGS };
	CString	m_record;
	CString	m_shotframe;
	int		m_comboRecord;
	int		m_comboShotframe;
	//}}AFX_DATA

	BOOL OnInitDialog();

	TCHAR SystemIniPath[ZOG_MAX_PATH_STR];

	void readini(char *path);
	void writeini(char *path);

	void Read2Dlg();
	void Dlg2Write();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSettings)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonShotframe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGS_H__4EE5CA2D_F23A_4BC5_A495_307E2EFCDD0E__INCLUDED_)
