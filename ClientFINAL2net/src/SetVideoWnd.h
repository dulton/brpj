#if !defined(AFX_SETVIDEOWND_H__EFC87854_C9E7_4200_BEDA_D65B7FBE7DC9__INCLUDED_)
#define AFX_SETVIDEOWND_H__EFC87854_C9E7_4200_BEDA_D65B7FBE7DC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetVideoWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetVideoWnd window

class CSetVideoWnd : public CStatic
{
// Construction
public:
	CSetVideoWnd();

// Attributes
public:
	CHANNMOTION *m_pChanMotion;

	int      m_curmouse_x,m_curmouse_y;
	int      m_wblock,m_hblock;
	int      m_ImageWidth,m_ImageHeight;
	int      m_HCount,m_VCount;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetVideoWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetVideoWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetVideoWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETVIDEOWND_H__EFC87854_C9E7_4200_BEDA_D65B7FBE7DC9__INCLUDED_)
