
#include "stdafx.h"

#if OPEN_VLC_RTSP_SDK


#include "vlcRtspSDK.h"

//////////////////////////////////
#include "CarDetect.h"
//////////////////////////////////
#include "FaceDetect.h"
//////////////////////////////////
#include "SmartVillage.h"
#include "SmartVillageDlg.h"
extern CSmartVillageDlg *DlgMain;
#include "DLGSetSystem.h"
extern CDLGSetSystem DlgSetSystem;

#include "YRSVMySQL.h"
extern CYRSVMySQL MySqlIO;

#include "YUV2RGB.h"
#include "YuanRtspClient.h"


void *Libvlc_Video_Lock_Callback(void *opaque, void **planes)
{
	CVlcRtspSDK *pDlg = (CVlcRtspSDK *)opaque;
	if (pDlg != NULL)
	{
		// 先加锁
		pDlg->Lock();
		memset(pDlg->RGBdata, 0, VIDEO_WIDTH*VIDEO_HEIGHT*3);
		*planes = pDlg->RGBdata;
		return *planes;
	}
	return NULL;
}
void Libvlc_Video_UnLock_Callback(void *opaque, void *picture, void *const *planes)
{
	CVlcRtspSDK *pDlg = (CVlcRtspSDK *)opaque;
	if (pDlg != NULL)
	{
		// 最后解锁
		pDlg->UnLock();

	}
}
void Libvlc_Video_Display_Callback(void *opaque, void *picture)
{
	CVlcRtspSDK *pDlg = (CVlcRtspSDK *)opaque;
	if (pDlg == NULL)
		return ;

	if(pDlg->Width ==0)
		return ;

	if( ! pDlg->CapturePath.IsEmpty())
	{
		ZOGDramBMP(pDlg->CapturePath.GetBuffer(0),(unsigned char *)picture,pDlg->Width,pDlg->Height);
		pDlg->CapturePath="";
	}

	memcpy(pDlg->m_DC_Cache.GetCacheBuffer(), picture, pDlg->Width*pDlg->Height*pDlg->m_nBitCount/8);

	CRect rect;
	::GetWindowRect(pDlg->hWnd,&rect);
	pDlg->pDc->StretchBlt(0, 0,rect.Width(), rect.Height(), 
		pDlg->m_DC_Cache.GetCacheDC(), 0, 0,pDlg->Width,pDlg->Height ,SRCCOPY);


	//车牌识别
#if OPEN_CARDETECT_CODE 	

	//启用识别
	if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].enableCarDetect)
	{
		DlgMain->DlgTabVideo.DlgScreen.CarAdd[pDlg->screenNo]++;
		if(0==DlgMain->DlgTabVideo.DlgScreen.CarAdd[pDlg->screenNo]%CAR_JUMP_NUM)
		{
			DlgMain->DlgTabVideo.DlgScreen.CarAdd[pDlg->screenNo]=0;

			//拷贝数值
			DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].m_playhandle=pDlg->screenNo;

			DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].camid=
				DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].camID;

			strcpy(DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].cam_name,
				DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].name.GetBuffer(0));

			if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].ip.GetLength() >1)
			{
				strcpy(DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].l_ipaddr,
					DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].ip.GetBuffer(0));
			}
			else
				strcpy(DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].l_ipaddr,"0.0.0.0");

			//颜色LC_VIDEO_FORMAT_YV12 与颜色LC_VIDEO_FORMAT_I420 相反
			DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].Start(LC_VIDEO_FORMAT_BGR24,
				(unsigned char *)picture,pDlg->Width,pDlg->Height,pDlg->Width*pDlg->Height*3);

			DlgMain->DlgTabVideo.DlgScreen.CarDetect[pDlg->screenNo].Result();
		}
	}

#endif


#if OPEN_FACEDETECT_CODE
	//启用识别
	if(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].enableFaceDetect)
	{
		DlgMain->DlgTabVideo.DlgScreen.FaceAdd[pDlg->screenNo]++;
		if(0==DlgMain->DlgTabVideo.DlgScreen.FaceAdd[pDlg->screenNo]%FACE_JUMP_NUM)
		{
			DlgMain->DlgTabVideo.DlgScreen.FaceAdd[pDlg->screenNo]=0;

			//	YUV2RGB((unsigned char *)pBuf, 	DlgMain->DlgTabVideo.DlgScreen.RGBdata[screenNo], 	nWidth,nHeight);

			//拷贝数值
			DlgMain->DlgTabVideo.DlgScreen.FaceDetect[pDlg->screenNo].m_playhandle=pDlg->screenNo;

			DlgMain->DlgTabVideo.DlgScreen.FaceDetect[pDlg->screenNo].camid=
				DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].camID;

			strcpy(DlgMain->DlgTabVideo.DlgScreen.FaceDetect[pDlg->screenNo].cam_name,
				DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].name.GetBuffer(0));

			strcpy(DlgMain->DlgTabVideo.DlgScreen.FaceDetect[pDlg->screenNo].l_ipaddr,
				DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].ip.GetBuffer(0));

			DlgMain->DlgTabVideo.DlgScreen.FaceDetect[pDlg->screenNo].Start(VIDEO_FORMAT_RGB888,
				(unsigned char *)picture,pDlg->Width,pDlg->Height,pDlg->Width*3,pDlg->Width*pDlg->Height*3);

			//ZOGDramBMP("d:\\112212121.bmp",DlgMain->DlgTabVideo.DlgScreen.RGBdata[screenNo], nWidth,nHeight);
		}
	}

#endif



}

unsigned Libvlc_Video_Format_Callback(void **opaque, char *chroma,  unsigned *width,
									  unsigned *height, unsigned *pitches, unsigned *lines)
{
	CVlcRtspSDK *pDlg = (CVlcRtspSDK *)(*opaque);
	if (pDlg != NULL)
	{
		// 改变解码格式
		strcpy(chroma, "RV24"); // "RGBA"  //在vlc_fourcc.h中定义
		*pitches = *width * pDlg->m_nBitCount/8; // 每行的像素
		*lines = *height; // 行数
		pDlg->Width = *width;
		pDlg->Height = *height;

		pDlg->m_DC_Cache.FreeCache();
		// 创建缓存DC
		pDlg->m_DC_Cache.CreateCache(*width, *height,pDlg->m_nBitCount);

		DlgMain->ShowCameraMessage(DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[pDlg->screenNo].name.GetBuffer(0),
			"连接成功，格式获取成功",FALSE);
	}
	return 1;
}
void Libvlc_Video_Cleanup_Callback(void *opaque)
{// 停止播放后，可以清理内存

}

CVlcRtspSDK::CVlcRtspSDK()
{
	screenNo=-1;
	CapturePath="";

	hWnd=NULL;

	m_nBitCount = 24;

	m_pLibvlc_Inst = NULL;
	m_pLibvlc_Mp = NULL;
	m_pLibvlc_m=NULL;

	Width=0;
	Height=0;

	RGBdata=(unsigned char*)calloc(VIDEO_WIDTH*VIDEO_HEIGHT*3,sizeof(unsigned char));
	// 创建备份缓存
	//m_DC_Cache.CreateCache(Width, Height, m_nBitCount);

	InitializeCriticalSection(&lockflag);

	m_pLibvlc_Inst = libvlc_new(0,NULL);
}

CVlcRtspSDK::~CVlcRtspSDK()
{
	StopPlay();

	/* Free the media_player */
	if (m_pLibvlc_Inst != NULL)
		libvlc_release(m_pLibvlc_Inst);

	if (RGBdata != NULL)
	{
		free(RGBdata);
		RGBdata = NULL;
	}


	DeleteCriticalSection(&lockflag);

}
void CVlcRtspSDK::Lock()
{
	EnterCriticalSection(&lockflag);
}
void CVlcRtspSDK::UnLock()
{
	LeaveCriticalSection(&lockflag);
}

bool CVlcRtspSDK::StartPlay(int screenNum,char *name,HWND inhWnd,char *Rtspurl)
{
	screenNo=screenNum;

	StopPlay();
	hWnd=inhWnd;
	hDC   =   ::GetDC(hWnd); 
	pDc   =   CDC::FromHandle(hDC);   
	pDc->SetStretchBltMode(COLORONCOLOR);

	wchar_t wbuff[1024];
	char utf8[1024];
	MultiByteToWideChar(CP_ACP, 0, Rtspurl, -1, wbuff, 1024);  
	// buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);  
	//  utf8 = new char[buffLen+1];  
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)utf8, 1024, 0, 0);  
	const char *p=NULL;

	// 判断rtsp地址是否可以连接
	// 使用url去创建一个CYuanRtspClient对象
#if !TEST_DEBUG
	CYuanRtspClient YuanRtsp(Rtspurl);
	ERTSPERROR eRet = YuanRtsp.InitSocket();
	if (eRet == E_RTSP_SUCCESS)
	{
		eRet = YuanRtsp.JudgeCorrect();
		if (eRet != E_RTSP_SUCCESS)
		{
			DlgMain->ShowCameraMessage(name,"RTSP连接失败",FALSE);
			return false;
		}
	}
	else
	{
		DlgMain->ShowCameraMessage(name,"RTSP连接失败",FALSE);
		return false;
	}
#endif


#if TEST_DEBUG
	m_pLibvlc_m =	libvlc_media_new_path(m_pLibvlc_Inst,utf8);
#else
	m_pLibvlc_m = libvlc_media_new_location(m_pLibvlc_Inst, utf8);
#endif

	//libvlc_media_new_path
	m_pLibvlc_Mp = libvlc_media_player_new_from_media(m_pLibvlc_m);

	libvlc_media_release(m_pLibvlc_m);

	//libvlc_media_player_set_hwnd(m_pLibvlc_Mp, hWnd);

	libvlc_video_set_callbacks(m_pLibvlc_Mp, Libvlc_Video_Lock_Callback, Libvlc_Video_UnLock_Callback,
		Libvlc_Video_Display_Callback, (void *)this);

	libvlc_video_set_format_callbacks(m_pLibvlc_Mp, Libvlc_Video_Format_Callback, Libvlc_Video_Cleanup_Callback);


	if(0==libvlc_media_player_play(m_pLibvlc_Mp)) 
	{	

		DlgMain->ShowCameraMessage(name,"连接成功",FALSE);
	}
	else
	{	

		DlgMain->ShowCameraMessage(name,"RTSP连接失败",FALSE);
		return false;
	}


	return true;
}

void CVlcRtspSDK::StopPlay()
{
	if (m_pLibvlc_Mp != NULL)
	{
		libvlc_media_player_stop(m_pLibvlc_Mp);
		libvlc_media_player_release(m_pLibvlc_Mp);
		m_pLibvlc_Mp = NULL;
		m_pLibvlc_m=NULL;

	}

	if(screenNo>=0)
	{
#if OPEN_CARDETECT_CODE 	
		//停止识别
		if(false == DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableCarDetect)
			DlgMain->DlgTabVideo.DlgScreen.CarDetect[screenNo].Stop();
	
		DlgMain->DlgTabVideo.DlgScreen.CarAdd[screenNo]=0;
#endif

#if OPEN_FACEDETECT_CODE 	
		//停止识别
		if(false == DlgMain->DlgTabVideo.DlgScreen.m_videoInfo[screenNo].enableFaceDetect)
			DlgMain->DlgTabVideo.DlgScreen.FaceDetect[screenNo].Stop();

		DlgMain->DlgTabVideo.DlgScreen.FaceAdd[screenNo]=0;
	
#endif
	}
	if(hWnd!=NULL)
	{
		::ReleaseDC(hWnd,hDC);//必须和GetDC配对
		hWnd=NULL;
	}

	m_DC_Cache.FreeCache();
	Width=0;
	Height=0;

}

void CVlcRtspSDK::Capture(char *filename)
{
	CapturePath=filename;
	Sleep(150);
}



#endif