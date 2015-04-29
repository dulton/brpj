#pragma once

#include <afx.h>

/////////////////////////////////////////////////////////////////////////
//设备回调信息
#define ERROR_NET_NOERROR					"No error"		
#define ERROR_NET_ERROR						"Unknown error"		
#define ERROR_NET_SYSTEM_ERROR				"Windows system error"
#define ERROR_NET_NETWORK_ERROR				"Network error, probably because of network-time-out"		
#define ERROR_NET_DEV_VER_NOMATCH			"No match protocol"		
#define ERROR_NET_INVALID_HANDLE			"Invalid handle"		
#define ERROR_NET_OPEN_CHANNEL_ERROR		"Failed to open channel"		
#define ERROR_NET_CLOSE_CHANNEL_ERROR		"Failed to close channel"		
#define ERROR_NET_ILLEGAL_PARAM				"Illegal parameter"		
#define ERROR_NET_SDK_INIT_ERROR			"Failed to initialize SDK"		
#define ERROR_NET_SDK_UNINIT_ERROR			"Failed to uninitialize SDK"		
#define ERROR_NET_RENDER_OPEN_ERROR			"Failed to open render"		
#define ERROR_NET_DEC_OPEN_ERROR			"Failed to open decoder"
#define ERROR_NET_DEC_CLOSE_ERROR			"Failed to close decoder"
#define ERROR_NET_MULTIPLAY_NOCHANNEL		"Channel number is zero"	
#define ERROR_NET_TALK_INIT_ERROR			"Failed to initialize Recoder"	
#define ERROR_NET_TALK_NOT_INIT				"Recorder is not initialized"			
#define ERROR_NET_TALK_SENDDATA_ERROR		"Error while sending audio data"
#define ERROR_NET_NO_TALK_CHANNEL			"No talking channel found"
#define ERROR_NET_NO_AUDIO					"Audio not openned"								
#define ERROR_NET_REAL_ALREADY_SAVING		"Already saving real data"		
#define ERROR_NET_NOT_SAVING				"Not saving real data"
#define ERROR_NET_OPEN_FILE_ERROR			"Failed to open file"
#define ERROR_NET_PTZ_SET_TIMER_ERROR		"Failed to active PTZ timer"		
#define ERROR_NET_RETURN_DATA_ERROR			"Unexpected data received"	
#define ERROR_NET_INSUFFICIENT_BUFFER		"Insufficient buffer"		
#define ERROR_NET_NOT_SUPPORTED				"Not supported"		
#define ERROR_NET_NO_RECORD_FOUND			"No record found"
#define ERROR_NET_LOGIN_ERROR_PASSWORD		"Incorrect password"
#define ERROR_NET_LOGIN_ERROR_USER			"User not Exist"	
#define ERROR_NET_LOGIN_ERROR_TIMEOUT		"Login Timeout"		
#define ERROR_NET_LOGIN_ERROR_RELOGGIN		"Curent User Has Logined"		
#define ERROR_NET_LOGIN_ERROR_LOCKED		"Curent User is Locked"		
#define ERROR_NET_LOGIN_ERROR_BLACKLIST		"Curent User is in blacklist"		
#define ERROR_NET_LOGIN_ERROR_BUSY			"System Busying"		
#define ERROR_NET_LOGIN_ERROR_CONNECT		"Connect Error"		
#define ERROR_NET_LOGIN_ERROR_NETWORK		"Network Error"										
#define ERROR_NET_RENDER_SOUND_ON_ERROR		"Render error: open audio"		
#define ERROR_NET_RENDER_SOUND_OFF_ERROR	"Render error: close audio"		
#define ERROR_NET_RENDER_SET_VOLUME_ERROR	"Render error: set volume"		
#define ERROR_NET_RENDER_ADJUST_ERROR		"Render error: set image parameter"		
#define ERROR_NET_RENDER_PAUSE_ERROR		"Render error: pause play"
#define ERROR_NET_RENDER_SNAP_ERROR			"Render error: snap picture"
#define ERROR_NET_RENDER_STEP_ERROR			"Render error: step"
#define ERROR_NET_RENDER_FRAMERATE_ERROR	"Render error: set framerate"		
#define ERROR_NET_CONFIG_DEVBUSY			"Device is busy"
#define ERROR_NET_CONFIG_DATAILLEGAL		"Illegal config data"
#define ERROR_NET_NO_INIT					"The SDK is not initialized"		
#define ERROR_NET_DOWNLOAD_END				"Download ended"

/////////////////////////////////////////////////////////////////////////
//连接错误定义
#define MSG_ERROR_PASSWORD			"Incorrect Password"
#define MSG_ERROR_USER				"User not Exist"
#define MSG_ERROR_TIMEOUT			"Login Timeout"
#define MSG_ERROR_RELOGGIN			"Curent User Has Logined"
#define MSG_ERROR_LOCKED			"Curent User Locked"
#define MSG_ERROR_BLACKLIST			"Curent User Invalid"
#define MSG_ERROR_BUSY				"System Busy"
#define MSG_ERROR_CONNECT			"Cant find host"
#define MSG_ERROR_NETWORK			"Network Error"
#define MSG_ERROR_LISTEN			"Start Listen Error"
#define MSG_DEVICE_DISCONNECT		"device disconnect"

#include"dahuaSDKNew/dhnetsdk.h"
#pragma comment( lib, "dahuaSDKNew/dhnetsdk.lib")

class CDahuaSDKNew
{
public:
	CDahuaSDKNew(void);
	~CDahuaSDKNew(void);

	int m_RealHandle[MAX_DEVICE_NUM];
	LLONG m_LoginHandle[MAX_DEVICE_NUM];
	int m_direction[MAX_DEVICE_NUM];
	BOOL m_bPTZCtrl;
	long m_ptzRealHandle;
	long m_ptzLoginHandle;

	void SDKInit();
	bool StartPlay(int screenNo,char *name,char *sip,int nPort,int channel,
		char *user,char *psw,HWND hWnd,int subtype,int Direction);
	void StopPlay(int screenNo);
	void Capture(long screenNo,char *filename);
	bool PtzStartPlay(char *sip,int nPort,int channel,char *user,char *psw,HWND hWnd);
	void PtzStopPlay();
	void PtzControl(long lLoginID, int type, BOOL dwStop, int param);
	char *RuntimeMessage(void);
	void GetConnectError(char *name,int error,int flag);

	int StartRecord(int screenNo,char *filename);
	int StopRecord(int screenNo);
	int GetHandleWindID(int RealHandle);
};
