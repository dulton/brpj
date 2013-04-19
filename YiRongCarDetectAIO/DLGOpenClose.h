#if !defined(AFX_DLGOPENCLOSE_H__E4384DBD_823E_448D_8A19_D35D20778BB5__INCLUDED_)
#define AFX_DLGOPENCLOSE_H__E4384DBD_823E_448D_8A19_D35D20778BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGOpenClose.h : header file

//flag 定义 使其自适应这些类型窗口
#define OPENCLOSE_PREVIEW 1
#define OPENCLOSE_DETECT 2
#define OPENCLOSE_ALARM 3
#define OPENCLOSE_RECORD 4

/////////////////////////////////////////////////////////////////////////////
// CDLGOpenClose dialog

class CDLGOpenClose : public CDialog
{
// Construction
public:
	CDLGOpenClose(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGOpenClose)
	enum { IDD = IDD_OPEN_CLOSE_PREVIEW };
	CListCtrl	m_List;
	BOOL	m_check;
	int		m_winno;
	//}}AFX_DATA

	// 
	int ListChoose;
	int flag;
	BOOL OnInitDialog();
	void InitList(void);

	////////////////////////////////////////
	void BuildListPreview(void);
	void BuildListDetect(void);
	void BuildListAlarm(void);
	void BuildListRecord(void);
	
	void CheckOneListPreview(int iItem);
	void CheckOneListDetect(int iItem);
	void CheckOneListAlarm(int iItem);
	void CheckOneListRecord(int iItem);
	
	void CheckAllListPreview(void);
	void CheckAllListDetect(void);
	void CheckAllListAlarm(void);
	void CheckAllListRecord(void);

	void OpenListPreview(void);
	void OpenListDetect(void);
	void OpenListAlarm(void);
	void OpenListRecord(void);
	
	void CloseListPreview(void);
	void CloseListDetect(void);
	void CloseListAlarm(void);
	void CloseListRecord(void);
//////////////////////////////////////////

	//屏蔽操作
	void OnOK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGOpenClose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGOpenClose)
	afx_msg void OnCheck();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonClose();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupComboWinno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPENCLOSE_H__E4384DBD_823E_448D_8A19_D35D20778BB5__INCLUDED_)
