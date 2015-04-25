
// CContainerWnd message handlers


// ContainerWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "ScanMate.h"
#include "ContainerWnd.h"


// CContainerWnd

IMPLEMENT_DYNAMIC(CContainerWnd, CWnd)

CContainerWnd::CContainerWnd()
{

}

CContainerWnd::~CContainerWnd()
{
}


BEGIN_MESSAGE_MAP(CContainerWnd, CWnd)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// CContainerWnd message handlers



int iVertPos;
void CContainerWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString strTitle;
	// TODO: Add your message handler code here and/or call default
	SCROLLINFO si;  
	si.cbSize = sizeof (si) ;
	si.fMask  = SIF_ALL ;
	GetScrollInfo (SB_VERT,  &si,SIF_ALL) ;

	//GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
	// Save the position for comparison later on

	iVertPos = si.nPos ;

	switch (nSBCode)
	{
	case SB_TOP:
		si.nPos = si.nMin ;
		break ;

	case SB_BOTTOM:
		si.nPos = si.nMax ;
		break ;

	case SB_LINEUP:
		si.nPos -= 10 ;
		break ;

	case SB_LINEDOWN:
		si.nPos += 10 ;
		break ;

	case SB_PAGEUP:
		si.nPos -= si.nPage ;
		break ;

	case SB_PAGEDOWN:
		si.nPos += si.nPage ;
		break ;

	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos ;
		break ;

	default:
		break ;         
	}
	// Set the position and then retrieve it.  Due to adjustments
	//   by Windows it may not be the same as the value set.

//	strTitle.Format(_T("%d,%d"),si.nPos,si.nMax);
//	GetParent()->SetWindowText(strTitle);

	si.fMask = SIF_POS ;
	SetScrollInfo ( SB_VERT, &si, TRUE) ;
	GetScrollInfo ( SB_VERT,  &si,SIF_ALL) ;

	// If the position has changed, scroll the window and update it

	if (si.nPos != iVertPos)
	{                    
		ScrollWindow ( 0, (iVertPos - si.nPos), 	NULL, NULL) ;
		UpdateWindow () ;

	}

	GetParent()->Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

}
