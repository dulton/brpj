// CYaAnSDKNew.cpp: implementation of the CVideoChannel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#if OPEN_YAAN_NEW_SDK

#include "YaAnSDKNew.h"
//////////////////////////////////
#include "CarDetect.h"
//////////////////////////////////

//////////////////////////////////
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;


#include "YaAnSDKNew/NET_NVR.h"

#pragma comment( lib, "YaAnSDKNew/NET_NVR.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 显示回调
//////////////////////////////////////////////////////////////////////
//zogna添加
void YUV2YUV420_NEW(BYTE *m_y,BYTE *m_u,BYTE *m_v,int width,int height,unsigned char* yuv420_image)
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
	CYaAnSDKNew *pchannel = (CYaAnSDKNew*)context;
	int curWndIndex = pchannel->GetWndIndex(hHandle);
	pchannel->LinkProcess(curWndIndex,lParam);
}

CYaAnSDKNew::CYaAnSDKNew()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_RealHandle[i] = i;
		m_LoginHandle[i] = -1;
		image[i]=(unsigned char *)calloc(1920*1080*3,sizeof(unsigned char));	//ZOGNA YUV420 BUFFER
	}
	m_ptzLoginHandle = -1;
	m_ptzRealHandle = MAX_DEVICE_NUM;
	
}

CYaAnSDKNew::~CYaAnSDKNew()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		
		StopPlay(i);
		free(image[i]); //ZOGNA YUV420 BUFFER
	}
	PtzStopPlay();

//	free(image); //ZOGNA YUV420 BUFFER
}

void CYaAnSDKNew::SDKInit()
{

}

bool CYaAnSDKNew::StartPlay(int screenNo,char *name,char *sip,WORD port,char *user,char *psw,HWND hWnd,int subtype)
{
	StopPlay(screenNo);


	m_LoginHandle[screenNo] = NET_NVR_Login(sip,port,user,psw);


	if (m_LoginHandle[screenNo] == -1 || m_LoginHandle[screenNo] == 0)
	{
		AfxMessageBox(_T("登陆失败 ：\n句柄获取失败\n可能输入的IP不存在"));
		return false;
	}
	else
	{	
		m_RealHandle[screenNo]  = NET_NVR_RealPlay((int)m_LoginHandle[screenNo],true,hWnd,0,0);
		if (m_RealHandle[screenNo] == -1 || m_RealHandle[screenNo] == 0)
		{
			AfxMessageBox(_T("播放失败 ：\n句柄获取失败\n可能输入的IP不存在"));
			return false;
		}
		return true;
	}
}

int CYaAnSDKNew::Capture(int screenNo,char *filename)
{
	int iRet = 0;
	char *p=strrchr(filename,'\\');
	p++;
	char dir[MAX_PATH]="";
	memcpy(dir,filename,strlen(filename)-strlen(p));

	iRet = NET_NVR_CapturePictureEX(m_RealHandle[screenNo],	dir,p);
	if(iRet != 0)
	{
		TRACE("Capture Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

int CYaAnSDKNew::StartRecord(int screenNo,char* filename)
{
	int iRet = 0;

	char *p=strrchr(filename,'\\');
	p++;
	char dir[MAX_PATH]="";
	memcpy(dir,filename,strlen(filename)-strlen(p));

	iRet = NET_NVR_SaveRealDataEX(m_RealHandle[screenNo],dir,p);
	if(iRet != 0)
	{
		TRACE("StartRecord Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

int CYaAnSDKNew::StopRecord(int screenNo)
{
	int iRet = 0;
	iRet = NET_NVR_StopSaveRealData(m_RealHandle[screenNo]);
	if(iRet != 0)
	{
		TRACE("StopRecord Error:%d\n",iRet);
		return 1;
	}
	return 0;
}

void CYaAnSDKNew::StopPlay(int screenNo)
{
	if(m_LoginHandle[screenNo] != -1 || m_LoginHandle[screenNo] != 0)
	{
		NET_NVR_StopRealPlay(m_RealHandle[screenNo]);
	
		NET_NVR_Logout(m_LoginHandle[screenNo]);
		m_LoginHandle[screenNo] = -1;
	}
		
#if OPEN_CARDETECT_CODE 	
	//停止识别
	if(false == DlgMain->DlgScreen.m_videoInfo[screenNo].enableDetect)
		DlgMain->DlgScreen.CarDetect[screenNo].Stop();
#endif

}


int CYaAnSDKNew::GetWndIndex(int LoginHandle)
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

void CYaAnSDKNew::LinkProcess(int screenNo,int lParam)
{
	if(lParam == 0)
	{
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"连接成功",FALSE);
		TRACE("连接成功\n");
//		VSNET_ClientShowcallback(m_LoginHandle[screenNo],m_ShowCallBack,&m_RealHandle[screenNo]);
	}
	else
	{
		DlgMain->ShowCameraMessage(DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0),"连接失败",FALSE);
		TRACE("连接失败\n");
//		StopPlay(screenNo);
	}
}

void CYaAnSDKNew::PtzControl(long lLoginID, int type, BOOL dwStop, int param)
{
	switch(type)
	{
		case PTZ_CONTROL_UP:
		case PTZ_CONTROL_DOWN:
			
			break;
		case PTZ_CONTROL_LEFT:
		case PTZ_CONTROL_RIGHT:
			break;
		case PTZ_CONTROL_ZOOM_ADD:
			break;
		case PTZ_CONTROL_ZOOM_SUB:
			break;
		case PTZ_CONTROL_FOCUS_ADD:
		case PTZ_CONTROL_FOCUS_SUB:
			break;
		case PTZ_CONTROL_IRIS_ADD:
		case PTZ_CONTROL_IRIS_SUB:
			break;
		case PTZ_CONTROL_POINT_MOVE:
		case PTZ_CONTROL_POINT_SET:
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
			break;
		default:
			break;
	}
}

bool CYaAnSDKNew::PtzStartPlay(char *sip,int nPort,char *user,char *psw,HWND hWnd)
{


	PtzStopPlay();

	m_ptzLoginHandle= NET_NVR_Login(sip,nPort,user,psw);


	if (m_ptzLoginHandle == -1 ||m_ptzLoginHandle == 0)
	{
		AfxMessageBox(_T("登陆失败 ：\n句柄获取失败\n可能输入的IP不存在"));
		return false;
	}
	else
	{	
		m_ptzRealHandle = NET_NVR_RealPlay((int)m_ptzLoginHandle,true,hWnd,0,0);
		if (m_ptzRealHandle == -1 || m_ptzRealHandle== 0)
		{
			AfxMessageBox(_T("播放失败 ：\n句柄获取失败\n可能输入的IP不存在"));
			return false;
		}
		return true;
	}


}

void CYaAnSDKNew::PtzStopPlay()
{
	if(m_ptzLoginHandle != -1)
	{
		NET_NVR_StopRealPlay(m_ptzRealHandle);

		NET_NVR_Logout(m_ptzLoginHandle);
		m_ptzLoginHandle = -1;
	}
}

#endif