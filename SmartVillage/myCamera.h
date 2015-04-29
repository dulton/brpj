
#ifndef _MY_CAMERA_
#define _MY_CAMERA_

#include "DahuaSDK.h"
#include "DahuaSDKNew.h"
#include "HaiKangSDK.h"
#include "StreamClientSDK.h"
#include "vlcRtspSDK.h"



class CMyCamera
{
public:
	CMyCamera();
	virtual ~CMyCamera();
	
	long m_LoginHandle[MAX_DEVICE_NUM];
	long m_RealHandle[MAX_DEVICE_NUM];

#if	OPEN_DAHUA_SDK
	CDahuaSDK     m_dahua;
#endif

#if	OPEN_DAHUA_SDK_NEW
	CDahuaSDKNew     m_dahua;
#endif

#if OPEN_HAIKANG_SDK
	CHaikangSDK   m_haikang;
#endif

#if OPEN_STREAM_CLIENT_SDK
	CStreamClientSDK m_StreamClient;
#endif

#if OPEN_VLC_RTSP_SDK
	CVlcRtspSDK m_VlcRtsp[MAX_DEVICE_NUM];
#endif



	void SDKInit();
	bool StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,int channel,
					char *user,char *psw,HWND hWnd,int subtype,char *Rtspurl,int RTP,int DecodeTag,int Direction);
	void StopPlay(int venderID,int screenNo);
	void Capture(int venderID,int screenNo,char *filename);
	void PtzControl(int venderID, int screenNo, int type, BOOL dwStop, int param);

	int StartRecord(int venderID,int screenNo,char *filename);
	void StopRecord(int venderID,int screenNo);

private:

};











#endif