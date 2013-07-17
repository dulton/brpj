#if !defined(AFX_SETVIDEOWND1_H__21A953DA_B0B6_4C71_B064_6B2B55C66C43__INCLUDED_)
#define AFX_SETVIDEOWND1_H__21A953DA_B0B6_4C71_B064_6B2B55C66C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetVideoWnd1.h : header file
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
	int      m_bIsArea;
	int      m_bready;
	int      m_curmouse_x,m_curmouse_y;
	int      m_wblock,m_hblock;
	int      m_ImageWidth,m_ImageHeight;
	int      m_HCount,m_VCount;
	CHANNMOTION *m_ChanMotion;

	int      m_bVideoMaskBtnDown;
	int      m_maskmax_w,m_maskmax_h;
	int      m_vmask_x,m_vmask_y,m_vmask_w,m_vmask_h;
	CHANNVIDEOMASK *pvmask;
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETVIDEOWND1_H__21A953DA_B0B6_4C71_B064_6B2B55C66C43__INCLUDED_)
