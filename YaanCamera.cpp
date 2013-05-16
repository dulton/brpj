// VideoChannel.cpp: implementation of the CYaanCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YaanCamera.h"
#include "BarcodeRecord.h"
//////////////////////////////////////////////////////////////
#include "BarcodeRecordDlg.h"
extern CBarcodeRecordDlg *pCMainDlg;
////////////////////////////////////
#include "SqliteOperate.h"
extern CSqliteOperate SQLiteIO;

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


//////////////////////////////////////////////////////////////
CYaanCamera::CYaanCamera()
{
	int i;
	for(i=0;i<MAX_PLAYWIN;i++)
	{
		m_bplay[i] = FALSE;
		m_brecord[i] = FALSE;
		m_playhandle[i] = i;
		memset(&record[i],0,sizeof(struct RECORD_INFO_ST));
	}
	for(i=0;i<PMAX_NUM;i++)
	{
		memset(&ConvrtThread[i],0,sizeof(struct RECORD_INFO_ST));
		ConvrtThread[i].ThreadFlag=true;
		ConvrtThread[i].pthread=NULL;
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
		//LONG ret = VSNET_ClientGetState(m_playhandle[nCuWinID]);
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
	int iRet=0;
	if(m_bplay[nCuWinID] == FALSE)
	{
		return 0;
	}

	iRet = VSNET_ClientStartMp4Capture(m_playhandle[nCuWinID],filename);
	if(iRet != 0)
	{
		m_brecord[nCuWinID] = TRUE;
	}
	return iRet;
}

//////////////////////////////////////////////////////////////
int CYaanCamera::StopRecord(int nCuWinID)
{
	int iRet=0;
	if(m_brecord[nCuWinID] == FALSE)
		return FALSE;

	iRet = VSNET_ClientStopMp4Capture(m_playhandle[nCuWinID]);
	m_brecord[nCuWinID] = FALSE;

	CTime nowtime=CTime::GetTickCount();
	sprintf(record[nCuWinID].etime,"%04d-%02d-%02d %02d:%02d:%02d", \
							nowtime.GetYear(), \
							nowtime.GetMonth(), \
							nowtime.GetDay(), \
							nowtime.GetHour(), \
							nowtime.GetMinute(), \
							nowtime.GetSecond());
	ConvertMp4ToAvi(record[nCuWinID]);

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

void printffile(char *stra,char *strb,int i)
{
	/*
	FILE *fp;
	fp=fopen("aaaa.txt","a+");
	if(fp)
	{
		fprintf(fp,"%s:%s %d\n",stra,strb,i);
		fclose(fp);
	}
	*/
}
//转换线程
DWORD WINAPI ThreadPROC(LPVOID lpParameter)
{
	RECORD_INFO_ST *pRecord = (RECORD_INFO_ST*)lpParameter;

	pRecord->ThreadFlag=false;
	printffile("ThreadPROC","in",0);
	if(zogMP4toAVI(pRecord->MP4path,pRecord->AVIpath))
	{
		SQLiteIO.Video_Add(pRecord->RunningNumber,
						   pRecord->tag,
						   pRecord->HmNum,
						   pRecord->Description,
						   pRecord->stime,
						   pRecord->etime,
						   pRecord->AVIpath,
						   pRecord->size);
		//删除MP4文件
		DeleteFile(pRecord->MP4path);
	}
	printffile("ThreadPROC",pRecord->MP4path,0);
	pRecord->ThreadFlag=true;
	return 0;
}

void CYaanCamera::ConvertMp4ToAvi(struct RECORD_INFO_ST file)
{
	Sleep(1000);
	for(int i=0;i<PMAX_NUM;i++)
	{
		if(ConvrtThread[i].ThreadFlag)
		{
			if(NULL!=ConvrtThread[i].pthread)
			{	TerminateThread(ConvrtThread[i].pthread,0);
				printffile("TerminateThread","stop",i);
			}


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
							printffile("CreateThread","c",i);
			//线程满了
			if(NULL==ConvrtThread[i].pthread)
			{		printffile("CreateThread","break",i);
				break;
			}
			break;
		}
	}
}

//等待线程释放
void CYaanCamera::WaitConvrtThread()
{
	for(int i=0;i<PMAX_NUM;i++)
	{
		while(ConvrtThread[i].ThreadFlag == false)
		{
			Sleep(1000);
		}
	}
}



