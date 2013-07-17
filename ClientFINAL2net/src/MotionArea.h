#if !defined(AFX_MOTIONAREA_H__F5648E49_0D00_4272_AF9D_193A759A7E70__INCLUDED_)
#define AFX_MOTIONAREA_H__F5648E49_0D00_4272_AF9D_193A759A7E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotionArea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMotionArea window

class CMotionArea : public CStatic
{
// Construction
public:
	CMotionArea();

// Attributes
public:
	//LONG     m_hchann;
	int      m_bIsArea;
	int      m_curmouse_x,m_curmouse_y;
	int      m_wblock,m_hblock;
	BOOL	 m_bLinkOk;
	int      m_ImageWidth,m_ImageHeight;
	int      m_HCount,m_VCount;
	CHANNMOTION *m_pmotion;

	int      m_bVideoMaskBtnDown;
	int      m_maskmax_w,m_maskmax_h;
	int      m_vmask_x,m_vmask_y,m_vmask_w,m_vmask_h;
	CHANNVIDEOMASK *pvmask;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotionArea)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMotionArea();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMotionArea)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTIONAREA_H__F5648E49_0D00_4272_AF9D_193A759A7E70__INCLUDED_)
