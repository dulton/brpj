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
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static void WINAPI s_messagecallback(LONG hHandle,int wParam,int lParam,void *context)
{
	CYaAnSDK *pchannel = (CYaAnSDK*)context;
	int curWndIndex = pchannel->GetWndIndex(hHandle);
	pchannel->LinkProcess(curWndIndex,lParam);
}

/*Note:In video and audio callback function,you must return as soon as possible.
otherwise,you will lost frames or connection broken. */
/*
pbuff:       MPEG4 video data;have vol header,just send this data to decoder or save to disk.
headsize:    MPEG4 Video frame head length;
datasize:    MPEG4 video data length;
timetick:    MPEG4 time tick(ms)
biskeyframe: TRUE: key frame(I frame), FALSE : other
*/
static void WINAPI m_nomalvideo(char *pbuff,int headsize,int datasize,int timetick,int biskeyframe,void *context)
{
	int iRet;
	int phandle = (*(int*)context);

    iRet = LC_PLAYM4_InputData(phandle,(long*)(pbuff - headsize),headsize + datasize);
	if(iRet != 0)
	{
		TRACE("send video data error\n",iRet);
	}
}
/*
pbuff:       MP3 audio data;just send this data to decoder or save to disk.
headsize:    MP3 audio frame head length;
datasize:    MP3 audio data length;
timetick:    MP3 time tick(ms)
biskeyframe: Not used.
*/
static void WINAPI m_nomalaudio(char *pbuff,int headsize,int datasize,int timetick,int biskeyframe,void *context)
{
	int iRet;
	int phandle = (*(int*)context);

	iRet = LC_PLAYM4_InputData(phandle,(long*)(pbuff - headsize),headsize + datasize);
	if(iRet != 0)
	{
		TRACE("send audio data error\n",iRet);
	}
}
//zogna添加
void YUV2YUV420(VIDEO_FRAMEINFO *yuv_image,unsigned char* yuv420_image)
{
	long int size;
	size=yuv_image->width*yuv_image->height;

	memcpy(&yuv420_image[0],yuv_image->pY,size);
	memcpy(&yuv420_image[size],yuv_image->pU,size/4);
	memcpy(&yuv420_image[size+size/4],yuv_image->pV,size/4);
}


/*Decoder callback function*/
static long  CALLBACK s_DecodeCallBack(long hHandle,long frametype,long *frameinfo,long lParam)
{
    CYaAnSDK *pchannel = (CYaAnSDK*)lParam;
    if(frametype == FRAMETYPE_VIDEO)
    {
        VIDEO_FRAMEINFO *pframeinfo = (VIDEO_FRAMEINFO*)frameinfo;
		/*
        TRACE("video pchannel->handle:%d,handle:%d,timetick:%d,width:%d,height:%d,stridey:%d,strideuv:%d\n",
            pchannel->m_RealHandle,hHandle,pframeinfo->timetick,pframeinfo->width,pframeinfo->height,
            pframeinfo->stridey,pframeinfo->strideuv);
			*/

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
			YUV2YUV420(pframeinfo,pchannel->image[hHandle]);
			DlgMain->DlgScreen.CarDetect[hHandle].Start(LC_VIDEO_FORMAT_I420,\
				pchannel->image[hHandle],pframeinfo->width,pframeinfo->height,pframeinfo->width*pframeinfo->height*3/2);
			
			DlgMain->DlgScreen.CarDetect[hHandle].Result();
		}
#endif

    }
    else if(frametype == FRAMETYPE_AUDIO)
    {
        AUDIO_FRAMEINFO *pframeinfo = (AUDIO_FRAMEINFO*)frameinfo;
		/*
        TRACE("audio pchannel->handle:%d,handle:%d,timetick:%d,channel:%d,samplerate:%d,bits:%d,size:%d\n",
            pchannel->m_RealHandle,hHandle,pframeinfo->timetick,pframeinfo->nChannels,
            pframeinfo->nSamplesPerSec,pframeinfo->wBitsPerSample,pframeinfo->size);
			*/
    }
    return 0;
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


}

void CYaAnSDK::SDKInit()
{
//	LC_PLAYM4_Initial((long)DlgMain->m_hWnd);
	LC_PLAYM4_Initial(NULL);
	VSNET_ClientStartup(NULL,NULL); //call this function to initialize SDK;
}

bool CYaAnSDK::StartPlay(int screenNo,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd,int subtype)
{
	StopPlay(screenNo);
	LC_PLAYM4_CloseStream(m_RealHandle[screenNo]);
	
	int iRet;
	iRet = LC_PLAYM4_OpenStream(m_RealHandle[screenNo],(long*)&m_filehead,sizeof(ETI_FILE_HEAD));
	if(iRet != 0)
	{
		TRACE("Open File or stream error:%d\n",iRet);
		return false;
	}
    LC_PLAYM4_SetDecodeCallBack(m_RealHandle[screenNo],s_DecodeCallBack,(long)this);

	CHANNEL_CLIENTINFO m_chinfo;

	m_chinfo.m_buffnum    = 50;
	m_chinfo.m_ch         = 0;
	m_chinfo.m_hChMsgWnd  = NULL;
	m_chinfo.m_hVideohWnd = NULL;
	m_chinfo.m_nChmsgid   = NULL;
	m_chinfo.m_password   = psw;
	m_chinfo.m_playstart  = FALSE;
	m_chinfo.m_sername    = name;
	m_chinfo.m_tranType   = 3;
	m_chinfo.m_useoverlay = FALSE;
	m_chinfo.m_username   = user;
	m_chinfo.nColorKey    = RGB(255,0,255);
	m_chinfo.context      = this;
	m_chinfo.m_messagecallback = s_messagecallback;

	iRet = LC_PLAYM4_Play(m_RealHandle[screenNo],(long)hWnd);
	if(iRet != 0)
	{
		StopPlay(screenNo);
		DlgMain->ShowCameraMessage(name,"LC_PLAYM4_Play Stream Error",FALSE);
		TRACE("LC_PLAYM4_Play Stream Error:%d\n",iRet);
		return false;
	}

	m_LoginHandle[screenNo] = VSNET_ClientStart(sip,&m_chinfo,port);
	if(m_LoginHandle[screenNo] == -1)
	{
		StopPlay(screenNo);
		DlgMain->ShowCameraMessage(name,"VSNET_ClientStart Error",FALSE);
		TRACE("VSNET_ClientStart Error:%d\n",iRet);
		return false;
	}

	return true;
}

int CYaAnSDK::Capture(int screenNo,char *filename)
{
	int iRet;
	iRet = LC_PLAYM4_CapPic(m_RealHandle[screenNo],filename);
	if(iRet != 0)
		TRACE("Capture Error:%d\n",iRet);
	return iRet;
}

int CYaAnSDK::StartRecord(int screenNo,LPCSTR filename)
{
	int iRet;
	iRet = LC_PLAYM4_StartMp4Capture(m_RealHandle[screenNo],filename);
	if(iRet != 0)
		TRACE("StartRecord Error:%d\n",iRet);
	return iRet;
}

int CYaAnSDK::StopRecord(int screenNo)
{
	int iRet;
	iRet = LC_PLAYM4_StopMp4Capture(m_RealHandle[screenNo]);
	if(iRet != 0)
		TRACE("StopRecord Error:%d\n",iRet);
	return iRet;
}

void CYaAnSDK::StopPlay(int screenNo)
{
	int iRet;

	if(m_LoginHandle[screenNo] != -1)
	{
		VSNET_ClientStop(m_LoginHandle[screenNo]);
		m_LoginHandle[screenNo] = -1;
	}

	iRet = LC_PLAYM4_Stop(m_RealHandle[screenNo]);
	if(iRet != 0)
	{
		TRACE("LC_PLAYM4_Stop Stream Error:%d\n",iRet);
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
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"Connect successful",FALSE);
		TRACE("Connect successful\n");
		VSNET_ClientStartNomalCap(m_LoginHandle[screenNo],m_nomalvideo,&m_RealHandle[screenNo],NULL,&m_RealHandle[screenNo]);
	}
	else
	{
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"Connect failed",FALSE);
		TRACE("Connect failed\n");
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
		default:
			break;
	}
}

bool CYaAnSDK::PtzStartPlay(char *sip,int nPort,char *user,char *psw,HWND hWnd)
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
	pVSChann.m_ch         = 0;
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

void CYaAnSDK::RefrenshWnd()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_LoginHandle[i] != -1)
		{
			VSNET_ClientRefrenshWnd(m_LoginHandle[i]);
		}
	}
}
