#if !defined(AFX_EXBUTTON_H__BF46D1BA_5CA1_42D4_BD5E_82CB1B0BA598__INCLUDED_)
#define AFX_EXBUTTON_H__BF46D1BA_5CA1_42D4_BD5E_82CB1B0BA598__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExButton window

enum PTZ_FLAG_ENUM
{
	NORMAL_PTZ_FLAG=1,
	SET_PTZ_FLAG=2
};

class CExButton : public CBitmapButton
{
	DWORD  m_dwPTZCommand;

	//1== normal ptz 常规里的云台
	//2==set ptz 云台设置
	int ptzflag; 

// Construction
public:
	CExButton();

	void   SetButtonCommand(DWORD dwPTZCommand,int flag);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXBUTTON_H__BF46D1BA_5CA1_42D4_BD5E_82CB1B0BA598__INCLUDED_)
