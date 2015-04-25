// PlayWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SmartVillage.h"
#include "PlayWnd.h"
#include "ScreenPannel.h"
/////////////////////////////////
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;
//////////////////////////////////
#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

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
	oldscreenmode=SCREEN_16MODE;
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

	//显示预览摄像头名称
	if( DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[m_nWndID].isplay)
	{
		pDC->SetTextColor(RGB(0,0,255));
		pDC->DrawText(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[m_nWndID].name,rt,DT_SINGLELINE|DT_LEFT|DT_VCENTER); 
	}

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
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
			{
				pContainer->SetActivePage(this);
				DlgMain->DlgTabVideo.DlgScreen.SetCurWindId(m_nWndID);
				DlgMain->DlgTabVideo.DlgNormal.UpdateNormalWnd();
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
		//		CSmartVillageDlg * pMainWnd = (CSmartVillageDlg *)(AfxGetMainWnd());
		//	用	DlgMain 替代
				//屏蔽
			//	BOOL bMulti = pContainer->GetMultiScreen();
			//pContainer->SetMultiScreen(!bMulti);
				BOOL bMulti = pContainer->GetMultiScreen();
				if(bMulti)
				{
					oldscreenmode=DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.screenmode;
					DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode1();
				}
				else
				{
					switch(oldscreenmode)
					{
					case SCREEN_1MODE:DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode1();break;
					case SCREEN_4MODE:DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode4();break;
					case SCREEN_9MODE:DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode9();break;
					case SCREEN_16MODE:DlgMain->DlgTabVideo.DlgScreen.DlgShortCut.OnButtonMode16();break;
					default:break;
					}
				}
					
			}
			break;
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

	int screenNo = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
/*
//	CContainerWnd *pContainer = (CContainerWnd *)GetParent();
	CMenu menu;
	menu.CreatePopupMenu();
//	menu.AppendMenu(MF_STRING | pContainer->GetFullScreen()    ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_FULLSCREEN, NAME_MENU_FULLSCREEN);
//	menu.AppendMenu(MF_STRING | pContainer->GetMultiScreen()    ? MF_CHECKED : MF_UNCHECKED, 	VIDEO_MENU_MULTISCREEN, NAME_MENU_MULTISCREEN);
	menu.AppendMenu(MF_STRING | pContainer->GetAutoAdjustPos() ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_AUTOADJUST, NAME_MENU_AUTOADJUST);
	menu.AppendMenu(MF_STRING | pContainer->GetPlayState(screenNo) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTPLAY, NAME_MENU_STARTPLAY);
	menu.AppendMenu(MF_STRING | (!pContainer->GetPlayState(screenNo)) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPPLAY, NAME_MENU_STOPPLAY);
	menu.AppendMenu(MF_STRING | pContainer->GetCarDetectState(screenNo) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTDETECT, NAME_MENU_STARTDETECT);
	menu.AppendMenu(MF_STRING | (!pContainer->GetCarDetectState(screenNo)) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPDETECT, NAME_MENU_STOPDETECT);
	menu.AppendMenu(MF_STRING | pContainer->GetRecordState(screenNo) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTRECORD, NAME_MENU_STARTRECORD);
	menu.AppendMenu(MF_STRING | (!pContainer->GetRecordState(screenNo)) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPRECORD, NAME_MENU_STOPRECORD);
//	menu.AppendMenu(MF_STRING | pContainer->GetAlarmState(screenNo) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STARTALARM, NAME_MENU_STARTALARM);
//	menu.AppendMenu(MF_STRING | (!pContainer->GetAlarmState(screenNo)) ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_STOPALARM, NAME_MENU_STOPALARM);
	menu.AppendMenu(MF_STRING | 0 ? MF_CHECKED : MF_UNCHECKED, VIDEO_MENU_CAPTURE, NAME_MENU_CAPTURE);


	TrackPopupMenu( 
		menu.m_hMenu, 
		TPM_LEFTALIGN, 
		point.x, 
		point.y,
		0,
		m_hWnd,
		NULL);
*/

}

void CPlayWnd::OnVideoMenu(UINT nID)
{
	CScreenPannel *pContainer = (CScreenPannel *)GetParent();
	int screenNo = DlgMain->DlgTabVideo.DlgScreen.GetCurWindId();
	switch(nID)
	{
	case VIDEO_MENU_FULLSCREEN:
		//pContainer->SetFullScreen(!pContainer->GetFullScreen());
		break;
	case VIDEO_MENU_MULTISCREEN:
		{
			BOOL bMulti = pContainer->GetMultiScreen();
			pContainer->SetMultiScreen(!bMulti);
			break;
		}
	case VIDEO_MENU_AUTOADJUST:
		pContainer->SetAutoAdjustPos(!pContainer->GetAutoAdjustPos());
		break;
	case VIDEO_MENU_PRINTSCREEN:
		break;
	case VIDEO_MENU_STARTRECORD:
		if(!DlgLogin.CurrentUser.record)
		{
			MessageBox("无 录制 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindRecordState(screenNo,true);
		break;
	case VIDEO_MENU_STOPRECORD:
		if(!DlgLogin.CurrentUser.record)
		{
			MessageBox("无 录制 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindRecordState(screenNo,false);
		break;
	case VIDEO_MENU_STARTPLAY:
		if(!DlgLogin.CurrentUser.preview)
		{
			MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindPlayState(screenNo,true);
		break;
	case VIDEO_MENU_STOPPLAY:
		if(!DlgLogin.CurrentUser.preview)
		{
			MessageBox("无 预览 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindPlayState(screenNo,false);
		break;
	case VIDEO_MENU_STARTDETECT:
		if(!DlgLogin.CurrentUser.Cardetect)
		{
			MessageBox("无 识别 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindCarDetectState(screenNo,true);
		break;
	case VIDEO_MENU_STOPDETECT:
		if(!DlgLogin.CurrentUser.Cardetect)
		{
			MessageBox("无 识别-报警 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->SetWindCarDetectState(screenNo,false);
		break;
	case VIDEO_MENU_STARTALARM:
		
	
		break;
	case VIDEO_MENU_STOPALARM:
	
		break;
	case VIDEO_MENU_CAPTURE:
		if(!DlgLogin.CurrentUser.capbmp)
		{
			MessageBox("无 抓图 权限，请联系管理员",MESSAGEBOX_TITLE);
			return ;
		}
		pContainer->Capture(screenNo);
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
