
#ifndef _MY_CAMERA_
#define _MY_CAMERA_

#include "DahuaSDK.h"
#include "HaiKangSDK.h"
#include "YaAnSDK.h"
#include "YaAnSDKNew.h"
#include "StreamClientSDK.h"

//0---º£¿µ,1---´ó»ª
enum DEVICE_VENDERID_ENUM
{
	VENDER_TYPE_HAIKANG=0, 
	VENDER_TYPE_DAHUA=1, 
	VENDER_TYPE_YAAN=2, 
	VENDER_TYPE_YAAN_NEW=3, 
	VENDER_TYPE_STREAM=4, 
};


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

#if OPEN_HAIKANG_SDK
	CHaikangSDK   m_haikang;
#endif

#if OPEN_YAAN_SDK
	CYaAnSDK	  m_yaAn;
#endif

#if OPEN_STREAM_CLIENT_SDK
	CStreamClientSDK m_StreamClient;
#endif

#if OPEN_YAAN_NEW_SDK
	CYaAnSDKNew	  m_yaAnNew;
#endif

	void SDKInit();
	bool StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,int channel,
					char *user,char *psw,HWND hWnd,int subtype,char *Rtspurl,int RTP,int DecodeTag);
	void StopPlay(int venderID,int screenNo);
	void Capture(int venderID,int screenNo,char *filename);
	void PtzControl(int venderID, int screenNo, int type, BOOL dwStop, int param);

	int StartRecord(int venderID,int screenNo,char *filename);
	void StopRecord(int venderID,int screenNo);

private:

};











#endif