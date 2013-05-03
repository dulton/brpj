
#ifndef _MY_CAMERA_
#define _MY_CAMERA_

#include "DahuaSDK.h"
#include "HaiKangSDK.h"



class CMyCamera
{
public:
	CMyCamera();
	virtual ~CMyCamera();
	long m_RealHandle[MAX_DEVICE_NUM];

	CDahuaSDK     m_dahua;
	CHaikangSDK   m_haikang;

	void SDKInit();
	bool StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,char *user,char *psw,HWND hWnd,int subtype);
	void StopPlay(int venderID,int screenNo);
	void Capture(int venderID,int screenNo,char *filename);

private:
	long m_LoginHandle[MAX_DEVICE_NUM];
};











#endif