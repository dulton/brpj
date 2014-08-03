
#include "stdafx.h"
#include "HaikangSDK.h"

//////////////////////////////////
#include "CarDetect.h"
//////////////////////////////////

//////////////////////////////////
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;

//#if (ALLTAB_CAMERA_INC_TYPE == CAMERA_INC_HAIKANG)

#include "haikangSDK/HCNetSDK.h"
#include "haikangSDK/plaympeg4.h"

#pragma comment(lib, "haikangSDK/HCNetSDK.lib")
#pragma comment(lib, "haikangSDK/PlayCtrl.lib")

/*********************************************************
  Function:	DisplayCBFun
  Desc:		the call back function to snatch the bmp pictrue
  Input:	nPort,port;pBuf,pic buffer;nSize,pic size;nWidth,pic width;nHeight,pic height;nType,type;
  Output:	none
  Return:	none
**********************************************************/
void CALLBACK RemoteDisplayCBFun(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved)
{
	int screenNo = DlgMain->DlgScreen.m_video.m_haikang.GetPortWndindex(nPort);
	if(screenNo<0)
		return;

	if( ! DlgMain->DlgScreen.m_video.m_haikang.CapturePath[screenNo].IsEmpty())
	{
		PlayM4_ConvertToBmpFile(pBuf, nSize, nWidth, nHeight, nType, 
			DlgMain->DlgScreen.m_video.m_haikang.CapturePath[screenNo].GetBuffer(0));

		DlgMain->DlgScreen.m_video.m_haikang.CapturePath[screenNo]="";
	}

	//在这做识别
	
	//车牌识别
#if OPEN_CARDETECT_CODE 	

	//启用识别
	if(DlgMain->DlgScreen.m_videoInfo[screenNo].enableDetect)
	{
		//拷贝数值
		DlgMain->DlgScreen.CarDetect[screenNo].m_playhandle=screenNo;
		
		DlgMain->DlgScreen.CarDetect[screenNo].alarmflag=
			DlgMain->DlgScreen.m_videoInfo[screenNo].enableAlarm;
		
		DlgMain->DlgScreen.CarDetect[screenNo].camid=
			DlgMain->DlgScreen.m_videoInfo[screenNo].camID;
		
		strcpy(DlgMain->DlgScreen.CarDetect[screenNo].cam_name,
			DlgMain->DlgScreen.m_videoInfo[screenNo].name.GetBuffer(0));
		
		strcpy(DlgMain->DlgScreen.CarDetect[screenNo].l_ipaddr,
			DlgMain->DlgScreen.m_videoInfo[screenNo].ip.GetBuffer(0));
		//颜色LC_VIDEO_FORMAT_YV12 与颜色LC_VIDEO_FORMAT_I420 相反
		DlgMain->DlgScreen.CarDetect[screenNo].Start(LC_VIDEO_FORMAT_YV12,\
			(unsigned char *)pBuf,nWidth,nHeight,nSize);
		
		DlgMain->DlgScreen.CarDetect[screenNo].Result();
	}
	
#endif


}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,void* dwUser)
{
	CHaikangSDK *HaikangSDK = (CHaikangSDK *)dwUser;
	int screenNo = DlgMain->DlgScreen.m_video.m_haikang.GetHandleWindID(lRealHandle);
	LONG lPort = HaikangSDK->m_lPort[screenNo];
	CWnd* pWnd=NULL;
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //系统头
		
		if (lPort < 0)
		{
			if (PlayM4_GetPort(&lPort))  //获取播放库未使用的通道号
			{
				DlgMain->DlgScreen.m_video.m_haikang.m_lPort[screenNo] = lPort;
			}
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				break;
			}

			if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1920*1080)) //打开流接口
			{
				break;
			}

			if (!PlayM4_SetDisplayCallBack(lPort, RemoteDisplayCBFun))
			{
				break;
			}
#if DISPLAY_PREVIEW
			pWnd= DlgMain->DlgScreen.m_screenPannel.GetPage(screenNo);
			if (!pWnd)
			{
				return;
			}
			if (!PlayM4_Play(lPort, pWnd->m_hWnd)) //播放开始
			{
				break;
			}
#else
			if (!PlayM4_Play(lPort,NULL)) //播放开始
			{
				break;
			}
#endif
		}
	case NET_DVR_STREAMDATA:   //码流数据
		if (dwBufSize > 0 && lPort != -1)
		{
			if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
			{
				break;
			} 
		}
	}
}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = {0};
	switch(dwType) 
	{
		case EXCEPTION_RECONNECT:    //预览时重连
			break;
		default:
			break;
	}
}


CHaikangSDK::CHaikangSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		m_LoginHandle[i] = -1;
		m_RealHandle[i] = -1;
		m_lPort[i] = -1;
	}
	m_ptzRealHandle = -1;
	m_ptzLoginHandle = -1;
	SDKInit();
}

CHaikangSDK::~CHaikangSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
		StopPlay(i);

	PtzStopPlay();
	NET_DVR_Cleanup();
}

void CHaikangSDK::SDKInit()
{
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 5);
	NET_DVR_SetReconnect(10000, true);
}

bool CHaikangSDK::StartPlay(int screenNo,char *name,char *sip,int nPort,int channel,
							char *user,char *psw,HWND hWnd,int subtype)
{
	if(m_LoginHandle[screenNo] != -1)
	{
		StopPlay(screenNo);
	}
	//---------------------------------------
	// 注册设备
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_LoginHandle[screenNo] = NET_DVR_Login_V30(sip, nPort, user, psw, &struDeviceInfo);
	if (m_LoginHandle[screenNo] < 0)
	{
		DlgMain->ShowCameraMessage(name,"Login error!",FALSE);
		printf("Login error, %d\n", NET_DVR_GetLastError());
		//NET_DVR_Cleanup();
		return false;
	}

	//---------------------------------------
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	//启动预览并设置回调数据流
	NET_DVR_CLIENTINFO ClientInfo = {0};
	//需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.hPlayWnd = NULL;
	ClientInfo.lChannel = channel;		//预览通道号
	//最高位(31)为0表示主码流，为1表示子码流0～30位表示连接方式：0－TCP方式；1－UDP方式；2－多播方式；3－RTP方式;
	if(subtype == 0)
		ClientInfo.lLinkMode = 0;
	else
		ClientInfo.lLinkMode = 0x80000000;
	ClientInfo.sMultiCastIP = NULL;	//多播地址，需要多播预览时配置
  
	BOOL bPreviewBlock = false;       //请求码流过程是否阻塞，0：否，1：是
	m_RealHandle[screenNo] = NET_DVR_RealPlay_V30(m_LoginHandle[screenNo], &ClientInfo, g_RealDataCallBack_V30, this, 0);
	if (m_RealHandle[screenNo] < 0)
	{
		DlgMain->ShowCameraMessage(name,"Play error!",FALSE);
		printf("NET_DVR_RealPlay_V30 error\n");
		NET_DVR_Logout(m_LoginHandle[screenNo]);
	//	NET_DVR_Cleanup();
		m_LoginHandle[screenNo] = -1;
		return false;
	}
	return true;
}

void CHaikangSDK::StopPlay(int screenNo)
{
	//关闭预览
	NET_DVR_StopRealPlay(m_RealHandle[screenNo]);
	//注销用户
	NET_DVR_Logout_V30(m_LoginHandle[screenNo]);

#if OPEN_CARDETECT_CODE 	
	//停止识别
	if(false == DlgMain->DlgScreen.m_videoInfo[screenNo].enableDetect)
		DlgMain->DlgScreen.CarDetect[screenNo].Stop();
#endif

}

void CHaikangSDK::PtzControl(LONG lRealHandle, int type, BOOL dwStop, int param)
{
	BOOL ret;
	if (dwStop)
	{
		if(!m_bPTZCtrl)
		{
			goto exitPTZCtrl;
		}
	}
	if(lRealHandle == 0)
	{
		goto exitPTZCtrl;
	}

	switch(type)
	{
		case PTZ_CONTROL_UP:
		case PTZ_CONTROL_DOWN:
		case PTZ_CONTROL_LEFT:
		case PTZ_CONTROL_RIGHT:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, type+21, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_ZOOM_ADD:
		case PTZ_CONTROL_ZOOM_SUB:
		case PTZ_CONTROL_FOCUS_ADD:
		case PTZ_CONTROL_FOCUS_SUB:
		case PTZ_CONTROL_IRIS_ADD:
		case PTZ_CONTROL_IRIS_SUB:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, type+7, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_POINT_MOVE:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, GOTO_PRESET, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_POINT_SET:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, SET_PRESET, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_UPLEFT:
		case PTZ_CONTROL_UPRIGHT:
		case PTZ_CONTROL_DOWNLEFT:
		case PTZ_CONTROL_DOWNRIGHT:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, type+16, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
		case PTZ_CONTROL_SPEED_ADD:
			break;
		case PTZ_CONTROL_SPEED_SUB:
			break;
		case PTZ_CONTROL_AUTO:
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle, PAN_AUTO, dwStop, param);
			m_bPTZCtrl = !dwStop;
			break;
	}
	if(!ret)
	{
		//MessageBox(ConvertString(MSG_DEMODLG_PTZCTRLFAILED));
		goto exitPTZCtrl;
	}
	return;

exitPTZCtrl:
	m_bPTZCtrl = FALSE;
	return;
}

bool CHaikangSDK::PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd)
{
	//---------------------------------------
	// 注册设备
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	m_ptzLoginHandle = NET_DVR_Login_V30(sip, nPort, user, psw, &struDeviceInfo);
	if (m_ptzLoginHandle < 0)
	{
		return false;
	}

	//---------------------------------------
	//启动预览并设置回调数据流
	NET_DVR_CLIENTINFO ClientInfo = {0};
	//需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	ClientInfo.hPlayWnd = hWnd;
	ClientInfo.lChannel = channel;		//预览通道号
	//主码流
	ClientInfo.lLinkMode = 0;
	ClientInfo.sMultiCastIP = NULL;	//多播地址，需要多播预览时配置
	BOOL bPreviewBlock = false;

	m_ptzRealHandle = NET_DVR_RealPlay_V30(m_ptzLoginHandle, &ClientInfo, NULL, this, 0);
	if (m_ptzRealHandle < 0)
	{
		NET_DVR_Logout(m_ptzLoginHandle);
		m_ptzLoginHandle = -1;
		return false;
	}
	return true;
}

void CHaikangSDK::PtzStopPlay()
{
	//关闭预览
	if(m_ptzRealHandle != -1)
	{
		NET_DVR_StopRealPlay(m_ptzRealHandle);
	}
	//注销用户
	if(m_ptzLoginHandle != -1)
	{
		NET_DVR_Logout_V30(m_ptzLoginHandle);
	}
}

void CHaikangSDK::Capture(int screenNo,char *filename)
{
	/*
	if (NET_DVR_CapturePicture(m_RealHandle[screenNo], filename))
	{
		//DlgMain->ShowCameraMessage(name,"抓图成功!",flag);
	}
	else
	{
		//DlgMain->ShowCameraMessage(name,"抓图失败!",flag);
	}
	*/
	CapturePath[screenNo]=filename;
	Sleep(150);

}

int CHaikangSDK::GetPortWndindex(long lport)
{
	int Wndindex=0;
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_lPort[i] == lport)
			return Wndindex;
		Wndindex++;
	}
	return -1;
}

//根据播放句柄获取窗口ID
int CHaikangSDK::GetHandleWindID(int RealHandle)
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		if(m_RealHandle[i] == RealHandle)
		{
			return i;
		}
	}
	return -1;
}

int CHaikangSDK::StartRecord(int screenNo,char *filename)
{
	int iRet;
	iRet = NET_DVR_SaveRealData(m_RealHandle[screenNo],filename);
	if(iRet == 0)
	{
		TRACE("StartRecord Error:%d\n",iRet);
		return 1;
	}
	else
	{
		return 0;
	}
}

int CHaikangSDK::StopRecord(int screenNo)
{
	int iRet;
	iRet = NET_DVR_StopSaveRealData(m_RealHandle[screenNo]);
	if(iRet == 0)
	{
		TRACE("StopRecord Error:%d\n",iRet);
		return 1;
	}
	else
	{
		return 0;
	}
}

//#endif
