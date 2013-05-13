
#ifndef _MY_CAMERA_
#define _MY_CAMERA_

#include "DahuaSDK.h"
#include "HaiKangSDK.h"
#include "YaAnSDK.h"

//0---º£¿µ,1---´ó»ª
enum DEVICE_VENDERID_ENUM
{
	VENDER_TYPE_HAIKANG=0, 
	VENDER_TYPE_DAHUA=1, 
	VENDER_TYPE_YAAN=2, 
};


class CMyCamera
{
public:
	CMyCamera();
	virtual ~CMyCamera();
	long m_RealHandle[MAX_DEVICE_NUM];

	CDahuaSDK     m_dahua;
	CHaikangSDK   m_haikang;
	CYaAnSDK	  m_yaAn;

	void SDKInit();
	bool StartPlay(int venderID,int screenNo,char *name,char *sip,int nPort,char *user,char *psw,HWND hWnd,int subtype);
	void StopPlay(int venderID,int screenNo);
	void Capture(int venderID,int screenNo,char *filename);
	void PtzControl(int venderID, int screenNo, int type, BOOL dwStop, int param);

	int StartRecord(int venderID,int screenNo,char *filename);
	void StopRecord(int venderID,int screenNo);

private:
	long m_LoginHandle[MAX_DEVICE_NUM];
};











#endif