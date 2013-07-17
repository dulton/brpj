//NetHostLib.h
#ifndef __NETHOSTLIB_H__
#define __NETHOSTLIB_H__

#ifndef CNET_APIIMPORT
#define CNET_APIIMPORT  extern "C"__declspec(dllexport)
#endif//CNET_APIIMPORT


typedef struct
{
	char *IpAddr;			/* 服务器地址 */
	WORD  wPort;			/* 服务器端口号 */
	BYTE *pMac;				/* MAC地址 */
	char *pSerName;			/* 服务器名称 */
	char *pSerialNo;		/* 服务器序列号 */
	int   nAlarmChannel;	/* 报警通道 */
	int   nAlarmType;		/* 报警类型 */
}SERVERALARMINFO;

typedef struct
{
	char *IpAddr;			/* 服务器地址 */
	WORD  wPort;			/* 服务器端口 */
	BYTE *pMac;				/* MAC地址 */
	char *pSerName;			/* 服务器名称 */
	char *pSerialNo;		/* 序列号 */
	int   nChannel;			/* 通道号 */
	int   nyear;			/* 文件时间 */
	BYTE  nmonth;
	BYTE  nday;
	BYTE  nhour;
	BYTE  nminute;
	BYTE  nsecond;

	int   nFileType;		/* 1：MPEG4码流，2：JPEG图像 */
	int   nRecvType;		/* 录像类型 */
}SERVERRECVHEADINFO;

typedef void (WINAPI *pSerAlarmInfoCallback)(SERVERALARMINFO *pAlarmInfo,void *pUserData);
typedef void*(WINAPI *pSerRecvHeadCallback) (SERVERRECVHEADINFO *pRecvHead,void *pUserData);
typedef int  (WINAPI *pSerRecvDataCallback) (char *pBuffer,int size,void *context,void *pUserData);

CNET_APIIMPORT BOOL  __stdcall NetHostLibInit();
CNET_APIIMPORT BOOL  __stdcall NetHostLibFree();
CNET_APIIMPORT BOOL  __stdcall NetHostLibRegCallBack(pSerAlarmInfoCallback pAlarmCallback,void *pAlarmData,
													 pSerRecvHeadCallback pRecvHeadCallback,void *pHeadData,
													 pSerRecvDataCallback pRecvDataCallback,void *pDataData);

CNET_APIIMPORT BOOL  __stdcall NetHostAlarm_Start(char *m_plocalip,WORD m_wPort);
CNET_APIIMPORT BOOL  __stdcall NetHostAlarm_Stop();

#endif//__NETHOSTLIB_H__
