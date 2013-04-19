
/***********************************************************************************
//说明 :海康SDK
//时间 :2013-04-18 17:39:00
***********************************************************************************/
#ifndef _HAI_KANG_SDK_
#define _HAI_KANG_SDK_

class CHaikangSDK
{
public:
	CHaikangSDK();
	virtual ~CHaikangSDK();
	long m_RealHandle[MAX_DEVICE_NUM];
	long m_lPort[MAX_DEVICE_NUM];

	void SDKInit();
	bool StartPlay(int screenNo,char *name,char *sip,int nPort,char *user,char *psw,HWND hWnd,int subtype);
	void StopPlay(int screenNo);
	void Capture(int screenNo,char *filename);
	//通过port获取到窗口号
	int GetPortWndindex(long lport);

private:
	long m_LoginHandle[MAX_DEVICE_NUM];
};



#endif