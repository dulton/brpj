// ExButton.cpp : implementation file
//

#include "stdafx.h"
#include "ExButton.h"
#include "YiRongCarDetectAIO.h"

#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

#include "DLGLogin.h"
extern CDLGLogin DlgLogin;

#include "DLGSetPTZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExButton

CExButton::CExButton()
{
	m_dwPTZCommand = -1;
	ptzflag=0;
}

CExButton::~CExButton()
{
}


BEGIN_MESSAGE_MAP(CExButton, CButton)
	//{{AFX_MSG_MAP(CExButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExButton message handlers
void CExButton::SetButtonCommand(DWORD dwPTZCommand,int flag)
{
	m_dwPTZCommand = dwPTZCommand;
	ptzflag=flag;

}

void CExButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!DlgLogin.CurrentUser.ptz)
	{
		MessageBox("无 云台设置 权限，请联系管理员",MESSAGEBOX_TITLE);
		return ;
	}

	CBitmapButton::OnLButtonDown(nFlags, point);

	if((int)m_dwPTZCommand < 0)
	{
		return;
	}

	switch(ptzflag)
	{
	case NORMAL_PTZ_FLAG:
		DlgMain->DlgPtz.SendPtzControl(m_dwPTZCommand,FALSE);
		break;
	case SET_PTZ_FLAG:
		((CDLGSetPTZ*)GetParent())->SendPtzControl(m_dwPTZCommand,FALSE);
		break;
	default:
		break;
	}

	TRACE("START\n");
}

void CExButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CBitmapButton::OnLButtonUp(nFlags, point);

	if((int)m_dwPTZCommand < 0)
	{
		return;		
	}

	switch(ptzflag)
	{
	case NORMAL_PTZ_FLAG:
		DlgMain->DlgPtz.SendPtzControl(m_dwPTZCommand,TRUE);
		break;
	case SET_PTZ_FLAG:
		((CDLGSetPTZ*)GetParent())->SendPtzControl(m_dwPTZCommand,TRUE);
		break;
	default:
		break;
	}

	TRACE("STOP\n");
}
