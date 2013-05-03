// PlayWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PlayWnd.h"
#include "ScreenPannel.h"
#include "BarcodeRecord.h"
//////////////////////////////////////////////////////////////
#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayWnd

CPlayWnd::CPlayWnd()
	:m_nWndID(0)
{
	InitializeCriticalSection(&m_cs);
	m_nPreSplit = 0;
	//{{AFX_DATA_INIT(CPlayWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CPlayWnd, CWnd)
	//{{AFX_MSG_MAP(CPlayWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(VIDEO_MENU_BASE, VIDEO_MENU_END, OnVideoMenu)
	ON_MESSAGE(VIDEO_REPAINT, OnRepaintWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayWnd message handlers
BOOL CPlayWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rt;
	GetClientRect(&rt);
	CBrush br;
	br.CreateSolidBrush(VIDEO_BACK_COLOR);
	pDC->FillRect(&rt,&br);

	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

LRESULT CPlayWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	if(pContainer)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			pContainer->SetActivePage(this);
			{
				pCMainDlg->DlgPlaywin.SetCurWindId(m_nWndID);
//				pCMainDlg->DlgPlaywin.UpdateWndProductInfo();
//				BOOL bRecord = pCMainDlg->DlgPlaywin.GetCurWndRecordState();
//				pCMainDlg->DlgControl.UpdateBtnState(bRecord);

			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				BOOL bMulti = pContainer->GetMultiScreen();
				pContainer->SetMultiScreen(!bMulti);
				break;
			}
		default:
			break;
		}
	}
	return CWnd::DefWindowProc(message, wParam, lParam);
}

void CPlayWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	pContainer->SetActivePage(this);

	BOOL iplay = pCMainDlg->DlgPlaywin.GetCurWndPlayState();
	BOOL brecord = pCMainDlg->DlgPlaywin.GetCurWndRecordState();

//	CContainerWnd *pContainer = (CContainerWnd *)GetParent();
	CMenu menu;
	menu.CreatePopupMenu();
//	menu.AppendMenu(MF_STRING | pContainer->GetFullScreen()    ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_FULLSCREEN, NAME_MENU_FULLSCREEN);
//	menu.AppendMenu(MF_STRING | pContainer->GetMultiScreen()    ? MF_CHECKED : MF_UNCHECKED, 	VIDEO_MENU_MULTISCREEN, NAME_MENU_MULTISCREEN);
//	menu.AppendMenu(MF_STRING | pContainer->GetAutoAdjustPos() ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_AUTOADJUST, NAME_MENU_AUTOADJUST);
//	if(iplay == FALSE)
//		menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTPLAY, NAME_MENU_STARTPLAY);
//	else
//		menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPPLAY, NAME_MENU_STOPPLAY);

//	if(brecord == FALSE)
//		menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTRECORD, NAME_MENU_STARTRECORD);
//	else
//		menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPRECORD, NAME_MENU_STOPRECORD);
//	menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_CAPTURE, NAME_MENU_CAPTURE);


	TrackPopupMenu( 
		menu.m_hMenu, 
		TPM_LEFTALIGN, 
		point.x, 
		point.y,
		0,
		m_hWnd,
		NULL);
}

void CPlayWnd::OnVideoMenu(UINT nID)
{
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	switch(nID)
	{
	case VIDEO_MENU_FULLSCREEN:
		break;
	case VIDEO_MENU_MULTISCREEN:
		{
			break;
		}
	case VIDEO_MENU_AUTOADJUST:
		pContainer->SetAutoAdjustPos(!pContainer->GetAutoAdjustPos());
		break;
	case VIDEO_MENU_PRINTSCREEN:
		break;
	case VIDEO_MENU_RECORDVIDEO:
		break;
	case VIDEO_MENU_EXITDECODE:
		//pContainer->SetFullScreen(FALSE);
		break;
	case VIDEO_MENU_STARTPLAY:
		pContainer->SetCurWndPlayState(TRUE);
		break;
	case VIDEO_MENU_STOPPLAY:
		pContainer->SetCurWndPlayState(FALSE);
		break;
	case VIDEO_MENU_STARTRECORD:
		pContainer->SetCurWndRecordState(TRUE);
		break;
	case VIDEO_MENU_STOPRECORD:
		pContainer->SetCurWndRecordState(FALSE);
		break;
	case VIDEO_MENU_CAPTURE:
		pContainer->Capture();
		break;
	default:
		break;
	}
}


LRESULT CPlayWnd::OnRepaintWnd(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	
	return 0;
}

BOOL CPlayWnd::DestroyWindow() 
{
	DeleteCriticalSection(&m_cs);
	return CWnd::DestroyWindow();
}
