// VideoChannel.cpp: implementation of the CYaanCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YaanCamera.h"
#include "BarcodeRecord.h"
//////////////////////////////////////////////////////////////
#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;

#include "Yaanlib/launet.h"
#include "Yaanlib/megaplay.h"

#pragma comment( lib, "Yaanlib/NetClient.lib")
#pragma comment( lib, "Yaanlib/NetPlaySDK.lib")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////
CYaanCamera::CYaanCamera()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		m_bplay[i] = FALSE;
		m_playhandle[i] = i;
	}
}

//////////////////////////////////////////////////////////////
CYaanCamera::~CYaanCamera()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		StopPlay(i);
	}
}

//////////////////////////////////////////////////////////////
void CYaanCamera::SDKinit()
{
	VSNET_ClientStartup(NULL,NULL); //call this function to initialize SDK;
}

//////////////////////////////////////////////////////////////
void CYaanCamera::StartPlay(int nCuWinID,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd)
{
	CHANNEL_CLIENTINFO pVSChann;

	pVSChann.m_hChMsgWnd  = NULL;
	pVSChann.m_nChmsgid   = 0;
	pVSChann.m_sername    = name;
	pVSChann.m_username   = user;
	pVSChann.m_password   = psw;
	pVSChann.m_playstart  = TRUE;
	pVSChann.m_tranType   = 3;
	pVSChann.m_useoverlay = FALSE;
	pVSChann.nColorKey    = RGB(192,0,192);
	pVSChann.m_ch         = 0;
	pVSChann.m_buffnum    = 20;
	pVSChann.m_hVideohWnd = hWnd;
	pVSChann.context      = NULL;
	pVSChann.m_messagecallback = NULL;

	if (m_playhandle[nCuWinID] != -1)
	{
		VSNET_ClientStop(m_playhandle[nCuWinID]);
		m_playhandle[nCuWinID] = -1;
	}
	m_playhandle[nCuWinID] = VSNET_ClientStart(sip,&pVSChann,port);
	if (m_playhandle[nCuWinID] == -1)
	{
		AfxMessageBox(_T("启动通道失败 ：\n句柄获取失败\n可能输入的IP不存在"));
		return;
	}
	else
	{
		m_bplay[nCuWinID] = TRUE;
		VSNET_ClientSetWnd(m_playhandle[nCuWinID],hWnd);
		VSNET_ClientMediaData(m_playhandle[nCuWinID],TRUE);
		VSNET_ClientStartViewEx(m_playhandle[nCuWinID]);
	}
}

//////////////////////////////////////////////////////////////
void CYaanCamera::StopPlay(int nCuWinID)
{
	int iRet=0;
	m_bplay[nCuWinID] = FALSE;
	if(m_playhandle[nCuWinID] != -1)
	{
		VSNET_ClientStop(m_playhandle[nCuWinID]);
		m_playhandle[nCuWinID] = -1;
	}
}

//////////////////////////////////////////////////////////////
void CYaanCamera::CapturePic(int nCuWinID,char *filename)
{
	VSNET_ClientCapturePicture(m_playhandle[nCuWinID], filename);
}

//////////////////////////////////////////////////////////////
int CYaanCamera::StartRecord(int nCuWinID,LPCSTR filename)
{
	int iRet=0;
	iRet = VSNET_ClientStartMp4Capture(m_playhandle[nCuWinID],filename);
	if(iRet != 0)
		TRACE("StartRecord Error:%d\n",iRet);
	return iRet;
}

//////////////////////////////////////////////////////////////
int CYaanCamera::StopRecord(int nCuWinID)
{
	int iRet=0;
	iRet = VSNET_ClientStopMp4Capture(m_playhandle[nCuWinID]);
	if(iRet != 0)
		TRACE("StopRecord Error:%d\n",iRet);
	return iRet;
}

