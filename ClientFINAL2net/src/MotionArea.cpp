// MotionArea.cpp : implementation file
//

#include "stdafx.h"
#include "laump4test.h"
#include "MotionArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotionArea

CMotionArea::CMotionArea()
{
	m_bLinkOk = FALSE;
	m_bIsArea = TRUE;

	m_bVideoMaskBtnDown = FALSE;
	m_maskmax_w = m_maskmax_h = 0;

	m_vmask_x = m_vmask_y = m_vmask_w = m_vmask_h = 0;
}

CMotionArea::~CMotionArea()
{
}


BEGIN_MESSAGE_MAP(CMotionArea, CStatic)
	//{{AFX_MSG_MAP(CMotionArea)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotionArea message handlers

void CMotionArea::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	{
		CRect Rect;
		//CBrush tempBrush(m_hChHandle == NULL ? UNLINK_COLOR : FACE_COLOR);
		CBrush tempBrush(RGB(0,0,0));
		GetClientRect(&Rect);
		dc.FillRect(Rect, &tempBrush);
		/*if(m_hchann != -1)
		{
			VSNET_ClientRefrenshWnd(m_hchann);
		}*/
	}
	if(!m_bLinkOk) return;
	if(m_bIsArea)
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
				if(m_pmotion->m_detect[h][v])
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
	else
	{
		RECT rect;
		CBrush BrushRed(RGB(255,0,0));
		CDC *pDc = &dc;
		if(m_vmask_w > 0)
		{
			rect.left   = m_vmask_x;
			rect.right  = rect.left + m_vmask_w;
		}
		else
		{
			rect.left   = m_vmask_x + m_vmask_w;
			rect.right  = rect.left - m_vmask_w;
		}
		if(m_vmask_h > 0)
		{
			rect.top    = m_vmask_y;
			rect.bottom = rect.top + m_vmask_h;
		}
		else
		{
			rect.top    = m_vmask_y + m_vmask_h;
			rect.bottom = rect.top - m_vmask_h;
		}
		pDc->FrameRect(&rect,&BrushRed);
	}
}

void CMotionArea::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bLinkOk) return;
	if(m_bIsArea)
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
	else 
	{
		if(m_bVideoMaskBtnDown && m_bLinkOk && nFlags == MK_LBUTTON)
		{
			RECT rect;
			GetClientRect(&rect);
			point.x = point.x < 0 ? 0 : point.x;
			point.x = point.x > rect.right ? rect.right : point.x;
			point.y = point.y < 0 ? 0 : point.y;
			point.y = point.y > rect.bottom ? rect.bottom : point.y;
			if(abs(point.x - m_vmask_x) > m_maskmax_w)
			{
				m_vmask_w = point.x > m_vmask_x ? m_maskmax_w : -m_maskmax_w;
			}
			else
			{
				m_vmask_w  = point.x - m_vmask_x;
			}
			if(abs(point.y - m_vmask_y) > m_maskmax_h)
			{
				m_vmask_h = point.y > m_vmask_y ? m_maskmax_h : -m_maskmax_h;
			}
			else
			{
				m_vmask_h = point.y - m_vmask_y;
			}
			if(m_vmask_w > 0)
			{
				pvmask->m_x     = m_vmask_x;
				pvmask->m_width = m_vmask_w;
			}
			else
			{
				pvmask->m_x		= m_vmask_x + m_vmask_w;
				pvmask->m_width = -m_vmask_w;
			}
			if(m_vmask_h > 0)
			{
				pvmask->m_y      = m_vmask_y;
				pvmask->m_height = m_vmask_h;
			}
			else
			{
				pvmask->m_y      = m_vmask_y + m_vmask_h;
				pvmask->m_height = - m_vmask_h;
			}
		}
		Invalidate();
	}
	//CStatic::OnMouseMove(nFlags, point);
}

void CMotionArea::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_bLinkOk) return;
	if(m_bIsArea)
	{
		int x,y;
		x = point.x / m_wblock;
		y = point.y / m_hblock;
		m_pmotion->m_detect[y][x] = !m_pmotion->m_detect[y][x];
		Invalidate();
	}
	else
	{
		m_bVideoMaskBtnDown = TRUE;
		m_vmask_x = point.x;
		m_vmask_y = point.y;
		m_vmask_w  = 0;
		m_vmask_h = 0;
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CMotionArea::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bVideoMaskBtnDown = FALSE;
	CStatic::OnLButtonUp(nFlags, point);
}
