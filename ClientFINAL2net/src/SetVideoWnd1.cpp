// SetVideoWnd1.cpp : implementation file
//

#include "stdafx.h"
#include "LAUMp4Test.h"
#include "SetVideoWnd1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetVideoWnd

CSetVideoWnd::CSetVideoWnd()
{
	m_ChanMotion = NULL;
	m_bready = FALSE;
}

CSetVideoWnd::~CSetVideoWnd()
{
}


BEGIN_MESSAGE_MAP(CSetVideoWnd, CStatic)
	//{{AFX_MSG_MAP(CSetVideoWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetVideoWnd message handlers

void CSetVideoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bready)
	{
		int x,y;
		x = point.x /m_wblock;
		y = point.y/m_hblock;
		m_ChanMotion->m_detect[y][x] = !m_ChanMotion->m_detect[y][x];
		Invalidate();
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CSetVideoWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bready)
	{
		static int no = 0;
		int nonow = (point.y/m_hblock)*m_HCount + (point.x /m_wblock);
		if(no != nonow)
		{
			no = nonow;
			m_curmouse_x = point.x;
			m_curmouse_y = point.y;
			Invalidate();
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CSetVideoWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush tempBrush(RGB(10,10,10));
	RECT Rect;
	GetClientRect(&Rect);
	CBrush *oldBrush = dc.SelectObject(&tempBrush);
	dc.Rectangle(&Rect);
	dc.SelectObject(oldBrush);
	if(m_bready)
	{
		int h,v;
		CBrush RectBrush(RGB(255,255,255));
		CBrush BrushRead(RGB(255,0,0));
		CDC *pDc = &dc;
		RECT rectwin;
		RECT rect;
		rect.left = m_curmouse_x/m_wblock;
		rect.left = rect.left * m_wblock;
		rect.right= rect.left + m_wblock;
		rect.top  = m_curmouse_y/m_hblock;
		rect.top  = rect.top * m_hblock;
		rect.bottom = rect.top + m_hblock;
		GetClientRect(&rectwin);
		rect.bottom = (rect.bottom > rectwin.bottom) ? rectwin.bottom : rect.bottom;
		rect.top = rect.bottom - m_hblock;
		
		pDc->FrameRect(&rect,&RectBrush);
		for(h=0;h<m_VCount;h++)
		{
			for(v=0;v<m_HCount;v++)
			{
				if(m_ChanMotion->m_detect[h][v])
				{
					rect.left = m_wblock * v;
					rect.right = rect.left + m_wblock;
					rect.top  = m_hblock * h;
					rect.bottom = rect.top + m_hblock;
					pDc->FrameRect(&rect,&BrushRead);
				}
			}
		}
	}
	// Do not call CStatic::OnPaint() for painting messages
}
