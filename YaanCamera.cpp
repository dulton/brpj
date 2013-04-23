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
#include "Yaanlib/lauplaym4.h"

#pragma comment( lib, "Yaanlib/PlayerSDK.lib")
#pragma comment( lib, "Yaanlib/NetClient.lib")
#pragma comment( lib, "Yaanlib/NetPlaySDK.lib")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct RECORD_INFO_ST ConvrtThread[PMAX_NUM];	//录像转换线程

//////////////////////////////////////////////////////////////
CYaanCamera::CYaanCamera()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		m_bplay[i] = FALSE;
		m_brecord[i] = FALSE;
		m_playhandle[i] = i;
	}
}

//////////////////////////////////////////////////////////////
CYaanCamera::~CYaanCamera()
{
	for(int i=0;i<MAX_PLAYWIN;i++)
	{
		StopPlay(i);
		if(m_brecord[i] == TRUE)
			StopRecord(i);
	}
}

//////////////////////////////////////////////////////////////
void CYaanCamera::SDKinit()
{
	VSNET_ClientStartup(NULL,NULL); //call this function to initialize SDK;
}

//////////////////////////////////////////////////////////////
bool CYaanCamera::StartPlay(int nCuWinID,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd)
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
		return false;
	}
	else
	{
		m_bplay[nCuWinID] = TRUE;
		VSNET_ClientSetWnd(m_playhandle[nCuWinID],hWnd);
		VSNET_ClientMediaData(m_playhandle[nCuWinID],TRUE);
		VSNET_ClientStartViewEx(m_playhandle[nCuWinID]);
		return true;
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
//	char sRecFileName[MAX_PATH];
//	sprintf(sRecFileName, "%s%d%s", "C:\\test_", "1111", ".mp4");
//	BOOL bSuc = VSNET_ClientStartMp4Capture(s_ChanSock[g_GlobeEnvi.m_iSelWndSn],sRecFileName);
	int iRet=0;
	iRet = VSNET_ClientStartMp4Capture(m_playhandle[nCuWinID],filename);
	m_brecord[nCuWinID] = TRUE;
	return iRet;
}

//////////////////////////////////////////////////////////////
int CYaanCamera::StopRecord(int nCuWinID)
{
	int iRet=0;
	iRet = VSNET_ClientStopMp4Capture(m_playhandle[nCuWinID]);
	m_brecord[nCuWinID] = FALSE;
	return iRet;
}


//////////////////////////////////////////////////////////////
//以下是MP4转AVI接口
bool zogMP4toAVI(char *src,char *dst)
{
	long int conv_h=LCPLAYM4_StartAVIConvert(src,dst);

	if(conv_h >= 0)
	{
		while(LCPLAYM4_GetAVIConvertPos(conv_h) >= 0)
			Sleep(2000);

		LCPLAYM4_StopAVIConvert(conv_h);
		return true;
	}
	return false;
}

//转换线程
DWORD WINAPI ThreadPROC(LPVOID lpParameter)
{
	RECORD_INFO_ST *pRecord = (RECORD_INFO_ST*)lpParameter;

	pRecord->ThreadFlag=false;

	if(zogMP4toAVI(pRecord->MP4path,pRecord->AVIpath))
	{
		///写数据库	
	}
	pRecord->ThreadFlag=true;
	return 0;
}

void CYaanCamera::ConvertMp4ToAvi(struct RECORD_INFO_ST file)
{
	for(int i=0;i<PMAX_NUM;i++)
	{
		if(ConvrtThread[i].ThreadFlag)
		{
			if(NULL!=ConvrtThread[i].pthread)
				TerminateThread(ConvrtThread[i].pthread,0);
	
			memcpy(ConvrtThread[i].AVIpath,file.AVIpath,260);
			memcpy(ConvrtThread[i].MP4path,file.MP4path,260);
			memcpy(ConvrtThread[i].stime,file.stime,32);
			memcpy(ConvrtThread[i].etime,file.etime,32);
			memcpy(ConvrtThread[i].RunningNumber,file.RunningNumber,256);
			memcpy(ConvrtThread[i].tag,file.tag,256);
			memcpy(ConvrtThread[i].HmNum,file.HmNum,256);
			memcpy(ConvrtThread[i].Description,file.Description,2560);
			
			
			ConvrtThread[i].pthread=NULL;
			ConvrtThread[i].pthread=CreateThread(NULL,0,ThreadPROC,&ConvrtThread[i],0,NULL);
				
			//线程满了
			if(NULL==ConvrtThread[i].pthread)
				break;
			break;
		}
	}
}



