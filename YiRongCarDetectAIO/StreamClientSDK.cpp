
#include "stdafx.h"

#if OPEN_STREAM_CLIENT_SDK
#include "StreamClientSDK.h"

//////////////////////////////////
#include "CarDetect.h"
//////////////////////////////////

//////////////////////////////////
#include "YiRongCarDetectAIO.h"
#include "YiRongCarDetectAIODlg.h"
extern CYiRongCarDetectAIODlg *DlgMain;
#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "IO.h"
extern IO OracleIO;

#include "StreamClientSDK/StreamClient.h"
#pragma comment(lib, "StreamClientSDK/StreamClient.lib")


#include "DB33PlayCtrlSDK/PlayM4.h"
#pragma comment(lib, "DB33PlayCtrlSDK/DB33PlayCtrl.lib")


void CALLBACK StreamClientHikanCBFun(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved)
{
	
	int screenNo = DlgMain->DlgScreen.m_video.m_StreamClient.GetPortWndindex(nPort);
	if(screenNo<0)
		return;

	if( ! DlgMain->DlgScreen.m_video.m_StreamClient.CapturePath[screenNo].IsEmpty())
	{
		DB33_PlayM4_ConvertToJpegFile(pBuf, nSize, nWidth, nHeight, nType, 
			DlgMain->DlgScreen.m_video.m_StreamClient.CapturePath[screenNo].GetBuffer(0));

		DlgMain->DlgScreen.m_video.m_StreamClient.CapturePath[screenNo]="";
	}


	//在这做识别

		//OracleIO.LOG_AddNewSystemLog("admin","c");
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

		if(DlgMain->DlgScreen.m_videoInfo[screenNo].ip.GetLength() >1)
		{
			strcpy(DlgMain->DlgScreen.CarDetect[screenNo].l_ipaddr,
				DlgMain->DlgScreen.m_videoInfo[screenNo].ip.GetBuffer(0));
		}
		else
			strcpy(DlgMain->DlgScreen.CarDetect[screenNo].l_ipaddr,"0.0.0.0");


		//颜色LC_VIDEO_FORMAT_YV12 与颜色LC_VIDEO_FORMAT_I420 相反
		DlgMain->DlgScreen.CarDetect[screenNo].Start(LC_VIDEO_FORMAT_YV12,\
			(unsigned char *)pBuf,nWidth,nHeight,nSize);

		DlgMain->DlgScreen.CarDetect[screenNo].Result();
	}

#endif


}

int  StreamClientHikanCBinitFun(int sessionhandle, void* userdata, int datatype, void* pdata,
								int ilen) 
{
	/*

	FILE *outfile = fopen("stream.txt", "a+");
	if(NULL!= outfile)
	{
	fwrite(pdata, ilen, 1, outfile);
	fclose(outfile);
	}

	*/

	CStreamClientSDK *StreamClientSDK = (CStreamClientSDK *)userdata;

	int screenNo = DlgMain->DlgScreen.m_video.m_StreamClient.GetHandleWindID(sessionhandle);
	LONG lPort = StreamClientSDK->m_lPort[screenNo];

	CWnd* pWnd=NULL;


	switch  (datatype) 
	{ 
	case STREAM_HEAD:   // 码流头数据 

		if (lPort < 0)
		{
			if (DB33_PlayM4_GetPort(&lPort))  //获取播放库未使用的通道号
			{
				DlgMain->DlgScreen.m_video.m_StreamClient.m_lPort[screenNo] = lPort;
			}
		}
		//m_iPort = lPort; //第一次回调的是系统头，将获取的播放库port号赋值给全局port，下次回调数据时即使用此port号播放
		if (ilen > 0)
		{
			if (!DB33_PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))  //设置实时流播放模式
			{
				DB33_PlayM4_FreePort(lPort);
				break;
			}
			if (!DB33_PlayM4_OpenStream(lPort, (PBYTE)pdata, ilen, 1920*1080*3)) //打开流接口
			{
				DB33_PlayM4_FreePort(lPort);
				break;
			}

			if (!DB33_PlayM4_SetDisplayCallBack(lPort, StreamClientHikanCBFun))
			{

				DB33_PlayM4_CloseStream(lPort);
				DB33_PlayM4_FreePort(lPort);
				break;
			}

			if(DlgSetSystem.m_display_preview)
			{
				pWnd= DlgMain->DlgScreen.m_screenPannel.GetPage(screenNo);
				if (!pWnd)
				{
					DB33_PlayM4_CloseStream(lPort);
					DB33_PlayM4_FreePort(lPort);
					return 0;
				}
				if (!DB33_PlayM4_Play(lPort, pWnd->m_hWnd)) //播放开始
				{
					DB33_PlayM4_CloseStream(lPort);
					DB33_PlayM4_FreePort(lPort);
					break;
				}
			}
			else
			{
				if (!DB33_PlayM4_Play(lPort,NULL)) //播放开始
				{
					DB33_PlayM4_CloseStream(lPort);
					DB33_PlayM4_FreePort(lPort);
					break;
				}
			}
		}

		break;
	
	case STREAM_DATA:   // 码流数据 
		if (ilen > 0 && lPort != -1)
		{
			if (!DB33_PlayM4_InputData(lPort, (PBYTE)pdata, ilen))
			{
				DB33_PlayM4_CloseStream(lPort);
				DB33_PlayM4_FreePort(lPort);
				break;
			} 

		}
		break; 
		
	case STREAM_PLAYBACK_FINISH:    // 回放/下载至结束 =;
		break; 
	default: 
		break; 
	} 

	return 0;

}
int CALLBACK StreamClientSDKDataFunc(int sessionhandle, void* userdata, int datatype, void* pdata,
									 int ilen) 
{ 
	if ((sessionhandle  < 0) || (NULL == pdata) || (ilen <= 0)) 
	{ 
		return -1; 
	} 

	StreamClientHikanCBinitFun( sessionhandle, userdata,  datatype,  pdata,  ilen);

	return 0; 
} 

int  CALLBACK StreamClientSDKMsgFunc(int  sessionhandle,  void*  userdata,  int  opt, 
									 void*  param1,  void*  param2, void* param3,  void* param4) 
{   
	if (sessionhandle  < 0) 
	{ 
		return 0; 
	} 
	char *name=(char*)userdata;

	char errDescribe[512] = {0}; 
	sprintf(errDescribe,  "StreamClient recv errCode:%d, errDescribe:%s", 
		opt, 
		StreamClient_GetErrMsgByErrCode(opt)); 

	DlgMain->ShowCameraMessage(name,errDescribe,FALSE);

	return 0; 
} 

CStreamClientSDK::CStreamClientSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
	{
		//m_LoginHandle[i] = -1;
		m_RealHandle[i] = -1;
		m_lPort[i] = -1;
	}
	m_ptzRealHandle = -1;
	//m_ptzLoginHandle = -1;
	SDKInit();
}

CStreamClientSDK::~CStreamClientSDK()
{
	for(int i=0;i<MAX_DEVICE_NUM;i++)
		StopPlay(i);

	PtzStopPlay();
	StreamClient_FiniLib();
}

void CStreamClientSDK::SDKInit()
{
	// 初始化
	StreamClient_InitLib();
}

bool CStreamClientSDK::StartPlay(int screenNo,char *name,char *sip,
								 char *user,char *psw,HWND hWnd,int subtype,
								 char *Rtspurl,int RTP,int DecodeTag)
{
	if(m_RealHandle[screenNo] >=0)
	{
		StopPlay(screenNo);
	}
	int i;
	int transmethod;

	m_RealHandle[screenNo]=StreamClient_CreateSession();
	if(m_RealHandle[screenNo]>=0)
	{
		
		i=StreamClient_SetMsgCallBack(m_RealHandle[screenNo],StreamClientSDKMsgFunc,name);
		if(i)
		{
			DlgMain->ShowCameraMessage(name,(char*)StreamClient_GetErrMsgByErrCode(i),FALSE);
			StreamClient_DestroySession(m_RealHandle[screenNo]);
			m_RealHandle[screenNo]=-1;
			return false;
		}
		
		//解码
		if(DecodeTag==DECODETAG_HAIKANG)
		{
			i=StreamClient_SetPsDataCallBack(m_RealHandle[screenNo],StreamClientSDKDataFunc,this);
		}
		else if(DecodeTag==DECODETAG_DAHUA)
		{
			i=StreamClient_SetDataCallBack(m_RealHandle[screenNo],StreamClientSDKDataFunc,this);
		}

		if(i)
		{
			DlgMain->ShowCameraMessage(name,(char*)StreamClient_GetErrMsgByErrCode(i),FALSE);
			StreamClient_DestroySession(m_RealHandle[screenNo]);
			m_RealHandle[screenNo]=-1;
			return false;
		}

		if(0==RTP)
			transmethod=RTPRTSP_TRANSMODE;
		else	if(1==RTP)
			transmethod=RTPUDP_TRANSMODE;
		else	if(2==RTP)
			transmethod=RTPMCAST_TRANSMODE;
		else
		{
			DlgMain->ShowCameraMessage(name,"RTP选择错误",FALSE);
			StreamClient_DestroySession(m_RealHandle[screenNo]);
			m_RealHandle[screenNo]=-1;
			return false;
		}

		i=StreamClient_Start(m_RealHandle[screenNo], NULL, Rtspurl, "StreamClient",  transmethod, user, psw);
		if(i)
		{
			DlgMain->ShowCameraMessage(name,(char*)StreamClient_GetErrMsgByErrCode(i),FALSE);
			StreamClient_DestroySession(m_RealHandle[screenNo]);
			m_RealHandle[screenNo]=-1;
			return false;
		}
	}
	else
	{
		if(-3 == m_RealHandle[screenNo])
			DlgMain->ShowCameraMessage(name,"STREAM_CLIENT_NO_RTSP_SESSION流媒体客户端会话已用尽!",FALSE);
		else	if(-2 == m_RealHandle[screenNo])
			DlgMain->ShowCameraMessage(name,"STREAM_CLIENT_NO_INIT 流媒体客户端未初始化!",FALSE);

		return false;
	}
	DlgMain->ShowCameraMessage(name,"连接成功",FALSE);
	return true;
}

void CStreamClientSDK::StopPlay(int screenNo)
{
	
	if(m_RealHandle[screenNo] >=0)
	{
		//关闭预览
		StreamClient_Stop(m_RealHandle[screenNo]);
		
		//注销用户
		StreamClient_DestroySession(m_RealHandle[screenNo]);
		//停止播放
		DB33_PlayM4_Stop(m_lPort[screenNo]);
		DB33_PlayM4_CloseStream(m_lPort[screenNo]);

		DB33_PlayM4_FreePort(m_lPort[screenNo]);
		
#if OPEN_CARDETECT_CODE 	
		//停止识别
		if(false == DlgMain->DlgScreen.m_videoInfo[screenNo].enableDetect)
			DlgMain->DlgScreen.CarDetect[screenNo].Stop();
#endif

	}
	m_lPort[screenNo]=-1;
	m_RealHandle[screenNo]=-1;


}

void CStreamClientSDK::PtzControl(LONG lRealHandle, int type, BOOL dwStop, int param)
{
	DlgMain->ShowCameraMessage("NULL","流媒体 不支持云台!",TRUE);
}

bool CStreamClientSDK::PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd)
{
	DlgMain->ShowCameraMessage("NULL","流媒体 不支持云台!",TRUE);
	return false;

}

void CStreamClientSDK::PtzStopPlay()
{
	return ;
}

void CStreamClientSDK::Capture(int screenNo,char *filename)
{

	CapturePath[screenNo]=filename;
	Sleep(150);

}

int CStreamClientSDK::GetPortWndindex(long lport)
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
int CStreamClientSDK::GetHandleWindID(long RealHandle)
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


int CStreamClientSDK::StartRecord(int screenNo,char *filename)
{
	DlgMain->ShowCameraMessage("NULL","流媒体 不支持录像!",TRUE);
	return 0;
}

int CStreamClientSDK::StopRecord(int screenNo)
{
	return 0;
}


//#endif


#endif