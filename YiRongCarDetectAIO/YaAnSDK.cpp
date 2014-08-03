// CYaAnSDK.cpp: implementation of the CVideoChannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YaAnSDK.h"
//////////////////////////////////
#include "CarDetect.h"
//////////////////////////////////

//////////////////////////////////
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

#include "YaAnSDK/launet.h"
#include "YaAnSDK/megaplay.h"

#pragma comment( lib, "YaAnSDK/NetClient.lib")
#pragma comment( lib, "YaAnSDK/NetPlaySDK.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 显示回调
//////////////////////////////////////////////////////////////////////
//zogna添加
void YUV2YUV420(BYTE *m_y,BYTE *m_u,BYTE *m_v,int width,int height,unsigned char* yuv420_image)
{
	long int size;
	size=width*height;


	memcpy(&yuv420_image[0],m_y,size);
	memcpy(&yuv420_image[size],m_u,size/4);
	memcpy(&yuv420_image[size+size/4],m_v,size/4);
}


static void WINAPI m_ShowCallBack(BYTE *m_y,BYTE *m_u,BYTE *m_v,int stridey,int strideuv,int width,int height,void *context)
{
    int hHandle = *((int*)context);

	//车牌识别
#if OPEN_CARDETECT_CODE 	
	//启用识别
	if(DlgMain->DlgScreen.m_videoInfo[hHandle].enableDetect)
	{
		//拷贝数值
		DlgMain->DlgScreen.CarDetect[hHandle].m_playhandle=hHandle;
		
		DlgMain->DlgScreen.CarDetect[hHandle].alarmflag=
			DlgMain->DlgScreen.m_videoInfo[hHandle].enableAlarm;
		
		DlgMain->DlgScreen.CarDetect[hHandle].camid=
			DlgMain->DlgScreen.m_videoInfo[hHandle].camID;
		
		strcpy(DlgMain->DlgScreen.CarDetect[hHandle].cam_name,
			DlgMain->DlgScreen.m_videoInfo[hHandle].name.GetBuffer(0));
		
		strcpy(DlgMain->DlgScreen.CarDetect[hHandle].l_ipaddr,
			DlgMain->DlgScreen.m_videoInfo[hHandle].ip.GetBuffer(0));
		//////////////////////////
		YUV2YUV420(m_y,m_u,m_v,width,height,DlgMain->DlgScreen.m_video.m_yaAn.image[hHandle]);
		DlgMain->DlgScreen.CarDetect[hHandle].Start(LC_VIDEO_FORMAT_I420,\
			DlgMain->DlgScreen.m_video.m_yaAn.image[hHandle],width, height,width*height*3/2);
		
		DlgMain->DlgScreen.CarDetect[hHandle].Result();
	}
#endif


}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static void WINAPI s_messagecallback(LONG hHandle,int wParam,int lParam,void *context)
{
	CYaAnSDK *pchannel = (CYaAnSDK*)context;
	int curWndIndex = pchannel->GetWndIndex(hHandle);
	pchannel->LinkProcess(curWndIndex,lParam);
}

CYaAnSDK::CYaAnSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_RealHandle[i] = i;
		m_LoginHandle[i] = -1;
		image[i]=(unsigned char *)calloc(1920*1080*3,sizeof(unsigned char));	//ZOGNA YUV420 BUFFER
	}
	m_ptzLoginHandle = -1;
	m_ptzRealHandle = MAX_DEVICE_NUM;

	memset(&m_filehead,0,sizeof(ETI_FILE_HEAD));
	strcpy(m_filehead.m_filemask,ETI_FILEMASK);
	m_filehead.m_fileversion = 1;
	m_filehead.videofoucc = 0X58564944; //DIVX  0X34363248; // H264
	m_filehead.videoid = 0;
	m_filehead.m_width = 352;
	m_filehead.m_height = 288;
	m_filehead.m_audiotag = 0x55; //Mp3,0xFE;// G722
	m_filehead.m_haveaudio = 1;
	m_filehead.m_auchs = 1;
	m_filehead.m_aurates = 24;
	m_filehead.m_ausample = 16000;
	SDKInit();
    //设置流播放时，视频头信息  videofoucc : 0X34363248:H264(lc5000) ; 0X58564944 : DIVX(lc8000,lc9000,lc6000)
	//                          m_audiotag:  0xFE:G722(lc5000,lc6000); 0x55: mp3(lc8000,lc9000)
	//lc5000,lc6000的采样频率都为8000，不能修改
	//在视频服务器连接成功后可通过函数VSNET_ClientGetStreamInfo,获得这些参数
	
}

CYaAnSDK::~CYaAnSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		StopPlay(i);
		LC_PLAYM4_CloseStream(m_RealHandle[i]);
		free(image[i]); //ZOGNA YUV420 BUFFER
	}
	PtzStopPlay();

//	free(image); //ZOGNA YUV420 BUFFER
}

void CYaAnSDK::SDKInit()
{
//	LC_PLAYM4_Initial((long)DlgMain->m_hWnd);
//	LC_PLAYM4_Initial(NULL);
	VSNET_ClientStartup(NULL,NULL); //call this function to initialize SDK;
	VSNET_ClientWaitTime(10,1000);
}

bool CYaAnSDK::StartPlay(int screenNo,char *name,char *sip,WORD port,int channel,char *user,char *psw,HWND hWnd,int subtype)
{
	StopPlay(screenNo);

	CHANNEL_CLIENTINFO m_chinfo;

	m_chinfo.m_hChMsgWnd  = NULL;
	m_chinfo.m_nChmsgid   = 0;
	m_chinfo.m_sername    = name;
	m_chinfo.m_username   = user;
	m_chinfo.m_password   = psw;
	m_chinfo.m_playstart  = TRUE;
	m_chinfo.m_tranType   = 3;
	m_chinfo.m_useoverlay = FALSE;
	m_chinfo.nColorKey    = RGB(192,0,192);
	m_chinfo.m_ch         = channel;
	m_chinfo.m_buffnum    = 20;
	m_chinfo.m_hVideohWnd = hWnd;
	m_chinfo.context      = this;
	m_chinfo.m_messagecallback = s_messagecallback;

	m_LoginHandle[screenNo] = VSNET_ClientStart(sip,&m_chinfo,port,subtype);
	if (m_LoginHandle[screenNo] == -1)
	{
		AfxMessageBox(_T("启动通道失败 ：\n句柄获取失败\n可能输入的IP不存在"));
		return false;
	}
	else
	{
		VSNET_ClientSetWnd(m_LoginHandle[screenNo],hWnd);
		VSNET_ClientMediaData(m_LoginHandle[screenNo],TRUE);
		VSNET_ClientStartViewEx(m_LoginHandle[screenNo]);
		return true;
	}
}

int CYaAnSDK::Capture(int screenNo,char *filename)
{
	int iRet = 0;
	iRet = VSNET_ClientCapturePicture(m_LoginHandle[screenNo],filename);
	if(iRet != TRUE)
	{
		TRACE("Capture Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

int CYaAnSDK::StartRecord(int screenNo,LPCSTR filename)
{
	int iRet = 0;
	iRet = VSNET_ClientStartMp4Capture(m_LoginHandle[screenNo],filename);
	if(iRet != TRUE)
	{
		TRACE("StartRecord Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

int CYaAnSDK::StopRecord(int screenNo)
{
	int iRet = 0;
	iRet = VSNET_ClientStopMp4Capture(m_LoginHandle[screenNo]);
	if(iRet != TRUE)
	{
		TRACE("StopRecord Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

void CYaAnSDK::StopPlay(int screenNo)
{
	if(m_LoginHandle[screenNo] != -1)
	{
		VSNET_ClientStop(m_LoginHandle[screenNo]);
		m_LoginHandle[screenNo] = -1;
	}
		
#if OPEN_CARDETECT_CODE 	
	//停止识别
	if(false == DlgMain->DlgScreen.m_videoInfo[screenNo].enableDetect)
		DlgMain->DlgScreen.CarDetect[screenNo].Stop();
#endif

}


int CYaAnSDK::GetWndIndex(int LoginHandle)
{
	int Wndindex=0;
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_LoginHandle[i] == LoginHandle)
			return Wndindex;
		Wndindex++;
	}
	return -1;
}

void CYaAnSDK::LinkProcess(int screenNo,int lParam)
{
	if(lParam == 0)
	{
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"连接成功",FALSE);
		TRACE("连接成功\n");
		VSNET_ClientShowcallback(m_LoginHandle[screenNo],m_ShowCallBack,&m_RealHandle[screenNo]);
	}
	else
	{
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"连接失败",FALSE);
		TRACE("连接失败\n");
//		StopPlay(screenNo);
	}
}

void CYaAnSDK::PtzControl(long lLoginID, int type, BOOL dwStop, int param)
{
	switch(type)
	{
		case PTZ_CONTROL_UP:
		case PTZ_CONTROL_DOWN:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type+2,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+16,0);
			break;
		case PTZ_CONTROL_LEFT:
		case PTZ_CONTROL_RIGHT:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type-2,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+12,0);
			break;
		case PTZ_CONTROL_ZOOM_ADD:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type+5,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+19,0);
			break;
		case PTZ_CONTROL_ZOOM_SUB:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type+3,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+17,0);
			break;
		case PTZ_CONTROL_FOCUS_ADD:
		case PTZ_CONTROL_FOCUS_SUB:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+14,0);
			break;
		case PTZ_CONTROL_IRIS_ADD:
		case PTZ_CONTROL_IRIS_SUB:
			if(dwStop == 0)
				VSNET_ClientPTZCtrl(lLoginID,type-4,param);
			else
				VSNET_ClientPTZCtrl(lLoginID,type+10,0);
			break;
		case PTZ_CONTROL_POINT_MOVE:
		case PTZ_CONTROL_POINT_SET:
			VSNET_ClientPTZCtrl(lLoginID,type,param);
			break;
		case PTZ_CONTROL_UPLEFT:
		case PTZ_CONTROL_UPRIGHT:
		case PTZ_CONTROL_DOWNLEFT:
		case PTZ_CONTROL_DOWNRIGHT:
			break;
		case PTZ_CONTROL_SPEED_ADD:
			break;
		case PTZ_CONTROL_SPEED_SUB:
			break;
		case PTZ_CONTROL_AUTO:
			VSNET_ClientPTZCtrl(lLoginID,12,1);
			break;
		default:
			break;
	}
}

bool CYaAnSDK::PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd)
{
	CHANNEL_CLIENTINFO pVSChann;

	pVSChann.m_hChMsgWnd  = NULL;
	pVSChann.m_nChmsgid   = 0;
	pVSChann.m_sername    = "摄像机";
	pVSChann.m_username   = user;
	pVSChann.m_password   = psw;
	pVSChann.m_playstart  = TRUE;
	pVSChann.m_tranType   = 3;
	pVSChann.m_useoverlay = FALSE;
	pVSChann.nColorKey    = RGB(192,0,192);
	pVSChann.m_ch         = channel;
	pVSChann.m_buffnum    = 20;
	pVSChann.m_hVideohWnd = hWnd;
	pVSChann.context      = NULL;
	pVSChann.m_messagecallback = NULL;

	PtzStopPlay();

	m_ptzLoginHandle = VSNET_ClientStart(sip,&pVSChann,3000);
	if (m_ptzLoginHandle == -1)
	{
		AfxMessageBox(_T("启动通道失败 ：\n句柄获取失败\n可能输入的IP不存在"));
		return false;
	}
	else
	{
		VSNET_ClientSetWnd(m_ptzLoginHandle,hWnd);
		VSNET_ClientMediaData(m_ptzLoginHandle,TRUE);
		VSNET_ClientStartViewEx(m_ptzLoginHandle);
		return true;
	}
}

void CYaAnSDK::PtzStopPlay()
{
	if(m_ptzLoginHandle != -1)
	{
		VSNET_ClientStop(m_ptzLoginHandle);
		m_ptzLoginHandle = -1;
	}
}
