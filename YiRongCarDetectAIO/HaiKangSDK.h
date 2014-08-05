
/***********************************************************************************
//说明 :海康SDK
//时间 :2013-04-18 17:39:00
***********************************************************************************/
#ifndef _HAI_KANG_SDK_
#define _HAI_KANG_SDK_

#if OPEN_HAIKANG_SDK


class CHaikangSDK
{
public:
	CHaikangSDK();
	virtual ~CHaikangSDK();
	long m_RealHandle[MAX_DEVICE_NUM];
	long m_LoginHandle[MAX_DEVICE_NUM];
	long m_ptzRealHandle;
	long m_ptzLoginHandle;
	long m_lPort[MAX_DEVICE_NUM];
	BOOL m_bPTZCtrl;

	CString CapturePath[MAX_DEVICE_NUM];

	void SDKInit();
	bool StartPlay(int screenNo,char *name,char *sip,int nPort,int channel,
					char *user,char *psw,HWND hWnd,int subtype);
	void StopPlay(int screenNo);
	void Capture(int screenNo,char *filename);
	void PtzControl(LONG lRealHandle, int type, BOOL dwStop, int param);
	bool PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd);
	void PtzStopPlay();
	//通过port获取到窗口号
	int GetPortWndindex(long lport);
	int GetHandleWindID(int RealHandle);

	int StartRecord(int screenNo,char *filename);
	int StopRecord(int screenNo);
};



#endif

#endif
