#if !defined(AFX_DLGADDAREA_H__70D127CF_033D_4505_B260_5D0A9CC28DE7__INCLUDED_)
#define AFX_DLGADDAREA_H__70D127CF_033D_4505_B260_5D0A9CC28DE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGAddArea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGAddArea dialog

class CDLGAddArea : public CDialog
{
// Construction
public:
	CDLGAddArea(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGAddArea)
	enum { IDD = IDD_DIALOG1 };
	CString	m_Area;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGAddArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGAddArea)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDAREA_H__70D127CF_033D_4505_B260_5D0A9CC28DE7__INCLUDED_)
